TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = image \
          app
app.depends = image
