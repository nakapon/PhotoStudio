#!/bin/bash

## !!! Need to change following paths according to your environment !!!
QMAKE=$HOME/Qt/5.13.0/gcc_64/bin/qmake

SCRIPT_DIR=$(cd $(dirname $0); pwd)

QMAKE_CMD_DEBUG="-r -spec linux-g++ CONFIG+=debug"
QMAKE_CMD_RELEASE="-r -spec linux-g++"

. ./Build.sh

