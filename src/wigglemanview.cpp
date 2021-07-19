#include "wigglemanview.h"
#include "ui_wigglemanview.h"

#include "types.h"

WigglemanView::WigglemanView(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::WigglemanView)
{
    m_ui->setupUi(this);

    setWindowTitle(tr("Wiggleman v0.1a"));

    m_scene = new PathScene(m_ui->graphicsView);
    m_ui->graphicsView->setScene(m_scene);

    // setup body
    m_body = new Body();
    m_scene->addItem(m_body);
    m_body->CreateBody();

    SetupBodyControls();
    UpdateBodyControls();

    // save buttons
    SetupSettingsControls();

    // animation
    SetupAnimationControls();
    UpdateAnimationControls();
}

WigglemanView::~WigglemanView()
{
    delete m_ui;
}

void WigglemanView::SaveSpriteSheet()
{
    // get file location
    QString filename = QFileDialog::getSaveFileName(nullptr, "Save Sheet", "", "Image Files (*.png)");

    // resize the body to a better resolution
    m_body->SetScale(2.0f);
    // disable handles
    bool showHandles = m_body->ShowHandles();
    m_body->SetShowHandles(false);

    // run through animation once, getting the desired rect
    const int frames = m_ui->framesSpinBox->value();

    float side = 0;
    float top = 0;
    float bot = 0;
    for (int i = 0; i < frames; ++i)
    {
        m_body->UpdateAnimations(static_cast<float>(i) / static_cast<float>(frames));

        QRectF rect = m_scene->itemsBoundingRect();

        top = fmin(top, rect.top());
        bot = fmax(bot, rect.bottom());
        side = fmax(side, fmax(fabs(rect.left()), fabs(rect.right())));
    }

    // add some extra space for safety
    side = ceil(side+=10);
    top = floor(top-=10);
    bot = ceil(bot+=10);

    QRectF spriteRect = QRectF(-side, top, side + side, bot - top);
    QSize sheetSize = spriteRect.size().toSize();

    const float cols = ceil(sqrt(frames));
    const float rows = ceil(frames / cols);

    // ensure the sheet is a multiple of four

    sheetSize.setWidth(ceil(sheetSize.width() * cols / 4) * 4);
    sheetSize.setHeight(ceil(sheetSize.height() * rows / 4) * 4);

    QImage sheet(sheetSize, QImage::Format_ARGB32);
    sheet.fill(Qt::transparent);

    QPainter painter(&sheet);

    if (m_ui->aliasingCheckBox->isChecked())
    {
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
    }

    // loop a second time
    for (int j = 0; j < rows; ++j)
    {
        for (int i = 0; i < cols; ++i)
        {
            const int frame = j * cols + i;

            if (frame >= frames) break;

            m_body->UpdateAnimations(static_cast<float>(frame) / static_cast<float>(frames));

            QRectF targetRect = QRectF(0, 0, spriteRect.width(), spriteRect.height());
            targetRect.moveLeft(spriteRect.width() * i);
            targetRect.moveTop(spriteRect.height() * j);
            m_scene->RenderToPainter(&painter, spriteRect, targetRect);
        }
    }

    filename.replace(".png", QString::number(spriteRect.width()) + "_" + QString::number(spriteRect.height()) + ".png");

    sheet.save(filename);

    // restore properties
    m_body->SetScale(1.0f);
    m_body->SetShowHandles(showHandles);
}

void WigglemanView::SetupSettingsControls()
{
    connect(m_ui->saveButton, &QPushButton::clicked, m_scene, &PathScene::SaveImage);

    connect(m_ui->saveXmlButton, &QPushButton::clicked, m_body, &Body::SaveToXml);
    connect(m_ui->loadXmlButton, &QPushButton::clicked, this, [=]()
    {
        m_body->LoadFromXml();
        UpdateBodyControls();
    });

    connect(m_ui->saveAnimationButton, &QPushButton::clicked, this, [=]()
    {
        m_body->mainAnimation->SaveToXml();
    });
    connect(m_ui->loadAnimationButton, &QPushButton::clicked, this, [=]()
    {
        m_body->mainAnimation->LoadFromXml();
        UpdateAnimationControls();

        m_ui->graphicsView->scene()->update();
    });
}

