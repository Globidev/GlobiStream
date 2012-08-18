include(../common.pri)
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
QT *= network


HEADERS += globilibs_global.h \
           GlobiNetwork.h \
           GlobiUtils.h \
		   
SOURCES += GlobiNetwork.cpp GlobiUtils.cpp

win32 {
    DEFINES += GLOBILIBS_EXPORT
}