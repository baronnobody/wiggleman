#ifndef ANIMATION_H
#define ANIMATION_H

#include <QtWidgets>
#include "ikpath.h"

namespace paths
{

class IKPath;

class Animation
{
public:
    Animation(const float &radius, QVector2D dir = QVector2D(), QVector2D offset = QVector2D());

    virtual QPointF PointAtTime(IKPath *path, const float &time) const = 0;

    void SetOffset(const QVector2D &offset) { m_offset = offset.normalized(); }
    QVector2D Offset() const { return m_offset; }

    void SetOffsetDirection(const float &angle);
    float OffsetDirection() const;

    void SetOffsetAmount(const float &amount) { m_offsetAmount = amount; }
    float OffsetAmount() const { return m_offsetAmount; }

    void SetRadius(const float &radius) { m_radius = radius; }
    float Radius() const { return m_radius; }

    void SetDirection(const float &angle);
    float Direction() const;

    void SetScaleX(const float &scale) { m_scaleX = scale; }
    float ScaleX() const { return m_scaleX; }
    void SetScaleY(const float &scale) { m_scaleY = scale; }
    float ScaleY() const { return m_scaleY; }

    void SetEnabled(bool enabled) { m_enabled = enabled; }
    bool Enabled() const { return m_enabled; }

protected:
    float m_radius;
    float m_offsetAmount = 1.0f;
    bool m_enabled = true;
    QVector2D m_dir;
    QVector2D m_offset;
    float m_scaleX = 1.0f;
    float m_scaleY = 1.0f;
};

class CircleAnimation : public Animation
{
public:
    CircleAnimation(const float &radius = 0.0f, QVector2D dir = QVector2D(), QVector2D offset = QVector2D());

    virtual QPointF PointAtTime(IKPath *path, const float &time) const override;
};

} // paths

#endif // ANIMATION_H
