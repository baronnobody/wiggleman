#include "pathpoint.h"
#include "types.h"

namespace paths
{

PathPoint::PathPoint(const QPointF &p, IKPath* parentPath, bool movable)
    : QObject(parentPath),
      QGraphicsItem(parentPath),
      m_movable(movable),
      m_parentPath(parentPath)
{
    setZValue(100);

    setPos(p);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    SetMovable(movable);

}

QRectF PathPoint::boundingRect() const
{
    float w = 1.0f;

    if (m_parentPath)
        w = m_parentPath->PenWidth() * 0.5f;

    return QRectF(-w, -w, w*2.0f, w*2.0f);
}

void PathPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (m_visible)
    {
        QPen pen;
        pen.setWidth(4);
        pen.setColor(Qt::darkGray);
        painter->setPen(pen);
        painter->drawEllipse(boundingRect());
    }
}

QPainterPath PathPoint::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());

    return path;
}

int PathPoint::type() const
{
    return Type_PathPoint;
}

QVariant PathPoint::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        QPointF newPos = value.toPointF();

        if(m_parentPath)
        {
            newPos = m_parentPath->PointWithinRange(newPos);

            emit Moved(newPos);
            return newPos;
        }

        QRectF rect = scene()->sceneRect();
        if(!rect.contains(newPos))
        {
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }

        emit Moved(newPos);
    }

    return QGraphicsItem::itemChange(change, value);
}

void PathPoint::SetMovable(bool movable)
{
    m_movable = movable;
    setFlag(QGraphicsItem::ItemIsMovable, movable);
}

void PathPoint::SetVisible(bool visible)
{
    m_visible = visible;
}

IKPath* PathPoint::ParentPath()
{
    return m_parentPath;
}

QVector<QPointF> PathPoint::PointsFromPath(QVector<PathPoint*> ps)
{
    QVector<QPointF> points;

    for (PathPoint *point : ps)
    {
        points << point->pos();
    }

    return points;
}

} // paths
