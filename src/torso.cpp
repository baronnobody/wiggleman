#include "torso.h"
#include <QPainter>

Torso::Torso(QGraphicsItem* parent) : QObject(), QGraphicsItem(parent)
{
}

QRectF Torso::boundingRect() const
{
    return QRectF();
}

void Torso::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // draw the bottom half
    float bottomHeight = height * gradLength;
    bottomHeight += hipCurvature;

    QRectF bottom(0, 0, width, bottomHeight);
    bottom.moveCenter(QPointF(0, 0));
    bottom.moveBottom(0);

    QPainterPath bottomPath;
    bottomPath.addRoundedRect(bottom, hipCurvature, hipCurvature);

    painter->fillPath(bottomPath, QBrush(bottomColor));

    // draw the top half
    float topHeight = height * (1.0f - gradLength);

    QRectF top(0, 0, width, topHeight);
    top.moveCenter(QPointF(0, 0));
    top.moveTop(-height);

    QPainterPath topPath;
    topPath.addRoundedRect(top, shoulderCurvature, shoulderCurvature);

    painter->fillPath(topPath, QBrush(topColor));

    // draw the overlapping rect
    QRectF over(0, 0, width, shoulderCurvature + 1);
    over.moveCenter(QPointF(0, 0));
    over.moveBottom(topHeight - height);

    painter->fillRect(over, topColor);
}

QPainterPath Torso::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());

    return path;
}

Jacket::Jacket(QGraphicsItem* parent) : Torso(parent) {}

void Jacket::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!drawJacket) return;

    // set the clipping rect to exclude the middle
    float widthWithGap = width * 0.5f * (1.0f - gap) + jacketWidth;

    QRectF leftClip(0, 0, widthWithGap, jacketHeight + shoulderCurvature);
    leftClip.moveLeft(- jacketWidth - (width * 0.5f));
    leftClip.moveTop(-height);

    QRectF rightClip(0, 0, widthWithGap + jacketWidth, jacketHeight + shoulderCurvature);
    rightClip.moveRight(jacketWidth + (width * 0.5f));
    rightClip.moveTop(-height);

    QPainterPath clipPath;
    clipPath.addRect(leftClip);
    clipPath.addRect(rightClip);

    painter->setClipPath(clipPath);

    // draw the top half
    float topHeight = (shoulderCurvature) * 2;

    QRectF top(0, 0, width + 2 * jacketWidth, topHeight);
    top.moveCenter(QPointF(0, 0));
    top.moveTop(-height);

    QPainterPath topPath;
    topPath.addRoundedRect(top, shoulderCurvature, shoulderCurvature);

    painter->fillPath(topPath, QBrush(color));

    // draw bottom
    QRectF bottom(0, 0, width + 2 * jacketWidth, jacketHeight);
    bottom.moveCenter(QPointF(0, 0));
    bottom.moveTop(- (height - shoulderCurvature));

    painter->fillRect(bottom, color);
}
