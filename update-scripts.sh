#! /bin/bash

#git fetch origin && git reset --hard origin/master && git clean -f -d
git fetch origin && git reset --hard origin/dev && git clean -f -d

# Set permissions
sudo chmod -R 744 *
