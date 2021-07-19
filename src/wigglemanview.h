#ifndef WIGGLEMANVIEW_H
#define WIGGLEMANVIEW_H

#include <QtWidgets>

#include "ikpath.h"
#include "body.h"
#include "pathscene.h"

namespace Ui {
class WigglemanView;
}

using namespace paths;

class WigglemanView : public QWidget
{
    Q_OBJECT

public:
    explicit WigglemanView(QWidget *parent = nullptr);
    ~WigglemanView();

    void ShowPathControls(IKPath* path);
    void ShowPointControls(PathPoint* point);

private:
    Ui::WigglemanView *m_ui;
    Body* m_body;
    PathScene* m_scene;
    QTimeLine *m_timer;

    void UpdateBodyControls();
    void SetupBodyControls();

    void UpdateAnimationControls();
    void SetupAnimationControls();

    void SetupSettingsControls();

    void SaveSpriteSheet();
};

#endif // WIGGLEMANVIEW_H
