#!/bin/sh
cd `dirname "$0"`
cp UniversalScroller /boot/home/config/add-ons/input_server/filters
/system/servers/input_server -q
alert --info --modal  "{{NAME}} V{{VERSION}} seems to be properly installed.

Shall I delete the no longer needed files from your install directory (UniversalScroller, install.sh)" "Yes" "No"
_retval=$?
[ $_retval -eq 0 ] && rm install.sh UniversalScroller 

alert --info --modal  "Do you want to read the readme.txt now?" "Yes" "No"
_retval=$?
[ $_retval -eq 0 ] && StyledEdit readme.txt 
