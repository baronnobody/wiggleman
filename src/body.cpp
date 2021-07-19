#include "body.h"

#include <QDomDocument>

// Version
// 0: initial version
// 1: added jackets
const int version = 1;

void SaveValue(QDomDocument &doc, QDomElement &root, const QString &name, const QString &value)
{
    QDomElement tag = doc.createElement(name);
    root.appendChild(tag);
    QDomText text = doc.createTextNode(value);
    tag.appendChild(text);
}

QString LoadValue(QDomElement &root, const QString &name)
{
    QDomNode tag = root.namedItem(name);

    return tag.firstChild().toText().data();
}

BodyAnimation::BodyAnimation()
{}

bool BodyAnimation::SaveToXml()
{
    QString filename = QFileDialog::getSaveFileName(nullptr, "Save WGLANIM", "", "WGLANIM Files (*.wglanim)");
    QFile file(filename);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        file.close();
        return false;
    }

    QDomDocument doc("AnimXML");

    QDomElement root = doc.createElement("Animations");
    doc.appendChild(root);

    SaveValue(doc, root, "Version", QString::number(version));

    // Left Arm
    QDomElement leftArm = doc.createElement("leftArm");
    root.appendChild(leftArm);
    SaveValue(doc, leftArm, "Enabled", QString::number(leftArmAnim->Enabled()));
    SaveValue(doc, leftArm, "Radius", QString::number(leftArmAnim->Radius()));
    SaveValue(doc, leftArm, "Direction", QString::number(leftArmAnim->Direction()));
    SaveValue(doc, leftArm, "Offset", QString::number(leftArmAnim->OffsetAmount()));
    SaveValue(doc, leftArm, "OffsetDirection", QString::number(leftArmAnim->OffsetDirection()));
    SaveValue(doc, leftArm, "ScaleX", QString::number(leftArmAnim->ScaleX()));
    SaveValue(doc, leftArm, "ScaleY", QString::number(leftArmAnim->ScaleY()));

    // Right Arm
    QDomElement rightArm = doc.createElement("rightArm");
    root.appendChild(rightArm);
    SaveValue(doc, rightArm, "Enabled", QString::number(rightArmAnim->Enabled()));
    SaveValue(doc, rightArm, "Radius", QString::number(rightArmAnim->Radius()));
    SaveValue(doc, rightArm, "Direction", QString::number(rightArmAnim->Direction()));
    SaveValue(doc, rightArm, "Offset", QString::number(rightArmAnim->OffsetAmount()));
    SaveValue(doc, rightArm, "OffsetDirection", QString::number(rightArmAnim->OffsetDirection()));
    SaveValue(doc, rightArm, "ScaleX", QString::number(rightArmAnim->ScaleX()));
    SaveValue(doc, rightArm, "ScaleY", QString::number(rightArmAnim->ScaleY()));

    // Left Leg
    QDomElement leftLeg = doc.createElement("leftLeg");
    root.appendChild(leftLeg);
    SaveValue(doc, leftLeg, "Enabled", QString::number(leftLegAnim->Enabled()));
    SaveValue(doc, leftLeg, "Radius", QString::number(leftLegAnim->Radius()));
    SaveValue(doc, leftLeg, "Direction", QString::number(leftLegAnim->Direction()));
    SaveValue(doc, leftLeg, "Offset", QString::number(leftLegAnim->OffsetAmount()));
    SaveValue(doc, leftLeg, "OffsetDirection", QString::number(leftLegAnim->OffsetDirection()));
    SaveValue(doc, leftLeg, "ScaleX", QString::number(leftLegAnim->ScaleX()));
    SaveValue(doc, leftLeg, "ScaleY", QString::number(leftLegAnim->ScaleY()));

    // Right Leg
    QDomElement rightLeg = doc.createElement("rightLeg");
    root.appendChild(rightLeg);
    SaveValue(doc, rightLeg, "Enabled", QString::number(rightLegAnim->Enabled()));
    SaveValue(doc, rightLeg, "Radius", QString::number(rightLegAnim->Radius()));
    SaveValue(doc, rightLeg, "Direction", QString::number(rightLegAnim->Direction()));
    SaveValue(doc, rightLeg, "Offset", QString::number(rightLegAnim->OffsetAmount()));
    SaveValue(doc, rightLeg, "OffsetDirection", QString::number(rightLegAnim->OffsetDirection()));
    SaveValue(doc, rightLeg, "ScaleX", QString::number(rightLegAnim->ScaleX()));
    SaveValue(doc, rightLeg, "ScaleY", QString::number(rightLegAnim->ScaleY()));

    // Save to file
    QTextStream stream(&file);
    stream << doc.toString();

    file.close();

    return true;
}

