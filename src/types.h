#ifndef TYPES_H
#define TYPES_H

#include <QGraphicsItem>

enum {
    Type_IKPath = QGraphicsItem::UserType + 1,
    Type_PathPoint
};

enum GradType {
    Full = 0,
    Half,
    None
};

#endif // TYPES_H
