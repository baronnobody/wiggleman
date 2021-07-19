#include "ikpath.h"
#include "types.h"

namespace paths
{

IKPath::IKPath(const QPointF &start,
               const QVector2D &direction,
               const float &length,
               IKType type,
               QString name,
               QGraphicsItem *parent)
    : QObject(),
      QGraphicsItem(parent),
      m_length(length),
      m_IKType(type),
      m_name(name)
{
    QPointF s = start;

    QPointF offset = (direction.normalized() * length).toPointF();

    startPoint = s;
    bendPoint = s + offset;
    handle = new PathPoint(s + offset * 2.0f, this);

    setPos(s);
    handle->setPos(s + offset * 2.0f);

    SetRoundEnd(true);
    SetRoundStart(true);

    connect(handle, &PathPoint::Moved, this, &IKPath::CalculateIKPositions);
}

int IKPath::type() const
{
    return Type_IKPath;
}

void IKPath::CalculateIKPositions(const QPointF &pos)
{
    // given start and end, calculate the position of the mid point
    QPointF endPt = pos;

    // extra check for point in range
    {
        QSignalBlocker blocker(handle);
        endPt = PointWithinRange(endPt);
        handle->setPos(endPt);
    }

    const QLineF startToEnd(startPoint, endPt);
    const float a = startToEnd.length() * 0.5f;

    float pathRot = startToEnd.angle();
    float bendRot;

    const float h = m_length;
    bendRot = std::acos(a / h) * 180.0f / M_PI;

    if (m_length * 2.0f < PenWidth()) bendRot = 0.0f;

    if (m_IKType == LeftArm || m_IKType == RightArm) bendRot *= -1;

    if (!m_bendInvert) bendRot *= -1;

    // set bend point
    QLineF startToBend = startToEnd;

    startToBend.setAngle(pathRot + bendRot);
    startToBend.setLength(m_length);

    bendPoint = startToBend.p2();

    // set handle rotation
    handle->setRotation(fmod(bendRot - pathRot - 90, 360));

    // update
    scene()->update();
}

QVector<QPointF> IKPath::Points() const
{
    QVector<QPointF> points;
    points << startPoint;
    points << bendPoint;

    if (handle)
        points << handle->pos();

    return points;
}

QRectF IKPath::boundingRect() const
{
    float left = 1000000, top = 1000000;
    float right = -1000000, bottom = -1000000;

    for (const QPointF &p : Points())
    {
        if (p.x() < left) left = p.x();
        if (p.x() > right) right = p.x();
        if (p.y() < top) top = p.y();
        if (p.y() > bottom) bottom = p.y();
    }

    float w = abs(m_penWidth + right - left);
    float h = abs(m_penWidth + bottom - top);

    return QRectF(left - (m_penWidth * 0.5f), top - (m_penWidth * 0.5f), w, h);
}

void IKPath::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QVector<QPointF> points = Points();
    QPainterPath ppath;
    ppath.moveTo(points.at(0));

    for (int i=1; i<points.size(); ++i)
        ppath.lineTo(points.at(i));

    // Draw the bounding box
    if (false)
    {
        QColor b = Qt::blue;
        painter->fillRect(boundingRect(), b);
    }

    QColor shirtColor = (m_gradLength > 0.0f) ? m_clothColor : m_color;
    QColor color = (m_gradLength >= 1.0f) ? m_clothColor : m_color;

    // Draw rounded ends first
    if (m_roundStart)
    {
        painter->setBrush(shirtColor);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(points.first(), m_penWidth / 2, m_penWidth / 2);
    }
    if (m_roundEnd)
    {
        painter->setBrush(m_color);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(points.last(), m_penWidth / 2, m_penWidth / 2);
    }

    // make a gradient brush
    QLinearGradient gradient;
    gradient.setColorAt(0, shirtColor);
    gradient.setColorAt(qMax(0.0f, m_gradLength - 0.001f), shirtColor);
    gradient.setColorAt(m_gradLength, color);
    gradient.setColorAt(1, color);
    gradient.setStart(points.first());
    gradient.setFinalStop(points.last());

    // Draw the path
    QPen pen(color, m_penWidth, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    pen.setBrush(QBrush(gradient));

    painter->strokePath(ppath, pen);
}

QPainterPath IKPath::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());

    return path;
}

void IKPath::SetAnimationPos(float time)
{
    if (!m_anim) return;

    // set the pos of the handle based on animation
    float offsetTime = (m_anim->Enabled()) ? fmod(time + m_animPhase, 1.0f) : 0.5f;

    QPointF pos = m_anim->PointAtTime(this, offsetTime);
    pos.setX(roundf(pos.x()));
    pos.setY(roundf(pos.y()));

    handle->setPos(pos);
}

void IKPath::SetTransitionPos(float time, Animation *targetAnim)
{
    if (!m_anim) return;

    // don't ask why this is 0.5, just something to do with how the maths works out
    const float startPhase = (m_anim->Enabled()) ? m_animPhase : 0.5f;
    const float endPhase = (targetAnim->Enabled()) ? m_animPhase : 0.5f;

    // set the pos of the handle as a lerp between the two
    const QPointF start = m_anim->PointAtTime(this, startPhase);
    const QPointF end = targetAnim->PointAtTime(this, endPhase);

    QPointF mix = start + ((end - start) * time);
    mix.setX(roundf(mix.x()));
    mix.setY(roundf(mix.y()));

    handle->setPos(mix);
}

void IKPath::SetAnimationPhase(float offset)
{
    m_animPhase = offset;
}

void IKPath::SetPenWidth(int penWidth)
{
    m_penWidth = penWidth;
    scene()->update();
}

int IKPath::PenWidth()
{
    return m_penWidth;
}

PathPoint* IKPath::HandlePoint() const
{
    return handle;
}

// return a point within range in the dir of point
QPointF IKPath::PointWithinRange(QPointF &point)
{
    const QLineF line(startPoint, point);

    if (IsPointWithinRange(point))
        return point;

    float dist = Length() * 2.0f;

    QPointF endPt = line.pointAt(dist / line.length());

    return endPt;
}

bool IKPath::IsPointWithinRange(QPointF &point)
{
    const QLineF line(startPoint, point);

    return line.length() < Length() * 2.0f;
}

void IKPath::SetInvertBend(bool invert)
{
    m_bendInvert = invert;
    CalculateIKPositions(handle->pos());
}

bool IKPath::InvertBend()
{
    return m_bendInvert;
}

void IKPath::SetLength(float length)
{
    m_length = length;

    CalculateIKPositions(handle->pos());
}

float IKPath::Length()
{
    return m_length;
}

void IKPath::SetGradLength(float length)
{
    m_gradLength = length;
}

float IKPath::GradLength()
{
    return m_gradLength;
}

} // paths
