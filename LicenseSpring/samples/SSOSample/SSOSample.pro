QT += core widgets webenginewidgets

TEMPLATE = app
TARGET = LicenseSpringSSOSample

CONFIG += c++11
VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_BUILD = 0

DEFINES += \
    "VERSION_MAJOR=$$VERSION_MAJOR" \
    "VERSION_MINOR=$$VERSION_MINOR" \
    "VERSION_BUILD=$$VERSION_BUILD"

INCLUDEPATH += $$PWD/../../include

# Input
HEADERS += \
    ../AppConfig.h \
    MainWindow.h \
    ActivationWidget.h \
    BrowserWidget.h \
    LicenseInfoWidget.h

SOURCES += \
    ../AppConfig.cpp \
    main.cpp \
    MainWindow.cpp \
    ActivationWidget.cpp \
    BrowserWidget.cpp \
    LicenseInfoWidget.cpp

FORMS += \
    MainWindow.ui \
    ActivationWidget.ui \
    BrowserWidget.ui \
    LicenseInfoWidget.ui

RESOURCES += \
    Resources.qrc

macx {
    DESTDIR = ../../bin/shared/Release
    CONFIG(debug, debug|release)
        LIBS += -L$$PWD/../../bin/shared/Debug/ -lLicenseSpring
        PRE_TARGETDEPS += $$PWD/../../bin/shared/Debug/libLicenseSpring.dylib
    CONFIG(release, debug|release):
        LIBS += -L$$PWD/../../bin/shared/Release/ -lLicenseSpring
        PRE_TARGETDEPS += $$PWD/../../bin/shared/Release/libLicenseSpring.dylib
}

linux {
    DESTDIR = ../../bin/shared/Release
    CONFIG(debug, debug|release):
        LIBS += -L$$PWD/../../bin/shared/Debug/ -lLicenseSpring
        PRE_TARGETDEPS += $$PWD/../../bin/shared/Debug/libLicenseSpring.so
    CONFIG(release, debug|release):
        LIBS += -L$$PWD/../../bin/shared/Release/ -lLicenseSpring
        PRE_TARGETDEPS += $$PWD/../../bin/shared/Release/libLicenseSpring.so
}

win32 {

contains(QMAKE_TARGET.arch, x86_64) {
    DESTDIR = ..\..\bin\x64\dynamic
    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../../bin/x64/dynamic/ -lLicenseSpringD
        PRE_TARGETDEPS += $$PWD/../../bin/x64/dynamic/LicenseSpringD.dll
    }
    CONFIG(release, debug|release) {
        LIBS += -L$$PWD/../../bin/x64/dynamic/ -lLicenseSpring
        PRE_TARGETDEPS += $$PWD/../../bin/x64/dynamic/LicenseSpring.dll
    }
} else {
    DESTDIR = ..\..\bin\x86\dynamic
    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../../bin/x86/dynamic/ -lLicenseSpringD
        PRE_TARGETDEPS += $$PWD/../../bin/x86/dynamic/LicenseSpringD.dll
    }
    CONFIG(release, debug|release) {
        LIBS += -L$$PWD/../../bin/x86/dynamic/ -lLicenseSpring
        PRE_TARGETDEPS += $$PWD/../../bin/x86/dynamic/LicenseSpring.dll
    }
}
}

win32 {
    DEPLOY_TARGET = $$shell_quote($$DESTDIR/$${TARGET}.exe)
    DEPLOY_COMMAND = windeployqt --no-compiler-runtime --no-translations --no-opengl-sw
    QMAKE_TARGET_COMPANY = LicenseSpring
    QMAKE_TARGET_PRODUCT = LS SSO Sample
    QMAKE_TARGET_DESCRIPTION = LicenseSpring SSO Sample
    QMAKE_TARGET_COPYRIGHT = "LicenseSpring \\251 2022"
}

macx {
    DEPLOY_TARGET = $$shell_quote($$DESTDIR/$${TARGET}.app)
    DEPLOY_COMMAND = macdeployqt
    COPY_COMMAND = cp -a $$PWD/../../bin/shared/Release/*.dylib $$DESTDIR/$${TARGET}.app/Contents/Frameworks/ $$escape_expand(\\n)
}

win32|macx {
    # Generate release
    CONFIG(release, debug|release): QMAKE_POST_LINK = $${COPY_COMMAND} $${DEPLOY_COMMAND} $${DEPLOY_TARGET} -verbose=3
}
