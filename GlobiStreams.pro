TEMPLATE = subdirs
SUBDIRS = "libs/QtSingleApplication/buildlib.pro" "libs/GlobiUtils/buildlib.pro" client server
client.depends = "libs/QtSingleApplication/buildlib.pro" "libs/GlobiUtils/buildlib.pro"