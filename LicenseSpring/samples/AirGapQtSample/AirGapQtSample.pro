QT += core gui widgets

CONFIG += c++11
VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_BUILD = 0

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_BUILD=$$VERSION_BUILD"
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../AppConfig.cpp \
    activationwindow.cpp \
    deactivationdialog.cpp \
    devicedatadialog.cpp \
    licensewindow.cpp \
    main.cpp \
    mainwindow.cpp \
    productdetailsdialog.cpp

HEADERS += \
    ../AppConfig.h \
    activationwindow.h \
    deactivationdialog.h \
    devicedatadialog.h \
    licensewindow.h \
    mainwindow.h \
    productdetailsdialog.h

FORMS += \
    activationwindow.ui \
    deactivationdialog.ui \
    devicedatadialog.ui \
    licensewindow.ui \
    mainwindow.ui \
    productdetailsdialog.ui

RESOURCES += \
    res.qrc

INCLUDEPATH += $$PWD/../../include

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
    DESTDIR = ..\..\..\bin\x64\dynamic
    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../../../bin/x64/dynamic/ -lLicenseSpringD
        PRE_TARGETDEPS += $$PWD/../../../bin/x64/dynamic/LicenseSpringD.dll
    }
    CONFIG(release, debug|release) {
        LIBS += -L$$PWD/../../../bin/x64/dynamic/ -lLicenseSpring
        PRE_TARGETDEPS += $$PWD/../../../bin/x64/dynamic/LicenseSpring.dll
    }
} else {
    DESTDIR = ..\..\..\bin\x86\dynamic
    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../../../bin/x86/dynamic/ -lLicenseSpringD
        PRE_TARGETDEPS += $$PWD/../../../bin/x86/dynamic/LicenseSpringD.dll
    }
    CONFIG(release, debug|release) {
        LIBS += -L$$PWD/../../../bin/x86/dynamic/ -lLicenseSpring
        PRE_TARGETDEPS += $$PWD/../../../bin/x86/dynamic/LicenseSpring.dll
    }
}
}

win32 {
    DEPLOY_TARGET = $$shell_quote($$DESTDIR/$${TARGET}.exe)
    DEPLOY_COMMAND = windeployqt --no-compiler-runtime --no-translations --no-opengl-sw
    QMAKE_TARGET_COMPANY = LicenseSpring
    QMAKE_TARGET_PRODUCT = LS Airgap Licensing Sample
    QMAKE_TARGET_DESCRIPTION = LS Airgap Licensing Sample
    QMAKE_TARGET_COPYRIGHT = "LicenseSpring \\251 2021"
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
