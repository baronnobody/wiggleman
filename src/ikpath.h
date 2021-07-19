#ifndef IKPATH_H
#define IKPATH_H

#include "pathpoint.h"
#include "animation.h"

namespace paths
{

enum IKType
{
    None,
    LeftArm,
    RightArm,
    LeftLeg,
    RightLeg
};

class PathPoint;
class Animation;

class IKPath : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    IKPath(const QPointF &offset,
           const QVector2D &direction,
           const float &length,
           IKType type = None,
           QString name = "",
           QGraphicsItem *parent = nullptr);

    QVector<QPointF> Points() const;
    PathPoint* HandlePoint() const;

    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual int type() const override;

    // animation function
    void SetAnimationPos(float time);
    void SetAnimationPhase(float offset);

    void SetTransitionPos(float time, Animation *targetAnim);

    QPointF PointWithinRange(QPointF &point);
    bool IsPointWithinRange(QPointF &point);

    void SetInvertBend(bool invert);
    bool InvertBend();

    void SetLength(float length);
    float Length();

    void SetGradLength(float length);
    float GradLength();

    void SetPenWidth(int penWidth);
    int PenWidth();
    void SetSleeveWidth(int width);
    int SleeveWidth();

    void SetRoundStart(bool round) { m_roundStart = round; }
    bool RoundStart() { return m_roundStart; }

    void SetRoundEnd(bool round) { m_roundEnd = round; }
    bool RoundEnd() { return m_roundEnd; }

    void SetColor(QColor color) { m_color = color; }
    QColor Color() { return m_color; }

    void SetClothColor(QColor color) { m_clothColor = color; }
    QColor ClothColor() { return m_clothColor; }

    void SetName(QString name) { m_name = name; }
    QString Name() { return m_name; }

    void SetAnimation(Animation *anim) { m_anim = anim; }

    /*! Returns the IK type of a limb. */
    IKType Type() { return m_IKType; }

private:
    void CalculateIKPositions(const QPointF &pos);

    PathPoint* handle;
    QPointF startPoint;
    QPointF bendPoint;

    float m_penWidth = 20;
    float m_length;
    float m_gradLength = 0.5f;

    float m_animPhase = 0.0f;

    bool m_roundStart = false;
    bool m_roundEnd = false;
    bool m_bendInvert = false;
    IKType m_IKType;

    Animation* m_anim = nullptr;

    QString m_name;

    QColor m_color = Qt::red;
    QColor m_clothColor = Qt::gray;
};

} // paths

#endif // IKPATH_H
