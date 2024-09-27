#!/bin/bash
#
# +, -, *, /, %
#

a=10
b=3

let c=$a*$b
echo $c

d=$(( a * b ))
echo $d

echo `expr $a \* $b`

exit 0


#
# output
#

