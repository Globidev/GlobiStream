TEMPLATE = subdirs
SUBDIRS = "libs/QtSingleApplication/buildlib.pro" "libs/GlobiUtils/buildlib.pro" src
src.depends = "libs/QtSingleApplication/buildlib.pro" "libs/GlobiUtils/buildlib.pro"