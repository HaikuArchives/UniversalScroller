#!/bin/sh
cd `dirname "$0"`
alert --info --modal  "You are ybout to uninstall
UniversalScroller 3.0

Do you really want to do so?" "Yes" "No"
_retval=$?
if [ "$_retval" -eq 0 ]
then
	rm `pwd`/readme.txt
	rm `pwd`/install.sh
	rm `pwd`/uninstall.sh
	rm `pwd`/Preferences
	rm `pwd`/UniversalScroller
	rm /boot/home/config/add-ons/input_server/filters/UniversalScroller
	rmdir `pwd`
	/system/servers/input_server -q
	alert "UniversalScroller 3.0 was removed.

if you want to get it again, download it at
http://come.to/Troublemaker"
else
	alert "UniversalScroller 3.0 was NOT removed"
fi
