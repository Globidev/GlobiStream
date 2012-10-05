TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += "../libs/QtSingleApplication/src" "../libs/GlobiUtils/src"
LIBS += -L"../libs/QtSingleApplication/lib" -L"../libs/GlobiUtils/lib" -lQSingleApplication -lGlobiUtils
QT += network xml

# Input
HEADERS += Event.h Poller.h ServerUi.h SingleAppManager.h
FORMS += ServerUi.ui
SOURCES += Event.cpp main.cpp Poller.cpp ServerUi.cpp
RESOURCES += globistreamserver.qrc
