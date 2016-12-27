QT += core widgets

TARGET = 277
TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG += warn_on
CONFIG += debug

INCLUDEPATH += include

include(src/src.pri)

FORMS += forms/mainwindow.ui \
    forms/cameracontrolshelp.ui

RESOURCES += glsl.qrc

*-clang*|*-g++* {
    message("Enabling additional warnings")
    CONFIG -= warn_on
    QMAKE_CXXFLAGS += -Wall -Wextra -pedantic -Winit-self
    QMAKE_CXXFLAGS += -Wno-strict-aliasing
    QMAKE_CXXFLAGS += -fno-omit-frame-pointer
}
linux-clang*|linux-g++*|macx-clang*|macx-g++* {
    message("Enabling stack protector")
    QMAKE_CXXFLAGS += -fstack-protector-all
}

# FOR LINUX & MAC USERS INTERESTED IN ADDITIONAL BUILD TOOLS
# ----------------------------------------------------------
# This conditional exists to enable Address Sanitizer (ASAN) during
# the automated build. ASAN is a compiled-in tool which checks for
# memory errors (like Valgrind). You may enable it for yourself;
# check the hidden `.build.sh` file for info. But be aware: ASAN may
# trigger a lot of false-positive leak warnings for the Qt libraries.
# (See `.run.sh` for how to disable leak checking.)
address_sanitizer {
    message("Enabling Address Sanitizer")
    QMAKE_CXXFLAGS += -fsanitize=address
    QMAKE_LFLAGS += -fsanitize=address
}
