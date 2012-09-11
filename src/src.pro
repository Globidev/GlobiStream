TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += "../libs/QtSingleApplication/src" "../libs/GlobiUtils/src"
LIBS += -L"../libs/QtSingleApplication/lib" -L"../libs/GlobiUtils/lib" -lQSingleApplication -lGlobiUtils
QT += network webkit xml

# Input
HEADERS += AddEventDialog.h \
           CalendarDialog.h \
           ChatBrowser.h \
           CommandProcess.h \
           CustomCalendar.h \
           Dialogs.h \
           Event.h \
           EventTable.h \
           Global.h \
           MainUi.h \
           ScheduleDialog.h \
           SingleAppManager.h \
           StreamActionWidget.h \
           StreamTable.h \
           SystemTray.h
FORMS += AddEventDialogUi.ui \
         CalendarDialogUi.ui \
         DoubleStringDialogUi.ui \
         GomTVDialogUi.ui \
         GStreamUi.ui \
         HostDialogUi.ui \
         PathDialogUi.ui \
         ScheduleDialogUi.ui
SOURCES += AddEventDialog.cpp \
           CalendarDialog.cpp \
           ChatBrowser.cpp \
           CommandProcess.cpp \
           CustomCalendar.cpp \
           Dialogs.cpp \
           Event.cpp \
           EventTable.cpp \
           Global.cpp \
           main.cpp \
           MainUi.cpp \
           ScheduleDialog.cpp \
           SingleAppManager.cpp \
           StreamActionWidget.cpp \
           StreamTable.cpp \
           SystemTray.cpp
RESOURCES += globistreams.qrc
