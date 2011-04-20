#!/bin/sh
cd `dirname "$0"`
cp `pwd`/UniversalScroller /boot/home/config/add-ons/input_server/filters
/system/servers/input_server -q
alert --info --modal  "UniversalScroller 3.0 seems to be properly installed.

Shall I delete the unecessary files from your install directory (UniversalScroller, install.sh)" "Yes" "No"
_retval=$?
[ $_retval -eq 0 ] && rm `pwd`/install.sh `pwd`/UniversalScroller 

alert --info --modal  "Do you want to read the readme.txt now?" "Yes" "No"
_retval=$?
[ $_retval -eq 0 ] && StyledEdit  `pwd`/readme.txt 
