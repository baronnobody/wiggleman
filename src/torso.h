#ifndef TORSO_H
#define TORSO_H

#include <QGraphicsItem>

class Torso : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Torso(QGraphicsItem* parent = nullptr);

    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    float height;
    float width;

    float gradLength = 0.5f;

    float shoulderCurvature;
    float hipCurvature;

    QColor topColor = Qt::red;
    QColor bottomColor = Qt::red;
};

class Jacket : public Torso
{
public:
    Jacket(QGraphicsItem* parent = nullptr);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QColor color = Qt::black;

    float gap;
    float jacketHeight;
    float jacketWidth;

    bool drawJacket = false;
    bool drawSleeves = false;
};

#endif // TORSO_H
