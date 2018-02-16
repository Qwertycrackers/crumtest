#!/bin/bash
mkdir -p ~/.crumtest # create the crumtest folder, if it does not exist
gcc *.c -o crumtest # compile
cp crumtest ~/.crumtest # place crumtest in its home

echo "setenv PATH \$PATH:\$HOME/.crumtest" >> ~/.profile # make crumtest directory searchable in PATH
source ~/.profile

