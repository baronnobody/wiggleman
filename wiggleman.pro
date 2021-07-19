SOURCES += src/main.cpp \
    src/animation.cpp \
    src/body.cpp \
    src/foot.cpp \
    src/ikpath.cpp \
    src/pathpoint.cpp \
    src/pathscene.cpp \
    src/torso.cpp \
    src/wigglemanview.cpp
HEADERS += src/ikpath.h \
    src/animation.h \
    src/body.h \
    src/foot.h \
    src/pathpoint.h \
    src/pathscene.h \
    src/torso.h \
    src/types.h \
    src/wigglemanview.h


RESOURCES += wiggleman.qrc

QT += widgets xml

FORMS += \
    src/wigglemanview.ui

