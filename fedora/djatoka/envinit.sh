#!/bin/sh
CURRENTDIR=$PWD
LAUNCHDIR=/opt/adore-djatoka-1.1/bin
DJATOKA_HOME=/opt/adore-djatoka-1.1
LIBPATH=$DJATOKA_HOME/lib

if [ `uname` = 'Linux' ] ; then
  if [ `uname -p` = "x86_64" ] ; then
    # Assume Linux AMD 64 has 64-bit Java
    PLATFORM="Linux-x86-64"
    LD_LIBRARY_PATH="$LIBPATH/$PLATFORM"
    export LD_LIBRARY_PATH
    KAKADU_LIBRARY_PATH="-DLD_LIBRARY_PATH=$LIBPATH/$PLATFORM"
  else
    # 32-bit Java
    PLATFORM="Linux-x86-32"
    LD_LIBRARY_PATH="$LIBPATH/$PLATFORM"
    export LD_LIBRARY_PATH
    KAKADU_LIBRARY_PATH="-DLD_LIBRARY_PATH=$LIBPATH/$PLATFORM"
  fi
elif [ `uname` = 'Darwin' ] ; then
  # Mac OS X
  PLATFORM="Mac-x86"
  export PATH="/System/Library/Frameworks/JavaVM.framework/Versions/1.5/Home/bin:$PATH"
  export DYLD_LIBRARY_PATH="$LIBPATH/$PLATFORM"
  KAKADU_LIBRARY_PATH="-DDYLD_LIBRARY_PATH=$LIBPATH/$PLATFORM"
elif [ `uname` = 'SunOS' ] ; then
  if [ `uname -p` = "i386" ] ; then
    # Assume Solaris x86
    PLATFORM="Solaris-x86"
    LD_LIBRARY_PATH="$LIBPATH/$PLATFORM"
    export LD_LIBRARY_PATH
  else
    # Sparcv9
    PLATFORM="Solaris-Sparcv9"
    LD_LIBRARY_PATH="$LIBPATH/$PLATFORM"
    export LD_LIBRARY_PATH
  fi
else
  echo "djatoka env: Unsupported platform: `uname`"
  exit
fi

KAKADU_HOME=$DJATOKA_HOME/bin/$PLATFORM
export KAKADU_HOME
JAVA_OPTS="$JAVA_OPTS -Djava.awt.headless=true -Dkakadu.home=$KAKADU_HOME -Djava.library.path=$LIBPATH/$PLATFORM $KAKADU_LIBRARY_PATH"
