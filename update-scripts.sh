#! /bin/bash

if [ "$1" = "master" ]; then
  echo "Updating to master"
  git fetch origin && git reset --hard origin/master && git clean -f -d
fi
if [ "$1" = "dev" ]; then
  echo "Updating to dev"
  git fetch origin && git reset --hard origin/dev && git clean -f -d
fi

# Set permissions
sudo chmod -R 744 *