bool BodyAnimation::LoadFromXml()
{
    QString filename = QFileDialog::getOpenFileName(nullptr, "Save WGLANIM", "", "WGLANIM Files (*.wglanim)");
    QFile file(filename);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        file.close();
        return false;
    }

    QDomDocument doc;
    doc.setContent(&file);

    QDomElement anims = doc.documentElement();
    if (anims.nodeName() != "Animations")
    {
        file.close();
        return false;
    }

    // version
    int version = LoadValue(anims, "Version").toInt();

    if (version >= 0)
    {
        // Left arm
        QDomElement leftArm = anims.namedItem("leftArm").toElement();
        leftArmAnim->SetEnabled(LoadValue(leftArm, "Enabled").toInt());
        leftArmAnim->SetRadius(LoadValue(leftArm, "Radius").toFloat());
        leftArmAnim->SetDirection(LoadValue(leftArm, "Direction").toFloat());
        leftArmAnim->SetOffsetAmount(LoadValue(leftArm, "Offset").toFloat());
        leftArmAnim->SetOffsetDirection(LoadValue(leftArm, "OffsetDirection").toFloat());
        leftArmAnim->SetScaleX(LoadValue(leftArm, "ScaleX").toFloat());
        leftArmAnim->SetScaleY(LoadValue(leftArm, "ScaleY").toFloat());

        // Right arm
        QDomElement rightArm = anims.namedItem("rightArm").toElement();
        rightArmAnim->SetEnabled(LoadValue(rightArm, "Enabled").toInt());
        rightArmAnim->SetRadius(LoadValue(rightArm, "Radius").toFloat());
        rightArmAnim->SetDirection(LoadValue(rightArm, "Direction").toFloat());
        rightArmAnim->SetOffsetAmount(LoadValue(rightArm, "Offset").toFloat());
        rightArmAnim->SetOffsetDirection(LoadValue(rightArm, "OffsetDirection").toFloat());
        rightArmAnim->SetScaleX(LoadValue(rightArm, "ScaleX").toFloat());
        rightArmAnim->SetScaleY(LoadValue(rightArm, "ScaleY").toFloat());

        // Left leg
        QDomElement leftLeg = anims.namedItem("leftLeg").toElement();
        leftLegAnim->SetEnabled(LoadValue(leftLeg, "Enabled").toInt());
        leftLegAnim->SetRadius(LoadValue(leftLeg, "Radius").toFloat());
        leftLegAnim->SetDirection(LoadValue(leftLeg, "Direction").toFloat());
        leftLegAnim->SetOffsetAmount(LoadValue(leftLeg, "Offset").toFloat());
        leftLegAnim->SetOffsetDirection(LoadValue(leftLeg, "OffsetDirection").toFloat());
        leftLegAnim->SetScaleX(LoadValue(leftLeg, "ScaleX").toFloat());
        leftLegAnim->SetScaleY(LoadValue(leftLeg, "ScaleY").toFloat());

        // Right arm
        QDomElement rightLeg = anims.namedItem("rightLeg").toElement();
        rightLegAnim->SetEnabled(LoadValue(rightLeg, "Enabled").toInt());
        rightLegAnim->SetRadius(LoadValue(rightLeg, "Radius").toFloat());
        rightLegAnim->SetDirection(LoadValue(rightLeg, "Direction").toFloat());
        rightLegAnim->SetOffsetAmount(LoadValue(rightLeg, "Offset").toFloat());
        rightLegAnim->SetOffsetDirection(LoadValue(rightLeg, "OffsetDirection").toFloat());
        rightLegAnim->SetScaleX(LoadValue(rightLeg, "ScaleX").toFloat());
        rightLegAnim->SetScaleY(LoadValue(rightLeg, "ScaleY").toFloat());
    }

    valid = true;

    file.close();

    return true;
}

Body::Body(QGraphicsItem* parent)
    : QObject(), QGraphicsItem(parent)
{}

