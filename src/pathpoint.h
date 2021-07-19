#ifndef PATHPOINT_H
#define PATHPOINT_H

#include <QtWidgets>

#include "ikpath.h"

namespace paths
{

class IKPath;

class PathPoint : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit PathPoint(const QPointF &point, IKPath* parentPath, bool movable = true);

    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    virtual int type() const override;

    bool Movable() { return m_movable; }
    void SetMovable(bool movable);

    void SetVisible(bool visible);
    bool Visible() const { return m_visible; }

    static QVector<QPointF> PointsFromPath(QVector<PathPoint*> points);

    IKPath* ParentPath();

signals:
    void Moved(const QPointF &point);

private:
    bool m_movable;
    bool m_dragable;
    bool m_visible = true;

    IKPath* m_parentPath;
};

} // paths
#endif // PATHPOINT_H
