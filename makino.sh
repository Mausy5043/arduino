#! /bin/bash

if [ -d "$1" ]; then
  pushd $1
  time (ino clean && ino build && ino upload)
  popd
else
  echo "Usage: makeino.sh <project>"
fi