void Body::CreateBody()
{
    QPointF center(0.0f, 0.0f);

    m_torso = new Torso(this);

    m_jacket = new Jacket(this);

    m_leftArm = new IKPath(center, QVector2D(-0.5f, 1.0f), 50.0f, LeftArm, "Left Arm", this);
    m_rightArm = new IKPath(center, QVector2D(0.5f, 1.0f), 50.0f, RightArm, "Right Arm", this);
    m_leftLeg = new IKPath(center, QVector2D(0.0f, 1.0f), 50.0f, LeftLeg, "Left Leg", this);
    m_rightLeg = new IKPath(center, QVector2D(0.0f, 1.0f), 50.0f, RightLeg, "Right Leg", this);

    m_leftLeg->SetRoundEnd(false);
    m_rightLeg->SetRoundEnd(false);

    m_leftFoot = new Foot(40.0f, 20.0f, 20.0f, m_leftLeg->HandlePoint());
    m_rightFoot = new Foot(40.0f, 20.0f, 20.0f, m_rightLeg->HandlePoint());

    // set z orders
    m_rightArm->setZValue(5);
    m_jacket->setZValue(4);
    m_rightLeg->setZValue(3);
    m_torso->setZValue(2);
    m_leftLeg->setZValue(1);
    m_leftArm->setZValue(0);

    // position the limbs correctly
    UpdateLimbs();

    mainAnimation = new BodyAnimation();
    targetAnimation = new BodyAnimation();

    mainAnimation->valid = true;
    mainAnimation->leftLegAnim = new CircleAnimation(0.2f, QVector2D(0.0f, -1.0f), QVector2D(0, 1));
    mainAnimation->rightLegAnim = new CircleAnimation(0.2f, QVector2D(0.0f, -1.0f), QVector2D(0, 1));
    mainAnimation->leftArmAnim = new CircleAnimation(0.1f, QVector2D(-0.2f, -1.0f), QVector2D(-0.2f, 1));
    mainAnimation->rightArmAnim = new CircleAnimation(0.1f, QVector2D(-0.2f, -1.0f), QVector2D(0.2f, 1));
    mainAnimation->leftArmAnim->SetOffsetAmount(0.95f);
    mainAnimation->rightArmAnim->SetOffsetAmount(0.95f);

    m_leftLeg->SetAnimation(mainAnimation->leftLegAnim);
    m_rightLeg->SetAnimation(mainAnimation->rightLegAnim);
    m_rightLeg->SetAnimationPhase(0.5f);

    m_leftArm->SetAnimation(mainAnimation->leftArmAnim);
    m_rightArm->SetAnimation(mainAnimation->rightArmAnim);
    m_leftArm->SetAnimationPhase(0.5f);

    SetWidth(50);
    SetHeight(70);
    SetArmLength(50);
    SetArmWidth(20);
    SetLegLength(60);
    SetLegWidth(30);

    SetTrouserLength(2);
    SetSleeveLength(1);
    SetHipsLength(1 + static_cast<int>(100.0f * LegWidth() / Height()));

    SetJacketLength(50);
    SetJacketWidth(10);
    SetJacketGap(50);
}

void Body::UpdateAnimations(const float &time)
{
    if (!m_transitionAnimation || !targetAnimation->valid)
    {
        m_leftLeg->SetAnimationPos(time);
        m_rightLeg->SetAnimationPos(time);
        m_leftArm->SetAnimationPos(time);
        m_rightArm->SetAnimationPos(time);
    }
    else
    {
        // otherwise lerp between end of current animation and start of next
        m_leftLeg->SetTransitionPos(time, targetAnimation->leftLegAnim);
        m_rightLeg->SetTransitionPos(time, targetAnimation->rightLegAnim);
        m_leftArm->SetTransitionPos(time, targetAnimation->leftArmAnim);
        m_rightArm->SetTransitionPos(time, targetAnimation->rightArmAnim);
    }

    scene()->update();
}

// Not safe to be used outside of for renders.
void Body::SetScale(float scale)
{
    float oldScale = Scale();

    m_scale = scale;

    float multScale = scale / oldScale;

    SetWidth(Width() * multScale);
    SetHeight(Height() * multScale);
    SetArmLength(ArmLength() * multScale);
    SetArmWidth(ArmWidth() * multScale);
    SetLegLength(LegLength() * multScale);
    SetLegWidth(LegWidth() * multScale);
    SetJacketLength(JacketLength() * multScale);
    SetJacketWidth(JacketWidth() * multScale);
    SetFeetHeight(FeetHeight() * multScale);
    SetFeetWidth(FeetWidth() * multScale);
}

