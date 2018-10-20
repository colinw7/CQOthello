TEMPLATE = app

QT += widgets

TARGET = CQOthello

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++14

#CONFIG += debug

# Input
SOURCES += \
CQOthello.cpp \
COthelloBoard.cpp \
COthelloPiece.cpp \
CQCanvas2D.cpp \

HEADERS += \
CQOthello.h \
COthello.h \
COthelloBoard.h \
COthelloPiece.h \
CQCanvas2D.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
../include \
../../CQUtil/include \
../../CImageLib/include \
../../CFont/include \
../../CFile/include \
../../CMath/include \
../../CStrUtil/include \
../../CUtil/include \
../../COS/include \
.

unix:LIBS += \
-L$$LIB_DIR \
-L../../CQUtil/lib \
-L../../CImageLib/lib \
-L../../CFont/lib \
-L../../CConfig/lib \
-L../../CFileUtil/lib \
-L../../CFile/lib \
-L../../CMath/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../CUtil/lib \
-L../../COS/lib \
-lCQUtil -lCImageLib -lCFont -lCConfig -lCFileUtil -lCFile -lCMath \
-lCStrUtil -lCUtil -lCRegExp -lCOS \
-lpng -ljpeg -ltre
