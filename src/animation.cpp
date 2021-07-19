#include "animation.h"

namespace paths
{

Animation::Animation(const float &radius, QVector2D dir, QVector2D offset)
    : m_radius(radius), m_dir(dir), m_offset(offset.normalized())
{
}

void Animation::SetOffsetDirection(const float &angle)
{
    QLineF line(QPointF(0, 0), QPointF(1, 0));

    line.setAngle(angle);
    line.setLength(1.0f);

    SetOffset(QVector2D(line.p2()));
}

float Animation::OffsetDirection() const
{
    QLineF line(QPointF(0, 0), Offset().toPointF());

    return line.angle();
}

void Animation::SetDirection(const float &angle)
{
    QLineF line(QPointF(0, 0), QPointF(1, 0));

    line.setAngle(angle);
    line.setLength(1.0f);

    m_dir = QVector2D(line.p2());
}

float Animation::Direction() const
{
    QLineF line(QPointF(0, 0), m_dir.toPointF());

    return line.angle();
}

CircleAnimation::CircleAnimation(const float &radius, QVector2D dir, QVector2D offset)
    : Animation(radius, dir, offset)
{}

QPointF CircleAnimation::PointAtTime(IKPath *path, const float &time) const
{
    const float angleAtTime = 360 * time;

    const float radius = m_radius * 2.0f * path->Length();

    QLineF lineAtTime(QPointF(), QPointF(radius, 0)); // we rotate around 0,0. Need to account for this in the offset.
    lineAtTime.setAngle(angleAtTime);

    QPointF scaledPoint = lineAtTime.p2();
    scaledPoint.setX(scaledPoint.x() * ScaleX());
    scaledPoint.setY(scaledPoint.y() * ScaleY());

    // rotate the point now so rotation has an effect
    QLineF rotatedLine(QPointF(), scaledPoint);
    rotatedLine.setAngle(rotatedLine.angle() + Direction());

    QPointF point = rotatedLine.p2();

    /*
    // set the pos of the handle to be on a circle according to time
    float angle = 360 * time;

    QPointF origin = m_radius * 2.0f * path->Length() * m_dir.normalized().toPointF();

    QLineF line(origin, QPointF());
    float startAngle = line.angle();

    line.setAngle(angle - startAngle);

    QPointF point = line.p2();
    point.setX(point.x() * m_transform.x());
    point.setY(point.y() * m_transform.y());*/

    return point + (m_offsetAmount * path->Length() * 2.0f * m_offset.toPointF());
}

} // paths