float Body::Scale() const
{
    return m_scale;
}

bool Body::SaveToXml()
{
    QString filename = QFileDialog::getSaveFileName(nullptr, "Save WGL", "", "WGL Files (*.wgl)");
    QFile file(filename);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        file.close();
        return false;
    }

    QDomDocument doc("BodyXML");

    QDomElement root = doc.createElement("Body");
    doc.appendChild(root);

    SaveValue(doc, root, "Version", QString::number(version));

    // Body sizes
    SaveValue(doc, root, "Width", QString::number(Width()));
    SaveValue(doc, root, "Height", QString::number(Height()));
    SaveValue(doc, root, "BodyColor", BodyColor().name(QColor::HexRgb));
    SaveValue(doc, root, "ShirtColor", ShirtColor().name(QColor::HexRgb));
    SaveValue(doc, root, "TrouserColor", TrouserColor().name(QColor::HexRgb));
    SaveValue(doc, root, "ShoesColor", ShoesColor().name(QColor::HexRgb));

    // Jacket
    SaveValue(doc, root, "UseJacket", QString::number(UseJacket()));
    QDomElement jacket = doc.createElement("Jacket");
    root.appendChild(jacket);
    SaveValue(doc, jacket, "Length", QString::number(JacketLength()));
    SaveValue(doc, jacket, "Width", QString::number(JacketWidth()));
    SaveValue(doc, jacket, "Gap", QString::number(JacketGap()));
    SaveValue(doc, jacket, "Color", JacketColor().name(QColor::HexRgb));

    // Arms
    QDomElement arms = doc.createElement("Arms");
    root.appendChild(arms);
    SaveValue(doc, arms, "Length", QString::number(ArmLength()));
    SaveValue(doc, arms, "Width", QString::number(ArmWidth()));
    SaveValue(doc, arms, "Sleeves", QString::number(SleeveLength()));

    // Legs
    QDomElement legs = doc.createElement("Legs");
    root.appendChild(legs);
    SaveValue(doc, legs, "Length", QString::number(LegLength()));
    SaveValue(doc, legs, "Width", QString::number(LegWidth()));
    SaveValue(doc, legs, "Trousers", QString::number(TrouserLength()));
    SaveValue(doc, legs, "Hips", QString::number(HipsLength()));

    // Feet
    QDomElement feet = doc.createElement("Feet");
    root.appendChild(feet);
    SaveValue(doc, feet, "Height", QString::number(FeetHeight()));
    SaveValue(doc, feet, "Width", QString::number(FeetWidth()));

    // Save to file
    QTextStream stream(&file);
    stream << doc.toString();

    file.close();

    return true;
}

bool Body::LoadFromXml()
{
    QString filename = QFileDialog::getOpenFileName(nullptr, "Load WGL", "", "WGL Files (*.wgl)");
    QFile file(filename);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        file.close();
        return false;
    }

    QDomDocument doc;
    doc.setContent(&file);

    QDomElement body = doc.documentElement();
    if (body.nodeName() != "Body")
    {
        file.close();
        return false;
    }

    // version
    int version = LoadValue(body, "Version").toInt();

    if (version >= 0)
    {
        // Body
        SetWidth(LoadValue(body, "Width").toFloat());
        SetHeight(LoadValue(body, "Height").toFloat());
        SetBodyColor(QColor(LoadValue(body, "BodyColor")));
        SetShirtColor(QColor(LoadValue(body, "ShirtColor")));
        SetTrouserColor(QColor(LoadValue(body, "TrouserColor")));
        SetShoesColor(QColor(LoadValue(body, "ShoesColor")));

        // Arms
        QDomElement arms = body.namedItem("Arms").toElement();
        SetArmLength(LoadValue(arms, "Length").toFloat());
        SetArmWidth(LoadValue(arms, "Width").toFloat());
        SetSleeveLength(LoadValue(arms, "Sleeves").toInt());

        // Legs
        QDomElement legs = body.namedItem("Legs").toElement();
        SetLegLength(LoadValue(legs, "Length").toFloat());
        SetLegWidth(LoadValue(legs, "Width").toFloat());
        SetTrouserLength(LoadValue(legs, "Trousers").toInt());
        SetHipsLength(LoadValue(legs, "Hips").toInt());

        // Feet
        QDomElement feet = body.namedItem("Feet").toElement();
        SetFeetHeight(LoadValue(feet, "Height").toFloat());
        SetFeetWidth(LoadValue(feet, "Width").toFloat());
    }

    if (version >= 1)
    {
        // Jacket
        SetUseJacket(LoadValue(body, "UseJacket").toInt());
        QDomElement jacket = body.namedItem("Jacket").toElement();
        SetJacketLength(LoadValue(jacket, "Length").toFloat());
        SetJacketWidth(LoadValue(jacket, "Width").toFloat());
        SetJacketGap(LoadValue(jacket, "Gap").toFloat());
        SetJacketColor(QColor(LoadValue(jacket, "Color")));
    }

    file.close();

    scene()->update();
    return true;
}

