###################################################################################################
#                                                                                                 #
#   OpenNN: Open Neural Networks Library                                                          #
#   www.artelnics.com/opennn                                                                      #
#                                                                                                 #
#   U R I N A R Y   I N F L A M M A T I O N S   D I A G N O S I S   P R O J E C T                 #
#                                                                                                 #
#   Roberto Lopez                                                                                 #
#   Artelnics - Making intelligent use of data                                                    #
#   robertolopez@artelnics.com                                                                    #
#                                                                                                 #
###################################################################################################

TEMPLATE = app
CONFIG += console
CONFIG += c++11

mac{
    CONFIG-=app_bundle
}

TARGET = urinary_inflammations_diagnosis

DESTDIR = "$$PWD/bin"

SOURCES = main.cpp

win32-g++{
QMAKE_LFLAGS += -static-libgcc
QMAKE_LFLAGS += -static-libstdc++
QMAKE_LFLAGS += -static
}

# OpenNN library

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../source/release/ -lopennn
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../source/debug/ -lopennn
#else:unix: LIBS += -L$$OUT_PWD/../../source/ -lopennn

#INCLUDEPATH += $$PWD/../../source
#DEPENDPATH += $$PWD/../../source

#win32-msvc:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../source/release/opennn.lib
#else:win32-msvc:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../source/debug/opennn.lib

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../source/release/libopennn.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../source/debug/libopennn.a

#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../source/libopennn.a


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../opennn/release/ -lopennn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../opennn/debug/ -lopennn
else:unix: LIBS += -L$$OUT_PWD/../../opennn/ -lopennn

INCLUDEPATH += $$PWD/../../opennn
DEPENDPATH += $$PWD/../../opennn

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../opennn/release/libopennn.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../opennn/debug/libopennn.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../opennn/release/opennn.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../opennn/debug/opennn.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../opennn/libopennn.a

# Tiny XML 2 library

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../tinyxml2/release/ -ltinyxml2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../tinyxml2/debug/ -ltinyxml2
else:unix: LIBS += -L$$OUT_PWD/../../tinyxml2/ -ltinyxml2

INCLUDEPATH += $$PWD/../../tinyxml2
DEPENDPATH += $$PWD/../../tinyxml2

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../tinyxml2/release/libtinyxml2.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../tinyxml2/debug/libtinyxml2.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../tinyxml2/release/tinyxml2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../tinyxml2/debug/tinyxml2.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../tinyxml2/libtinyxml2.a

# OpenMP library
!win32{
QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
}

mac{
INCLUDEPATH += /usr/local/Cellar/libiomp/20150701/include/libiomp
LIBS += -L/usr/local/Cellar/libiomp/20150701/lib -liomp5
}

# CUDA include
#include(../../cuda.pri)
