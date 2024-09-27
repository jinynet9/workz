#!/bin/bash

JAIL="/nginx"
HINT=""

# Do three possibilities for $JAIL ##
for i in 1 2 3 
do
    case $i in
    	1) JAIL="/nginx/jail"; HINT="value set";;
    	2) JAIL=""; HINT="value set to empty string";;
    	3) unset JAIL; HINT="\$JAIL unset";;
    esac
     
    ###############################################
    # Update user with actual values and action  
    # $JAIL set to a non-empty string (1)
    # $JAIL set to the empty string  (2)
    # $JAIL can be unset (3)
    ################################################
    echo "*** Current value of \$JAIL is '$JAIL' ($HINT) ***"
     
    ## Determine if a bash variable is empty or not ##
    if [ -z "${JAIL}" ]; then
        echo "[ -z \"\${JAIL}\" ] JAIL is unset or set to the empty string"
    fi
    if [ -z "${JAIL+set}" ]; then
        echo "[ -z \"\${JAIL+set}\" ] JAIL is unset"
    fi
    if [ -z "${JAIL-unset}" ]; then
        echo "[ -z \"\${JAIL-unset}\" ] JAIL is set to the empty string"
    fi
    if [ -n "${JAIL}" ]; then
        echo "[ -n \"\${JAIL}\" ] JAIL is set to a non-empty string"
    fi
    if [ -n "${JAIL+set}" ]; then
        echo "[ -n \"\${JAIL+set}\" ] JAIL is set, possibly to the empty string"
    fi
    if [ -n "${JAIL-unset}" ]; then
        echo "[ -n \"\${JAIL-unset}\" ] JAIL is either unset or set to a non-empty string"
    fi

    echo ""
done

exit 0


$ ./str_empty.sh 
*** Current value of $JAIL is '/nginx/jail' (value set) ***
[ -n "${JAIL}" ] JAIL is set to a non-empty string
[ -n "${JAIL+set}" ] JAIL is set, possibly to the empty string
[ -n "${JAIL-unset}" ] JAIL is either unset or set to a non-empty string

*** Current value of $JAIL is '' (value set to empty string) ***
[ -z "${JAIL}" ] JAIL is unset or set to the empty string
[ -z "${JAIL-unset}" ] JAIL is set to the empty string
[ -n "${JAIL+set}" ] JAIL is set, possibly to the empty string

*** Current value of $JAIL is '' ($JAIL unset) ***
[ -z "${JAIL}" ] JAIL is unset or set to the empty string
[ -z "${JAIL+set}" ] JAIL is unset
[ -n "${JAIL-unset}" ] JAIL is either unset or set to a non-empty string

