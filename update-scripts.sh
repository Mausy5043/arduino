#! /bin/bash


echo "Updating to "$1
git fetch origin && git reset --hard origin/$1 && git clean -f -d

# Set permissions
sudo chmod -R 744 *
