#!/bin/bash
mkdir -p ~/.crumtest # create the crumtest folder, if it does not exist
mkdir -p ~/CS159 # create the CS159 folder, though it will almost certainly exist
gcc *.c -o crumtest # compile
cp crumtest ~/CS159 # place crumtest in its home
