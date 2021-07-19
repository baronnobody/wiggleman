#ifndef FOOT_H
#define FOOT_H

#include <QGraphicsItem>

class Foot : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Foot(float w, float h, float lw, QGraphicsItem* parent = nullptr);

    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    float height;
    float width;

    float legWidth = 10.0f;

    QColor color = Qt::red;
};

#endif // FOOT_H
