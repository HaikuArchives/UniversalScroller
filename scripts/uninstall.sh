#!/bin/sh
cd `dirname "$0"`
alert --info --modal  "You are about to uninstall
{{NAME}}

Do you really want to proceed?" "Yes" "No"
_retval=$?
if [ "$_retval" -eq 0 ]
then
	rm /boot/home/config/add-ons/input_server/filters/UniversalScroller
	/system/servers/input_server -q
	alert "{{NAME}} was removed.

if you want to get it again, download it at
{{URL}}"
else
	alert "{{NAME}} was NOT removed"
fi
