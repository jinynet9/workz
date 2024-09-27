#!/bin/bash

for_seq ()
{
    local i

    echo "for_seq()"

    for i in $( seq 1 2 20 ); do
	echo "Welcome $i times"
    done
}

#
# threee-expression
#
for_three_expression ()
{
    local i
    local max_loop=5

    echo "for_three_expression()"

    for (( i=0; i<$max_loop; i++ )); do
	echo "Welcome $i times"
    done

    # infinite loops
    for ((;;)); do
	echo "infinite loops [hit CTRL+C to stop]"
	sleep 1
    done
}

#
# characterized by counting
# The for loop executes a sequence of commands for each member in a list of items.
#
for_char ()
{
    local i

    echo "for_char()"

    for i in 1 2 3 4 5; do
	echo "Welcome $i times"
    done

    echo "Bash version ${BASH_VERSION}..."
    echo "Latest bash version 3.0+ has inbuilt support for setting up ranges:"
    for i in {1..5}; do
	echo "Welcome $i times"
    done

    echo "Bash 4.0+ has inbuilt support for setting up a step value using {START..END..INCREMENT} syntax:"
    for i in {0..5..2}; do
	echo "Welcome $i times"
    done
}

for_three_expression
exit 0
for_seq
exit 0
for_char
exit 0

exit 0


$ ./var.sh 

