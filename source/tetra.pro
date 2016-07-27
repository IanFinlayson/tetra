TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = lib cmd ide
cmd.depends = lib
ide.depends = lib

