#ifndef PATHRENDERER_H
#define PATHRENDERER_H

#include <QtWidgets>

#include "pathpoint.h"
#include "ikpath.h"
#include "body.h"

namespace paths
{

class PathScene : public QGraphicsScene
{
    Q_OBJECT
public:

    explicit PathScene(QWidget *parent);

    void SaveImage();

    void RenderToPainter(QPainter *painter, QRectF &source, QRectF &target);

private:

    QPoint m_mousePress;
    bool m_mouseDrag;
};

} // paths

#endif // PATHRENDERER_H
