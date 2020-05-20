#-------------------------------------------------
#
# Project created by QtCreator 2020-04-22T22:22:22
#
#-------------------------------------------------

TEMPLATE = subdirs

SUBDIRS = lib-yaml yavi-visual

yavi-visual.depends = lib-yaml

include(common.pri)
