#!/bin/bash

wget http://invisible-island.net/datafiles/release/ncurses.tar.gz

cd ncurses-5.9
./configure --enable-widec 
make && sudo make install
cd ..


