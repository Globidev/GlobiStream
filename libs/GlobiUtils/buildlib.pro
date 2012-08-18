TEMPLATE=lib
CONFIG += qt dll
include(src/GlobiUtils.pri)

TARGET = $$GUTILS_LIBNAME
DESTDIR = $$GUTILS_LIBDIR

target.path = $$DESTDIR
INSTALLS += target