void Body::SetShowHandles(bool show)
{
    m_leftArm->HandlePoint()->SetVisible(show);
    m_rightArm->HandlePoint()->SetVisible(show);
    m_leftLeg->HandlePoint()->SetVisible(show);
    m_rightLeg->HandlePoint()->SetVisible(show);

    scene()->update();
}

bool Body::ShowHandles() const
{
    return m_leftArm->HandlePoint()->Visible();
}

void Body::SetUseJacket(bool enabled)
{
    m_jacket->drawJacket = enabled;

    if (enabled)
    {
        m_leftArm->SetClothColor(JacketColor());
        m_rightArm->SetClothColor(JacketColor());
    }
    else
    {
        m_leftArm->SetClothColor(ShirtColor());
        m_rightArm->SetClothColor(ShirtColor());
    }

    UpdateLimbs();

    scene()->update();
}

bool Body::UseJacket() const
{
    return m_jacket->drawJacket;
}

void Body::SetBodyColor(QColor color)
{
    m_bodyColor = color;

    // set the color for the limbs
    m_leftArm->SetColor(m_bodyColor);
    m_rightArm->SetColor(m_bodyColor);
    m_leftLeg->SetColor(m_bodyColor);
    m_rightLeg->SetColor(m_bodyColor);

    scene()->update();
}

QColor Body::BodyColor() const
{
    return m_bodyColor;
}

void Body::SetTrouserColor(QColor color)
{
    m_trouserColor = color;

    // set the color for the limbs
    m_torso->bottomColor = m_trouserColor;
    m_leftLeg->SetClothColor(m_trouserColor);
    m_rightLeg->SetClothColor(m_trouserColor);

    scene()->update();
}

QColor Body::TrouserColor() const
{
    return m_trouserColor;
}

void Body::SetShirtColor(QColor color)
{
    m_shirtColor = color;

    // set the color for the limbs
    m_torso->topColor = m_shirtColor;

    if (!UseJacket())
    {
        m_leftArm->SetClothColor(m_shirtColor);
        m_rightArm->SetClothColor(m_shirtColor);
    }

    scene()->update();
}

QColor Body::ShirtColor() const
{
    return m_shirtColor;
}

void Body::SetShoesColor(QColor color)
{
    m_shoesColor = color;

    // set the color for the peets
    m_leftFoot->color = m_shoesColor;
    m_rightFoot->color = m_shoesColor;

    scene()->update();
}

QColor Body::ShoesColor() const
{
    return m_shoesColor;
}

void Body::SetJacketColor(QColor color)
{
    m_jacketColor = color;

    // set the color for the jacket
    m_jacket->color = m_jacketColor;

    if (UseJacket())
    {
        m_leftArm->SetClothColor(JacketColor());
        m_rightArm->SetClothColor(JacketColor());
    }

    scene()->update();
}

QColor Body::JacketColor() const
{
    return m_jacketColor;
}

void Body::SetJacketLength(const int &length)
{
    m_jacket->jacketHeight = length;

    scene()->update();
}

int Body::JacketLength() const
{
    return m_jacket->jacketHeight;
}

void Body::SetJacketWidth(const int &width)
{
    m_jacket->jacketWidth = width;

    UpdateLimbs();

    scene()->update();
}

int Body::JacketWidth() const
{
    return m_jacket->jacketWidth;
}

void Body::SetJacketGap(const int &gap)
{
    m_jacketGap = gap;
    m_jacket->gap = m_jacketGap * 0.01f;

    scene()->update();
}

int Body::JacketGap() const
{
    return m_jacketGap;
}

void Body::SetSleeveLength(const int &length)
{
    m_sleeveLength = length;
    m_leftArm->SetGradLength(m_sleeveLength / 2.0f);
    m_rightArm->SetGradLength(m_sleeveLength / 2.0f);

    scene()->update();
}

