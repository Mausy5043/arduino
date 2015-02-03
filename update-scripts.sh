#! /bin/bash

#git fetch origin && git reset --hard origin/master && git clean -f -d
git fetch origin && git reset --hard origin/dev && git clean -f -d

# Set permissions
sudo chmod -R 744 *

cd Vsens
ino clean && ino build && ino upload
