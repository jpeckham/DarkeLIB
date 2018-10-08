#!/bin/bash
while :
do
	#this cd makes the debug.log work.
	cd /home/parnell/mud/mudlib
	/usr/bin/driver /home/parnell/mud/mudlib/config.darke -d -fTEST1 -fTEST2
    echo "Press [CTRL+C] to stop.."
	sleep 5s
done
