QT += core gui widgets


CONFIG += c++17

# Uncomment to build with hardware key support
# CONFIG += hardwarekey

CONFIG(hardwarekey):  LS_HARDWARE_KEY = on
CONFIG(!hardwarekey): LS_HARDWARE_KEY = off

VERSION_MAJOR = 2
VERSION_MINOR = 0
VERSION_BUILD = 0


DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_BUILD=$$VERSION_BUILD"


#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../AppConfig.cpp \
    activationwindow.cpp \
    devicedatadialog.cpp \
    licensewindow.cpp \
    main.cpp \
    mainwindow.cpp \
    productdetailsdialog.cpp \
    reconfiguredialog.cpp

HEADERS += \
    ../AppConfig.h \
    activationwindow.h \
    devicedatadialog.h \
    licensewindow.h \
    mainwindow.h \
    productdetailsdialog.h \
    reconfiguredialog.h

FORMS += \
    activationwindow.ui \
    devicedatadialog.ui \
    licensewindow.ui \
    mainwindow.ui \
    productdetailsdialog.ui \
    reconfiguredialog.ui

RESOURCES += \
    res.qrc

INCLUDEPATH += $$PWD/../../include


macx {
    contains(LS_HARDWARE_KEY, on) {
        SHARED_DIR = shared_HardwareKey
    } else {
        SHARED_DIR = shared
    }

    CONFIG(debug, debug|release)
        DESTDIR = ../../../bin/$$SHARED_DIR/Debug
        LIBS += -L$$PWD/../../bin/$$SHARED_DIR/Debug/ -lLicenseSpring
        PRE_TARGETDEPS += $$PWD/../../bin/$$SHARED_DIR/Debug/libLicenseSpring.dylib
    CONFIG(release, debug|release):
        DESTDIR = ../../../bin/$$SHARED_DIR/Release
        LIBS += -L$$PWD/../../bin/$$SHARED_DIR/Release/ -lLicenseSpring
        PRE_TARGETDEPS += $$PWD/../../bin/$$SHARED_DIR/Release/libLicenseSpring.dylib
}

linux {
    contains(LS_HARDWARE_KEY, on) {
        SHARED_DIR = shared_HardwareKey
    } else {
        SHARED_DIR = shared
    }

    DESTDIR = ../../bin/$$SHARED_DIR/Release
    CONFIG(debug, debug|release):
        LIBS += -L$$PWD/../../bin/$$SHARED_DIR/Debug/ -lLicenseSpring
        PRE_TARGETDEPS += $$PWD/../../bin/$$SHARED_DIR/Debug/libLicenseSpring.so
    CONFIG(release, debug|release):
        LIBS += -L$$PWD/../../bin/$$SHARED_DIR/Release/ -lLicenseSpring
        PRE_TARGETDEPS += $$PWD/../../bin/$$SHARED_DIR/Release/libLicenseSpring.so
}

win32 {
    contains(LS_HARDWARE_KEY, on) {
        SHARED_DIR = dynamic_HardwareKey
    } else {
        SHARED_DIR = dynamic
    }

    contains(QMAKE_TARGET.arch, x86_64) {
        DESTDIR = ../../bin/x64/$$SHARED_DIR
        CONFIG(debug, debug|release) {
            LIBS += -L$$PWD/../../bin/x64/$$SHARED_DIR/ -lLicenseSpringD
            PRE_TARGETDEPS += $$PWD/../../bin/x64/$$SHARED_DIR/LicenseSpringD.dll
        }
        CONFIG(release, debug|release) {
            LIBS += -L$$PWD/../../bin/x64/$$SHARED_DIR/ -lLicenseSpring
            PRE_TARGETDEPS += $$PWD/../../bin/x64/$$SHARED_DIR/LicenseSpring.dll
        }
    } else {
        DESTDIR = ..\..\bin\x86\dynamic
        CONFIG(debug, debug|release) {
            LIBS += -L$$PWD/../../bin/x86/$$SHARED_DIR/ -lLicenseSpringD
            PRE_TARGETDEPS += $$PWD/../../bin/x86/$$SHARED_DIR/LicenseSpringD.dll
        }
        CONFIG(release, debug|release) {
            LIBS += -L$$PWD/../../bin/x86/$$SHARED_DIR/ -lLicenseSpring
            PRE_TARGETDEPS += $$PWD/../../bin/x86/$$SHARED_DIR/LicenseSpring.dll
        }
    }
}

win32 {
    DEPLOY_TARGET = $$shell_quote($$DESTDIR/$${TARGET}.exe)
    DEPLOY_COMMAND = windeployqt --no-compiler-runtime --no-translations --no-opengl-sw
    QMAKE_TARGET_COMPANY = LicenseSpring
    QMAKE_TARGET_PRODUCT = LS Qt Sample
    QMAKE_TARGET_DESCRIPTION = LicenseSpring Qt Sample
    QMAKE_TARGET_COPYRIGHT = "LicenseSpring \\251 2021-2025"
}

macx {
    DEPLOY_TARGET = $$shell_quote($$DESTDIR/$${TARGET}.app)
    DEPLOY_COMMAND = macdeployqt
}

win32|macx {
    # Generate release
    CONFIG(release, debug|release): QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET} -verbose=3
}

macx {
    contains(LS_HARDWARE_KEY, on) {
        SHARED_DIR = shared_HardwareKey
    } else {
        SHARED_DIR = shared
    }
    CONFIG(release, debug|release): {
        TARGET_DIR = $$DESTDIR/$${TARGET}.app/Contents/Frameworks
        QMAKE_POST_LINK += && cp $$PWD/../../bin/$$SHARED_DIR/Release/*.dylib $${TARGET_DIR}
    }
}