void WigglemanView::UpdateAnimationControls()
{
    m_ui->transitionCheckBox->setChecked(m_body->TransitioningAnimation());

    // enabled
    m_ui->leftArmAnimationEnabledCheckbox->setChecked(m_body->mainAnimation->leftArmAnim->Enabled());
    m_ui->rightArmAnimationEnabledCheckbox->setChecked(m_body->mainAnimation->rightArmAnim->Enabled());
    m_ui->leftLegAnimationEnabledCheckbox->setChecked(m_body->mainAnimation->leftLegAnim->Enabled());
    m_ui->rightLegAnimationEnabledCheckbox->setChecked(m_body->mainAnimation->rightLegAnim->Enabled());

    // radius
    m_ui->leftArmRadiusSpinBox->setValue(m_body->mainAnimation->leftArmAnim->Radius());
    m_ui->rightArmRadiusSpinBox->setValue(m_body->mainAnimation->rightArmAnim->Radius());
    m_ui->leftLegRadiusSpinBox->setValue(m_body->mainAnimation->leftLegAnim->Radius());
    m_ui->rightLegRadiusSpinBox->setValue(m_body->mainAnimation->rightLegAnim->Radius());

    // direction
    m_ui->leftArmRotationSpinBox->setValue(m_body->mainAnimation->leftArmAnim->Direction());
    m_ui->rightArmRotationSpinBox->setValue(m_body->mainAnimation->rightArmAnim->Direction());
    m_ui->leftLegRotationSpinBox->setValue(m_body->mainAnimation->leftLegAnim->Direction());
    m_ui->rightLegRotationSpinBox->setValue(m_body->mainAnimation->rightLegAnim->Direction());

    // offset
    m_ui->leftArmOffsetAmountSpinBox->setValue(m_body->mainAnimation->leftArmAnim->OffsetAmount());
    m_ui->rightArmOffsetAmountSpinBox->setValue(m_body->mainAnimation->rightArmAnim->OffsetAmount());
    m_ui->leftLegOffsetAmountSpinBox->setValue(m_body->mainAnimation->leftLegAnim->OffsetAmount());
    m_ui->rightLegOffsetAmountSpinBox->setValue(m_body->mainAnimation->rightLegAnim->OffsetAmount());

    // offset direction
    m_ui->leftArmOffsetSpinBox->setValue(m_body->mainAnimation->leftArmAnim->OffsetDirection());
    m_ui->rightArmOffsetSpinBox->setValue(m_body->mainAnimation->rightArmAnim->OffsetDirection());
    m_ui->leftLegOffsetSpinBox->setValue(m_body->mainAnimation->leftLegAnim->OffsetDirection());
    m_ui->rightLegOffsetSpinBox->setValue(m_body->mainAnimation->rightLegAnim->OffsetDirection());

    // scale x
    m_ui->leftArmScaleXSpinBox->setValue(m_body->mainAnimation->leftArmAnim->ScaleX());
    m_ui->rightArmScaleXSpinBox->setValue(m_body->mainAnimation->rightArmAnim->ScaleX());
    m_ui->leftLegScaleXSpinBox->setValue(m_body->mainAnimation->leftLegAnim->ScaleX());
    m_ui->rightLegScaleXSpinBox->setValue(m_body->mainAnimation->rightLegAnim->ScaleX());

    // scale y
    m_ui->leftArmScaleYSpinBox->setValue(m_body->mainAnimation->leftArmAnim->ScaleY());
    m_ui->rightArmScaleYSpinBox->setValue(m_body->mainAnimation->rightArmAnim->ScaleY());
    m_ui->leftLegScaleYSpinBox->setValue(m_body->mainAnimation->leftLegAnim->ScaleY());
    m_ui->rightLegScaleYSpinBox->setValue(m_body->mainAnimation->rightLegAnim->ScaleY());
}

