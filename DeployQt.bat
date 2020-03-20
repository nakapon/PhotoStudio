@echo off

set BASE_PATH=%~dp0

rem !!! Need to change following paths according to your environment !!!
set QT_PATH_x86=C:\Qt\Qt5.5.1\5.5\msvc2013\
set QT_PATH_x64=C:\Qt\Qt5.5.1.x64\5.5\msvc2013_64\

rem x86 with QML
rem %QT_PATH_x86%bin\windeployqt.exe --dir %BASE_PATH%Bin/Debug --debug --qmldir %QT_PATH_x86%qml
rem %QT_PATH_x86%bin\windeployqt.exe --dir %BASE_PATH%Bin/Release --release --qmldir %QT_PATH_x86%qml
rem x86
rem %QT_PATH_x86%bin\windeployqt.exe %BASE_PATH%Bin/Debug --debug
rem %QT_PATH_x86%bin\windeployqt.exe %BASE_PATH%Bin/Release --release

rem x64 with QML
rem %QT_PATH_x64%bin\windeployqt.exe --dir %BASE_PATH%Bin/Debug --debug --qmldir %QT_PATH_x64%qml
rem %QT_PATH_x64%bin\windeployqt.exe --dir %BASE_PATH%Bin/Release --release --qmldir %QT_PATH_x64%qml

rem x64
%QT_PATH_x64%bin\windeployqt.exe %BASE_PATH%Bin/Debug.x64 --debug
%QT_PATH_x64%bin\windeployqt.exe %BASE_PATH%Bin/Release.x64 --release
