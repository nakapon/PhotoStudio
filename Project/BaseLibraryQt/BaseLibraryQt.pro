# QT
QT -= core gui

TEMPLATE = lib

CONFIG += staticlib
macx:CONFIG += c++11

# Target
TARGET = BaseLibraryQt

# Obj
CONFIG(debug, debug|release){
	win32:OBJECTS_DIR = $$PWD/../../Obj/Win32/Debug/BaseLibraryQt
	unix:!macx:OBJECTS_DIR = $$PWD/../../Obj/Linux/Debug/BaseLibraryQt
	macx:OBJECTS_DIR = $$PWD/../../Obj/Mac/Debug/BaseLibraryQt
} else {
	win32:OBJECTS_DIR = $$PWD/../../Obj/Win32/Release/BaseLibraryQt
	unix:!macx:OBJECTS_DIR = $$PWD/../../Obj/Linux/Release/BaseLibraryQt
	macx:OBJECTS_DIR = $$PWD/../../Obj/Mac/Release/BaseLibraryQt
}

# Define
win32:DEFINES += _WIN32 UNICODE _UNICODE
unix:!macx:DEFINES += _UNIX
macx:DEFINES += _MAC
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

# Destinaion
CONFIG(debug, debug|release){
	win32:DESTDIR = $$PWD/../../Common/Windows/Library.x64/Release
	unix:!macx:DESTDIR = $$PWD/../../Common/Linux/Library/Release
	macx:DESTDIR = $$PWD/../../Common/Mac/Library/Release
} else {
	win32:DESTDIR = $$PWD/../../Common/Windows/Library.x64/Debug
	unix:!macx:DESTDIR = $$PWD/../../Common/Linux/Library/Debug
	macx:DESTDIR = $$PWD/../../Common/Mac/Library/Debug
}

# Compiler Settings
unix:!macx:QMAKE_CXXFLAGS += -std=c++0x -fpermissive
macx:QMAKE_CXXFLAGS += -stdlib=libc++ -fpermissive

# Project Items
SOURCES += \
	../../Common/Generic/Source/Math/Vec.cpp \
	../../Common/Generic/Source/Math/Mat.cpp \
	../../Common/Generic/Source/ImageData.cpp \
	../../Common/Generic/Source/ImageProc.cpp \
	../../Common/Generic/Source/ImageReader.cpp \
	../../Common/Generic/Source/ImageWriter.cpp \
	../../Common/Generic/Source/StringConv.cpp \

HEADERS += \
	../../Common/Generic/Include/IImageData.h \
	../../Common/Generic/Include/ImageData.h \
	../../Common/Generic/Include/ImageProc.h \
	../../Common/Generic/Include/ImageReader.h \
	../../Common/Generic/Include/ImageWriter.h \
	../../Common/Generic/Include/StringConv.h \