void WigglemanView::SetupAnimationControls()
{
    m_timer = new QTimeLine(1000);
    m_timer->setFrameRange(0, 12);
    m_timer->setLoopCount(0);
    m_timer->setEasingCurve(QEasingCurve::Linear);

    connect(m_timer, &QTimeLine::valueChanged, m_body, &Body::UpdateAnimations);

    connect(m_ui->saveSheetButton, &QPushButton::clicked, this, [=]()
    {
        m_ui->sidebar->setEnabled(false);
        SaveSpriteSheet();
        m_ui->sidebar->setEnabled(true);
    });

    connect(m_ui->playButton, &QPushButton::toggled, this, [=](bool checked)
    {
        if (!checked)
        {
            m_ui->playButton->setText("Play");
            m_ui->framesSpinBox->setEnabled(true);
            m_ui->goToSlider->setEnabled(true);
            m_timer->stop();
        }
        else
        {
            m_ui->playButton->setText("Pause");
            m_ui->framesSpinBox->setEnabled(false);
            m_ui->goToSlider->setEnabled(false);
            m_timer->start();
        }
    });

    connect(m_ui->framesSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [=](int value)
    {
        m_timer->setFrameRange(0, value);
        m_ui->goToSlider->setMaximum(value - 1);
    });

    connect(m_ui->timeSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [=](double value)
    {
        m_timer->setDuration(static_cast<int>(value * 1000.0f));
    });

    connect(m_ui->goToSlider, &QSlider::valueChanged, this, [=](const int &value)
    {
        m_body->UpdateAnimations(static_cast<float>(value) / static_cast<float>(m_ui->goToSlider->maximum()));
    });

    connect(m_ui->transitionCheckBox, &QCheckBox::toggled, this, [=] (bool toggled)
    {
        m_ui->targetAnimationButton->setVisible(toggled);

        m_body->SetTransitioningAnimation(toggled);
    });

    m_ui->targetAnimationButton->setVisible(false);

    connect(m_ui->targetAnimationButton, &QPushButton::clicked, this, [=]()
    {
        m_body->targetAnimation->LoadFromXml();
    });

    // animation controls

    //enabled
    connect(m_ui->leftArmAnimationEnabledCheckbox, &QCheckBox::toggled, this, [this](bool enabled)
    {
        m_body->mainAnimation->leftArmAnim->SetEnabled(enabled);
    });
    connect(m_ui->rightArmAnimationEnabledCheckbox, &QCheckBox::toggled, this, [this](bool enabled)
    {
        m_body->mainAnimation->rightArmAnim->SetEnabled(enabled);
    });
    connect(m_ui->leftLegAnimationEnabledCheckbox, &QCheckBox::toggled, this, [this](bool enabled)
    {
        m_body->mainAnimation->leftLegAnim->SetEnabled(enabled);
    });
    connect(m_ui->rightLegAnimationEnabledCheckbox, &QCheckBox::toggled, this, [this](bool enabled)
    {
        m_body->mainAnimation->rightLegAnim->SetEnabled(enabled);
    });

    //radius
    connect(m_ui->leftArmRadiusSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->leftArmAnim->SetRadius(value);
    });
    connect(m_ui->rightArmRadiusSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->rightArmAnim->SetRadius(value);
    });
    connect(m_ui->leftLegRadiusSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->leftLegAnim->SetRadius(value);
    });
    connect(m_ui->rightLegRadiusSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->rightLegAnim->SetRadius(value);
    });

    //direction
    connect(m_ui->leftArmRotationSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [this](int value)
    {
        m_body->mainAnimation->leftArmAnim->SetDirection(value);
    });
    connect(m_ui->rightArmRotationSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [this](int value)
    {
        m_body->mainAnimation->rightArmAnim->SetDirection(value);
    });
    connect(m_ui->leftLegRotationSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [this](int value)
    {
        m_body->mainAnimation->leftLegAnim->SetDirection(value);
    });
    connect(m_ui->rightLegRotationSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [this](int value)
    {
        m_body->mainAnimation->rightLegAnim->SetDirection(value);
    });

    //offset amount
    connect(m_ui->leftArmOffsetAmountSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->leftArmAnim->SetOffsetAmount(value);
    });
    connect(m_ui->rightArmOffsetAmountSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->rightArmAnim->SetOffsetAmount(value);
    });
    connect(m_ui->leftLegOffsetAmountSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->leftLegAnim->SetOffsetAmount(value);
    });
    connect(m_ui->rightLegOffsetAmountSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->rightLegAnim->SetOffsetAmount(value);
    });

    //offset direction
    connect(m_ui->leftArmOffsetSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [this](int value)
    {
        m_body->mainAnimation->leftArmAnim->SetOffsetDirection(value);
    });
    connect(m_ui->rightArmOffsetSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [this](int value)
    {
        m_body->mainAnimation->rightArmAnim->SetOffsetDirection(value);
    });
    connect(m_ui->leftLegOffsetSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [this](int value)
    {
        m_body->mainAnimation->leftLegAnim->SetOffsetDirection(value);
    });
    connect(m_ui->rightLegOffsetSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, [this](int value)
    {
        m_body->mainAnimation->rightLegAnim->SetOffsetDirection(value);
    });

    //Scale X
    connect(m_ui->leftArmScaleXSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->leftArmAnim->SetScaleX(value);
    });
    connect(m_ui->rightArmScaleXSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->rightArmAnim->SetScaleX(value);
    });
    connect(m_ui->leftLegScaleXSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->leftLegAnim->SetScaleX(value);
    });
    connect(m_ui->rightLegScaleXSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->rightLegAnim->SetScaleX(value);
    });

    //Scale Y
    connect(m_ui->leftArmScaleYSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->leftArmAnim->SetScaleY(value);
    });
    connect(m_ui->rightArmScaleYSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->rightArmAnim->SetScaleY(value);
    });
    connect(m_ui->leftLegScaleYSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->leftLegAnim->SetScaleY(value);
    });
    connect(m_ui->rightLegScaleYSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double value)
    {
        m_body->mainAnimation->rightLegAnim->SetScaleY(value);
    });
}

