QMAKE_CXXFLAGS += -O3 -fomit-frame-pointer

TEMPLATE = subdirs
SUBDIRS = ./cliente \
        ./server



CONFIG += ordered