int Body::SleeveLength() const
{
    return m_sleeveLength;
}

void Body::SetTrouserLength(const int &length)
{
    m_trouserLength = length;
    m_leftLeg->SetGradLength(m_trouserLength / 2.0f);
    m_rightLeg->SetGradLength(m_trouserLength / 2.0f);

    scene()->update();
}

int Body::TrouserLength() const
{
    return m_trouserLength;
}

void Body::SetHipsLength(const int &length)
{
    m_hipsLength = length;
    m_torso->gradLength = m_hipsLength / 100.0f;

    scene()->update();
}

int Body::HipsLength() const
{
    return m_hipsLength;
}

void Body::SetHeight(const float &height)
{
    m_torso->height = height;
    m_jacket->height = height;

    // update limbs
    UpdateLimbs();
    scene()->update();
}

float Body::Height() const
{
    return m_torso->height;
}

void Body::SetWidth(const float &width)
{
    m_torso->width = width;
    m_jacket->width = width;

    // update limbs
    UpdateLimbs();
    scene()->update();
}

float Body::Width() const
{
    return m_torso->width;
}

void Body::SetArmWidth(const float &width)
{
    m_leftArm->SetPenWidth(width);
    m_rightArm->SetPenWidth(width);

    // update limbs
    UpdateLimbs();

    // update torso
    m_torso->shoulderCurvature = 0.5f * width;
    m_jacket->shoulderCurvature = 0.5f * width;

    scene()->update();
}

float Body::ArmWidth() const
{
    return m_leftArm->PenWidth();
}

void Body::SetLegWidth(const float &width)
{
    m_leftLeg->SetPenWidth(width);
    m_rightLeg->SetPenWidth(width);

    m_leftFoot->legWidth = width;
    m_rightFoot->legWidth = width;

    // update limbs
    UpdateLimbs();

    // update torso
    m_torso->hipCurvature = 0.5f * width;

    scene()->update();
}

float Body::LegWidth() const
{
    return m_leftLeg->PenWidth();
}

void Body::SetArmLength(const float &length)
{
    m_leftArm->SetLength(length);
    m_rightArm->SetLength(length);

    scene()->update();
}

float Body::ArmLength() const
{
    return m_leftArm->Length();
}

void Body::SetLegLength(const float &length)
{
    m_leftLeg->SetLength(length);
    m_rightLeg->SetLength(length);

    scene()->update();
}

float Body::LegLength() const
{
    return m_leftLeg->Length();
}

void Body::SetFeetWidth(const float &width)
{
    m_leftFoot->width = width;
    m_rightFoot->width = width;

    scene()->update();
}

float Body::FeetWidth() const
{
    return m_leftFoot->width;
}

void Body::SetFeetHeight(const float &height)
{
    m_leftFoot->height = height;
    m_rightFoot->height = height;

    scene()->update();
}

float Body::FeetHeight() const
{
    return m_leftFoot->height;
}

void Body::UpdateLimbs()
{
    PositionLimb(m_leftArm);
    PositionLimb(m_rightArm);
    PositionLimb(m_leftLeg);
    PositionLimb(m_rightLeg);
}

void Body::PositionLimb(IKPath *limb)
{
    const float torsoWidth = m_torso->width * 0.5f;
    const float torsoHeight = m_torso->height;

    const float jacketOffset = (UseJacket()) ? JacketWidth() : 0.0f;

    QPointF pos;
    IKType type = limb->Type();
    const float penWidth = limb->PenWidth() * 0.5f;
    switch(type)
    {
    case LeftArm:
    {
        pos.setX(penWidth - torsoWidth - jacketOffset);
        pos.setY(penWidth - torsoHeight);
        break;
    }
    case RightArm:
    {
        pos.setX(jacketOffset + torsoWidth - penWidth);
        pos.setY(penWidth - torsoHeight);
        break;
    }
    case LeftLeg:
    {
        pos.setX(penWidth - torsoWidth);
        pos.setY(-penWidth);
        break;
    }
    case RightLeg:
    {
        pos.setX(torsoWidth - penWidth);
        pos.setY(-penWidth);
        break;
    }
    case None:
    {
        return;
    }
    }

    limb->setPos(pos);
}

QRectF Body::boundingRect() const
{
    return QRectF();
}

void Body::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
    return;
}

QPainterPath Body::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}
