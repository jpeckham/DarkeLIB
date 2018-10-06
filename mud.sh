#!/bin/bash
while :
do
	/usr/bin/driver /home/parnell/mud/mudlib/config.darke -d -fTEST1 -fTEST2
    echo "Press [CTRL+C] to stop.."
	sleep 5s
done
