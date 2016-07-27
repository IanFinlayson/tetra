# tetra.pro
# this file just says the project consists of the three
# subdirectorys, and they should all be built, and ide
# and cmd both depend on the lib

TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = lib cmd ide
cmd.depends = lib
ide.depends = lib

