#/bin/bash

bison -d language.y
mv language.tab.c language.tab.cpp
flex -o language.yy.cpp language.l