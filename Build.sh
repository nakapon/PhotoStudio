#!/bin/bash

# help
if test "$1" = "help"; then
	echo "(none): all"
	echo "lib:"
	echo "libclean:"
	echo "app:"
	echo "appclean:"
fi

if (test "$#" -eq 0) || (test "$1" = "lib") || (test "$1" = "clean") || (test "$1" = "libclean"); then
	MAKE_OPT=
	if (test "$1" = "clean") || (test "$1" = "libclean"); then
		MAKE_OPT=clean
	fi

	## Library
	MakeLibrary() {
		PORJECT_NAME=$1
		PROJECT_DIR=Project/$PORJECT_NAME
		echo make $PORJECT_NAME:

		BUILD_DIR=$SCRIPT_DIR/Build/Debug.x64/$PROJECT_DIR
		if [ ! -e $BUILD_DIR ]; then
			mkdir -p $BUILD_DIR
		fi
		cd $BUILD_DIR
		$QMAKE $SCRIPT_DIR/$PROJECT_DIR/$PORJECT_NAME.pro $QMAKE_CMD_DEBUG
		make $MAKE_OPT

		BUILD_DIR=$SCRIPT_DIR/Build/Release.x64/$PROJECT_DIR
		if [ ! -e $BUILD_DIR ]; then
			mkdir -p $BUILD_DIR
		fi
		cd $BUILD_DIR
		$QMAKE $SCRIPT_DIR/$PROJECT_DIR/$PORJECT_NAME.pro $QMAKE_CMD_RELEASE
		make $MAKE_OPT
	}

	MakeLibrary BaseLibraryQt

fi

if (test "$#" -eq 0) || (test "$1" = "app") || (test "$1" = "clean") || (test "$1" = "appclean"); then
	MAKE_OPT=
	if (test "$1" = "clean") || (test "$1" = "appclean"); then
		MAKE_OPT=clean
	fi

	## PhotoStudio
	PORJECT_NAME=PhotoStudioQt
	PROJECT_DIR=Project/$PORJECT_NAME
	echo make $PORJECT_NAME:

	BUILD_DIR=$SCRIPT_DIR/Build/Debug.x64/$PROJECT_DIR
	if [ ! -e $BUILD_DIR ]; then
		mkdir -p $BUILD_DIR
	fi
	cd $BUILD_DIR
	$QMAKE $SCRIPT_DIR/$PROJECT_DIR/$PORJECT_NAME.pro $QMAKE_CMD_DEBUG
	make $MAKE_OPT

	BUILD_DIR=$SCRIPT_DIR/Build/Release.x64/$PROJECT_DIR
	if [ ! -e $BUILD_DIR ]; then
		mkdir -p $BUILD_DIR
	fi
	cd $BUILD_DIR
	$QMAKE $SCRIPT_DIR/$PROJECT_DIR/$PORJECT_NAME.pro $QMAKE_CMD_RELEASE
	make $MAKE_OPT

fi

