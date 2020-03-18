@echo off

rem build.bat path
set BASE_PATH=%~dp0

rem Project path
set PROJECT_PATH=
set PORJECT_NAME=

rem VS path
set VCVARSALL_PATH="C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC"

rem !!! Need to change following paths according to your environment !!!
set QMAKE_x86=C:/Qt/Qt5.5.1/5.5/msvc2013/bin/qmake.exe
set JOM_x86=C:/Qt/Qt5.5.1/Tools/QtCreator/bin/jom.exe

set QMAKE_x64=C:/Qt/Qt5.5.1.x64/5.5/msvc2013_64/bin/qmake.exe
set JOM_x64=C:/Qt/Qt5.5.1.x64/Tools/QtCreator/bin/jom.exe

rem QMAKE args
set QMAKE_CMD_DEBUG=-r -spec win32-msvc2013 "CONFIG+=debug"
set QMAKE_CMD_RELEASE=-r -spec win32-msvc2013

rem 64bit (x64ÅjBuild

if "%IS_VS_PATH_SET%"=="" ( 
	echo set vcvarsall.bat
	set IS_VS_PATH_SET=OK

	cd /d %VCVARSALL_PATH%
	call vcvarsall.bat amd64

	cd /d %BASE_PATH%
)

REM -----------------------------------
REM Main
REM -----------------------------------
	if /i "%1"=="all"       goto _CMD_BUILD_ALL
	if /i "%1"=="clean"     goto _CMD_CLEAN_ALL
	if /i "%1"=="lib"       goto _CMD_BUILD_LIB
	if /i "%1"=="libclean"  goto _CMD_CLEAN_LIB
	if /i "%1"=="plg"       goto _CMD_BUILD_PLG
	if /i "%1"=="plgclean"  goto _CMD_CLEAN_PLG
	if /i "%1"=="app"       goto _CMD_BUILD_APP
	if /i "%1"=="appclean"  goto _CMD_CLEAN_APP
	goto _PRINT_USAGE

rem @cd

:_CMD_BUILD_ALL
	echo -----------------------------------------------------------------
	echo                          BUILD ALL START                         
	echo -----------------------------------------------------------------
	set MAKE_OPT=
	call :_BUILD_LIB
	call :_BUILD_PLG
	call :_BUILD_APP
	goto _END

:_CMD_CLEAN_ALL
	echo -----------------------------------------------------------------
	echo                          CLEAN ALL START                         
	echo -----------------------------------------------------------------
	set MAKE_OPT=clean
	call :_BUILD_LIB
	call :_BUILD_PLG
	call :_BUILD_APP
	goto _END

:_CMD_BUILD_LIB
	echo -----------------------------------------------------------------
	echo                          BUILD LIBRARY START                     
	echo -----------------------------------------------------------------
	set MAKE_OPT=
	call :_BUILD_LIB
	goto _END

:_CMD_CLEAN_LIB
	echo -----------------------------------------------------------------
	echo                          CLEAN LIBRARY START                     
	echo -----------------------------------------------------------------
	set MAKE_OPT=clean
	call :_BUILD_LIB
	goto _END

:_CMD_BUILD_PLG
	echo -----------------------------------------------------------------
	echo                          BUILD PLUGIN START                      
	echo -----------------------------------------------------------------
	set MAKE_OPT=
	call :_BUILD_PLG
	goto _END

:_CMD_CLEAN_PLG
	echo -----------------------------------------------------------------
	echo                          CLEAN PLUGIN START                      
	echo -----------------------------------------------------------------
	set MAKE_OPT=clean
	call :_BUILD_PLG
	goto _END

:_CMD_BUILD_APP
	echo -----------------------------------------------------------------
	echo                          BUILD APP START                         
	echo -----------------------------------------------------------------
	set MAKE_OPT=
	call :_BUILD_APP
	goto _END

:_CMD_CLEAN_APP
	echo -----------------------------------------------------------------
	echo                          CLEAN APP START                         
	echo -----------------------------------------------------------------
	set MAKE_OPT=clean
	call :_BUILD_APP
	goto _END

REM -----------------------------------
REM Sub
REM -----------------------------------
:_BUILD_LIB
	echo.
	echo Library ---------------------------------------------------------
	set PROJECT_PATH=Library\

REM	set PORJECT_NAME=MyLibrary
REM	call :_MAKE

	exit /b


:_BUILD_PLG
	echo.
	echo Image Reader Plugin ---------------------------------------------
REM	set PROJECT_PATH=Plugins\Image\Reader\

REM	set PORJECT_NAME=BasicReader
REM	call :_MAKE


	echo.
	echo Image Writer Plugin ---------------------------------------------
REM	set PROJECT_PATH=Plugins\Image\Writer\

REM	set PORJECT_NAME=BasicWriter
REM	call :_MAKE

	exit /b


:_BUILD_APP
	echo.
	echo Application PhotoStudioQt -------------------------------------------
	set PROJECT_PATH=

	set PORJECT_NAME=PhotoStudioQt
	call :_MAKE

	exit /b


REM -----------------------------------
REM Make
REM -----------------------------------
:_MAKE
	echo.
	echo %PORJECT_NAME%:
	REM x64 Debug
	set BUILD_PATH=%BASE_PATH%Build\Debug.x64\%PROJECT_PATH%%PORJECT_NAME%
	if not exist %BUILD_PATH% (
		mkdir %BUILD_PATH%
	)

	cd /d %BUILD_PATH%

	%QMAKE_x64% %BASE_PATH%%PROJECT_PATH%%PORJECT_NAME%/%PORJECT_NAME%.pro %QMAKE_CMD_DEBUG%
	%JOM_x64% %MAKE_OPT%

	REM x64 Release
	set BUILD_PATH=%BASE_PATH%Build\Release.x64\%PROJECT_PATH%%PORJECT_NAME%
	if not exist %BUILD_PATH% (
		mkdir %BUILD_PATH%
	)

	cd /d %BUILD_PATH%

	%QMAKE_x64% %BASE_PATH%%PROJECT_PATH%%PORJECT_NAME%/%PORJECT_NAME%.pro %QMAKE_CMD_RELEASE%
	%JOM_x64% %MAKE_OPT%

	exit /b


REM -----------------------------------
REM Print Usage
REM -----------------------------------
:_PRINT_USAGE
	echo Usage: build [options]
	echo   options:
	echo   -----------------------------------------------------------------
	echo     all                          build all
	echo     clean                        clean all
	echo     lib                          build library
	echo     libclean                     clean library
	echo     plg                          build plugin
	echo     plgclean                     clean plugin
	echo     app                          build application
	echo     appclean                     clean application
	goto _END


REM -----------------------------------
REM End
REM -----------------------------------
:_END
	rem restore current directory
	cd /d %BASE_PATH%
