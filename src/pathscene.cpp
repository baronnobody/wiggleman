#include "pathscene.h"
#include "types.h"

namespace paths
{

PathScene::PathScene(QWidget *parent)
    : QGraphicsScene(parent)
{
}

void PathScene::SaveImage()
{
    clearSelection();
    setSceneRect(itemsBoundingRect());

    QImage image(sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    render(&painter);

    image.save("testout.png");
}

void PathScene::RenderToPainter(QPainter *painter, QRectF &source, QRectF &target)
{
    render(painter, target, source);
}

} // paths
