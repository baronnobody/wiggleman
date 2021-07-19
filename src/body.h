#ifndef BODY_H
#define BODY_H

#include <QGraphicsItem>

#include "ikpath.h"
#include "torso.h"
#include "foot.h"

using namespace paths;

class BodyAnimation
{
public:
    explicit BodyAnimation();

    bool SaveToXml();
    bool LoadFromXml();

    CircleAnimation *leftLegAnim = new CircleAnimation();
    CircleAnimation *leftArmAnim = new CircleAnimation();
    CircleAnimation *rightLegAnim = new CircleAnimation();
    CircleAnimation *rightArmAnim = new CircleAnimation();
    bool valid = false;
};

class Body : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Body(QGraphicsItem* parent = nullptr);

    void CreateBody();

    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void UpdateAnimations(const float &time);

    bool SaveToXml();
    bool LoadFromXml();
    bool SaveAnimationsToXml();
    bool LoadAnimationsFromXml();

    void SetHeight(const float &height);
    float Height() const;
    void SetWidth(const float &width);
    float Width() const;
    void SetArmWidth(const float &width);
    float ArmWidth() const;
    void SetLegWidth(const float &width);
    float LegWidth() const;
    void SetArmLength(const float &length);
    float ArmLength() const;
    void SetLegLength(const float &length);
    float LegLength() const;
    void SetFeetWidth(const float &width);
    float FeetWidth() const;
    void SetFeetHeight(const float &height);
    float FeetHeight() const;

    void SetBodyColor(QColor color);
    QColor BodyColor() const;

    void SetTrouserColor(QColor color);
    QColor TrouserColor() const;

    void SetShirtColor(QColor color);
    QColor ShirtColor() const;

    void SetShoesColor(QColor color);
    QColor ShoesColor() const;

    void SetJacketColor(QColor color);
    QColor JacketColor() const;

    void SetJacketLength(const int &length);
    int JacketLength() const;

    void SetJacketWidth(const int &width);
    int JacketWidth() const;

    void SetJacketGap(const int &gap);
    int JacketGap() const;

    void SetSleeveLength(const int &length);
    int SleeveLength() const;

    void SetTrouserLength(const int &length);
    int TrouserLength() const;

    void SetHipsLength(const int &length);
    int HipsLength() const;

    void SetShowHandles(bool show);
    bool ShowHandles() const;
    void SetUseJacket(bool jacket);
    bool UseJacket() const;

    void SetScale(float scale);
    float Scale() const;

    void SetTransitioningAnimation(bool transitioning) { m_transitionAnimation = transitioning; }
    bool TransitioningAnimation() { return m_transitionAnimation; }

    BodyAnimation *mainAnimation;
    BodyAnimation *targetAnimation;

private:
    void UpdateLimbs();
    void PositionLimb(IKPath* limb);

    QColor m_bodyColor = QColor(250, 250, 250);
    QColor m_trouserColor = QColor(100, 100, 100);
    QColor m_shirtColor = QColor(200, 200, 200);
    QColor m_shoesColor = QColor(50, 50, 50);
    QColor m_jacketColor = QColor(10, 10, 10);

    int m_sleeveLength = 1;
    int m_trouserLength = 2;
    int m_hipsLength = 20;
    int m_jacketGap = 50;

    float m_scale = 1.0f;

    bool m_transitionAnimation = false;

    IKPath* m_leftArm = nullptr;
    IKPath* m_rightArm;
    IKPath* m_leftLeg;
    IKPath* m_rightLeg;
    Torso* m_torso;
    Jacket* m_jacket;
    Foot* m_leftFoot;
    Foot* m_rightFoot;
};

#endif // BODY_H
