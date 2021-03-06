# QT
QT += core gui widgets

TEMPLATE = app

macx:CONFIG += c++11

# Target
TARGET = PhotoStudioQt

# Obj
CONFIG(debug, debug|release){
	win32:OBJECTS_DIR = $$PWD/../../Obj/Win32/Debug/PhotoStudioQt
	win32:MOC_DIR = $$PWD/../../Obj/Win32/Debug/PhotoStudioQt
	win32:UI_DIR = $$PWD/../../Obj/Win32/Debug/PhotoStudioQt
	win32:RCC_DIR = $$PWD/../../Obj/Win32/Debug/PhotoStudioQt
	unix:!macx:OBJECTS_DIR = $$PWD/../../Obj/Linux/Debug/PhotoStudioQt
	unix:!macx:MOC_DIR = $$PWD/../../Obj/Linux/Debug/PhotoStudioQt
	unix:!macx:UI_DIR = $$PWD/../../Obj/Linux/Debug/PhotoStudioQt
	unix:!macx:RCC_DIR = $$PWD/../../Obj/Linux/Debug/PhotoStudioQt
	macx:OBJECTS_DIR = $$PWD/../../Obj/Mac/Debug/PhotoStudioQt
	macx:MOC_DIR = $$PWD/../../Obj/Mac/Debug/PhotoStudioQt
	macx:UI_DIR = $$PWD/../../Obj/Mac/Debug/PhotoStudioQt
	macx:RCC_DIR = $$PWD/../../Obj/Mac/Debug/PhotoStudioQt
} else {
	win32:OBJECTS_DIR = $$PWD/../../Obj/Win32/Release/PhotoStudioQt
	win32:MOC_DIR = $$PWD/../../Obj/Win32/Release/PhotoStudioQt
	win32:UI_DIR = $$PWD/../../Obj/Win32/Release/PhotoStudioQt
	win32:RCC_DIR = $$PWD/../../Obj/Win32/Release/PhotoStudioQt
	unix:!macx:OBJECTS_DIR = $$PWD/../../Obj/Linux/Release/PhotoStudioQt
	unix:!macx:MOC_DIR = $$PWD/../../Obj/Linux/Release/PhotoStudioQt
	unix:!macx:UI_DIR = $$PWD/../../Obj/Linux/Release/PhotoStudioQt
	unix:!macx:RCC_DIR = $$PWD/../../Obj/Linux/Release/PhotoStudioQt
	macx:OBJECTS_DIR = $$PWD/../../Obj/Mac/Release/PhotoStudioQt
	macx:MOC_DIR = $$PWD/../../Obj/Mac/Release/PhotoStudioQt
	macx:UI_DIR = $$PWD/../../Obj/Mac/Release/PhotoStudioQt
	macx:RCC_DIR = $$PWD/../../Obj/Mac/Release/PhotoStudioQt
}

# Define
win32:DEFINES += _WIN32 UNICODE _UNICODE
macx:DEFINES += TARGET_OS_OSX

# Include Path
INCLUDEPATH += $$PWD/../../Common/Generic/Include

win32:INCLUDEPATH += $$PWD/../../Common/Windows/Include
win32:INCLUDEPATH += $$PWD/../../ThirdParty/Windows/Include

!win32:INCLUDEPATH += $$PWD/../../Common/Unix/Include
unix:!macx:INCLUDEPATH += $$PWD/../../Common/Linux/Include
unix:!macx:INCLUDEPATH += $$PWD/../../ThirdParty/Linux/Include

macx:INCLUDEPATH += /usr/local/include
macx:INCLUDEPATH += $$PWD/../../Common/Mac/Include
macx:INCLUDEPATH += $$PWD/../../ThirdParty/Mac/Include

# Library Path
CONFIG(debug, debug|release){
	win32:LIBS += -L$$PWD/../../Common/Windows/Library.x64/Release
	unix:!macx:LIBS += -L$$PWD/../../Common/Linux/Library/Release
	macx:LIBS += -L$$PWD/../../Common/Mac/Library/Release
} else {
	win32:LIBS += -L$$PWD/../../Common/Windows/Library.x64/Debug
	unix:!macx:LIBS += -L$$PWD/../../Common/Linux/Library/Debug
	macx:LIBS += -L$$PWD/../../Common/Mac/Library/Debug
}

win32:LIBS += -L$$PWD/../../ThirdParty/Windows/Library.x64
unix:!macx:LIBS += -L$$PWD/../../ThirdParty/Linux/Library
macx:LIBS += -L$$PWD/../../ThirdParty/Mac/Library

macx:LIBS += -L/usr/local/lib

# Library Files
LIBS += -lBaseLibraryQt

win32:LIBS += -lgdi32
#!win32:LIBS += -ldl -ltiff -lz -lopencv_core -lopencv_imgproc

unix:!macx:LIBS += -lfreeimage
macx:LIBS += -lfreeimage

# Destination Path
CONFIG(debug, debug|release){
	win32:DESTDIR = $$PWD/../../Bin/Debug.x64
	unix:!macx:DESTDIR = $$PWD/../../Bin/Debug
	macx:DESTDIR = $$PWD/../../Bin/Debug
} else {
	win32:DESTDIR = $$PWD/../../Bin/Release.x64
	unix:!macx:DESTDIR = $$PWD/../../Bin/Release
	macx:DESTDIR = $$PWD/../../Bin/Release
}

# Compiler Settings
unix:!macx:QMAKE_CXXFLAGS += -std=c++0x -fpermissive
macx:QMAKE_CXXFLAGS += -stdlib=libc++ -fpermissive
!win32:QMAKE_CXXFLAGS += -mmmx -msse -msse2 -msse3 -mssse3

unix:!macx:QMAKE_CXXFLAGS += -fopenmp
unix:!macx:QMAKE_LFLAGS += -fopenmp

win32 {
	RC_FILE = resource.rc
}

# Project Items
SOURCES += \
	main.cpp \
	MainWindow.cpp \
	ImageLibraryQt.cpp \
	Session.cpp


HEADERS += \
	MainWindow.h \
	ImageLibraryQt.h \
	Session.h


FORMS += \
	MainWindow.ui \

#RESOURCES += \
#	PhotoStudioQt.qrc

DISTFILES += \
	resource.rc