void WigglemanView::UpdateBodyControls()
{
    // body color
    m_ui->bodyColorLineEdit->setText(m_body->BodyColor().name(QColor::HexRgb));

    // trouser color
    m_ui->trouserColorLineEdit->setText(m_body->TrouserColor().name(QColor::HexRgb));

    // torso color
    m_ui->torsoColorLineEdit->setText(m_body->ShirtColor().name(QColor::HexRgb));

    // shoe color
    m_ui->shoesColorLineEdit->setText(m_body->ShoesColor().name(QColor::HexRgb));

    // length controls
    m_ui->sleeveSlider->setValue(m_body->SleeveLength());
    m_ui->trouserSlider->setValue(m_body->TrouserLength());
    m_ui->hipsSlider->setValue(m_body->HipsLength());

    // torso controls
    m_ui->widthSlider->setValue(m_body->Width());
    m_ui->lengthSlider->setValue(m_body->Height());

    // arm controls
    m_ui->armWidthSlider->setValue(m_body->ArmWidth());
    m_ui->armLengthSlider->setValue(m_body->ArmLength());

    // leg controls
    m_ui->legWidthSlider->setValue(m_body->LegWidth());
    m_ui->legLengthSlider->setValue(m_body->LegLength());

    // feet controls
    m_ui->feetWidthSlider->setValue(m_body->FeetWidth());
    m_ui->feetHeightSlider->setValue(m_body->FeetHeight());

    // jacket controls
    m_ui->jacketCheckBox->setChecked(m_body->UseJacket());
    m_ui->jacketLengthSlider->setValue(m_body->JacketLength());
    m_ui->jacketWidthSlider->setValue(m_body->JacketWidth());
    m_ui->gapSlider->setValue(m_body->JacketGap());
    m_ui->jacketColorLineEdit->setText(m_body->JacketColor().name(QColor::HexRgb));

    // handle controls
    m_ui->showHandlesCheckBox->setChecked(m_body->ShowHandles());
}

