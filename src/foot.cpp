#include "foot.h"
#include <QPainter>

Foot::Foot(float w, float h, float lw, QGraphicsItem* parent) : QObject(), QGraphicsItem(parent)
{
    width = w;
    height = h;
    legWidth = lw;
}

QRectF Foot::boundingRect() const
{
    return shape().boundingRect();
}

void Foot::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // draw the foot
    QPainterPath footPath = shape();

    painter->fillPath(footPath, QBrush(color));
}

QPainterPath Foot::shape() const
{
    QPainterPath path;

    float w = qMax(legWidth / 2, width);

    path.moveTo(QPointF(legWidth / 2, height));
    path.lineTo(QPointF(-w, height));
    path.lineTo(QPointF(-legWidth / 2, -1.0f));
    path.lineTo(QPointF(legWidth / 2, -1.0f));

    return path;
}
