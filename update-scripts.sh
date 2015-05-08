#! /bin/bash

echo "Updating to "$1

git fetch origin
git checkout $1 && git reset --hard origin/$1 && git clean -f -d

# Set permissions
sudo chmod -R 744 *