void WigglemanView::SetupBodyControls()
{
    m_ui->jacketOptions->setVisible(false);

    // Link spinboxes to sliders
    connect(m_ui->lengthSpinBox, SIGNAL(valueChanged(int)), m_ui->lengthSlider, SLOT(setValue(int)));
    connect(m_ui->widthSpinBox, SIGNAL(valueChanged(int)), m_ui->widthSlider, SLOT(setValue(int)));
    connect(m_ui->lengthSlider, &QSlider::valueChanged, m_ui->lengthSpinBox, &QSpinBox::setValue);
    connect(m_ui->widthSlider, &QSlider::valueChanged, m_ui->widthSpinBox, &QSpinBox::setValue);

    connect(m_ui->legLengthSpinBox, SIGNAL(valueChanged(int)), m_ui->legLengthSlider, SLOT(setValue(int)));
    connect(m_ui->legWidthSpinBox, SIGNAL(valueChanged(int)), m_ui->legWidthSlider, SLOT(setValue(int)));
    connect(m_ui->legLengthSlider, &QSlider::valueChanged, m_ui->legLengthSpinBox, &QSpinBox::setValue);
    connect(m_ui->legWidthSlider, &QSlider::valueChanged, m_ui->legWidthSpinBox, &QSpinBox::setValue);

    connect(m_ui->armLengthSpinBox, SIGNAL(valueChanged(int)), m_ui->armLengthSlider, SLOT(setValue(int)));
    connect(m_ui->armWidthSpinBox, SIGNAL(valueChanged(int)), m_ui->armWidthSlider, SLOT(setValue(int)));
    connect(m_ui->armLengthSlider, &QSlider::valueChanged, m_ui->armLengthSpinBox, &QSpinBox::setValue);
    connect(m_ui->armWidthSlider, &QSlider::valueChanged, m_ui->armWidthSpinBox, &QSpinBox::setValue);

    connect(m_ui->feetWidthSpinBox, SIGNAL(valueChanged(int)), m_ui->feetWidthSlider, SLOT(setValue(int)));
    connect(m_ui->feetHeightSpinBox, SIGNAL(valueChanged(int)), m_ui->feetHeightSlider, SLOT(setValue(int)));
    connect(m_ui->feetWidthSlider, &QSlider::valueChanged, m_ui->feetWidthSpinBox, &QSpinBox::setValue);
    connect(m_ui->feetHeightSlider, &QSlider::valueChanged, m_ui->feetHeightSpinBox, &QSpinBox::setValue);

    connect(m_ui->hipsSpinBox, SIGNAL(valueChanged(int)), m_ui->hipsSlider, SLOT(setValue(int)));
    connect(m_ui->hipsSlider, &QSlider::valueChanged, m_ui->hipsSpinBox, &QSpinBox::setValue);

    connect(m_ui->jacketLengthSpinBox, SIGNAL(valueChanged(int)), m_ui->jacketLengthSlider, SLOT(setValue(int)));
    connect(m_ui->jacketLengthSlider, &QSlider::valueChanged, m_ui->jacketLengthSpinBox, &QSpinBox::setValue);
    connect(m_ui->jacketWidthSpinBox, SIGNAL(valueChanged(int)), m_ui->jacketWidthSlider, SLOT(setValue(int)));
    connect(m_ui->jacketWidthSlider, &QSlider::valueChanged, m_ui->jacketWidthSpinBox, &QSpinBox::setValue);

    // body color
    connect(m_ui->bodyColorLineEdit, &QLineEdit::textChanged, this, [=](const QString &text)
    {
        QColor col(text);
        if (col.isValid())
        {
            m_body->SetBodyColor(col);
            m_ui->bodyColorLabel->setStyleSheet("QLabel { color : " + text + "; }");
        }
    });

    // trouser color
    connect(m_ui->trouserColorLineEdit, &QLineEdit::textChanged, this, [=](const QString &text)
    {
        QColor col(text);
        if (col.isValid())
        {
            m_body->SetTrouserColor(col);
            m_ui->trouserColorLabel->setStyleSheet("QLabel { color : " + text + "; }");
        }
    });

    // torso color
    connect(m_ui->torsoColorLineEdit, &QLineEdit::textChanged, this, [=](const QString &text)
    {
        QColor col(text);
        if (col.isValid())
        {
            m_body->SetShirtColor(col);
            m_ui->torsoColorLabel->setStyleSheet("QLabel { color : " + text + "; }");
        }
    });

    // shoe color
    connect(m_ui->shoesColorLineEdit, &QLineEdit::textChanged, this, [=](const QString &text)
    {
        QColor col(text);
        if (col.isValid())
        {
            m_body->SetShoesColor(col);
            m_ui->shoesColorLabel->setStyleSheet("QLabel { color : " + text + "; }");
        }
    });

    // jacket color
    connect(m_ui->jacketColorLineEdit, &QLineEdit::textChanged, this, [=](const QString &text)
    {
        QColor col(text);
        if (col.isValid())
        {
            m_body->SetJacketColor(col);
            m_ui->jacketColorLabel->setStyleSheet("QLabel { color : " + text + "; }");
        }
    });

    // length controls
    connect(m_ui->sleeveSlider, &QSlider::valueChanged, m_body, &Body::SetSleeveLength);
    connect(m_ui->trouserSlider, &QSlider::valueChanged, m_body, &Body::SetTrouserLength);
    connect(m_ui->hipsSlider, &QSlider::valueChanged, m_body, &Body::SetHipsLength);

    // torso controls
    connect(m_ui->widthSlider, &QSlider::valueChanged, m_body, &Body::SetWidth);
    connect(m_ui->lengthSlider, &QSlider::valueChanged, m_body, &Body::SetHeight);

    // arm controls
    connect(m_ui->armWidthSlider, &QSlider::valueChanged, m_body, &Body::SetArmWidth);
    connect(m_ui->armLengthSlider, &QSlider::valueChanged, m_body, &Body::SetArmLength);

    // leg controls
    connect(m_ui->legWidthSlider, &QSlider::valueChanged, m_body, &Body::SetLegWidth);
    connect(m_ui->legLengthSlider, &QSlider::valueChanged, m_body, &Body::SetLegLength);

    // feet controls
    connect(m_ui->feetWidthSlider, &QSlider::valueChanged, m_body, &Body::SetFeetWidth);
    connect(m_ui->feetHeightSlider, &QSlider::valueChanged, m_body, &Body::SetFeetHeight);

    // jacket controls
    connect(m_ui->jacketCheckBox, &QCheckBox::toggled, this, [this](bool checked)
    {
       m_ui->jacketOptions->setVisible(checked);
       m_body->SetUseJacket(checked);
    });
    connect(m_ui->jacketLengthSlider, &QSlider::valueChanged, m_body, &Body::SetJacketLength);
    connect(m_ui->jacketWidthSlider, &QSlider::valueChanged, m_body, &Body::SetJacketWidth);
    connect(m_ui->gapSlider, &QSlider::valueChanged, m_body, &Body::SetJacketGap);

    // handle controls
    connect(m_ui->showHandlesCheckBox, &QCheckBox::toggled, m_body, &Body::SetShowHandles);
}
