#!/bin/bash

while_test () {
    local loop=0
    local rv

    while [ "$loop" -lt 20 ]; do

	echo "hello"

	loop=$( expr $loop + 1 )

	sleep 3
    done
}

while_test2 ()
{
    while $(sleep 10); do
	if systemctl is-system-running | grep -qE "running|degraded"; then
	    break
	fi
    done
}

while_test

exit 0


$ ./var.sh 

