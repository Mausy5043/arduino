#! /bin/bash

#if [ "$1" = "master" ]; then
#  echo "Updating to master"
#  git fetch origin && git reset --hard origin/master && git clean -f -d
#fi
#if [ "$1" = "dev" ]; then
#  echo "Updating to dev"
#  git fetch origin && git reset --hard origin/dev && git clean -f -d
#fi

echo "Updating to"$1
git fetch origin && git reset --hard origin/$1 && git clean -f -d

# Set permissions
sudo chmod -R 744 *
