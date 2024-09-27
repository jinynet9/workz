#!/bin/sh
#
# simple config file interaction
#
# conf file example
# VAR = value
#

if [ $# -lt 3 ]; then
    ME=`basename "$0"`
    echo "Usage: $ME FILE (get var|set var value)"
    exit 1
fi

FILE=$1
ACT=$2
VAR=$3
shift 3
VALUE="$@"

usage() {
    ME=`basename "$0"`
    echo "Usage: $ME FILE (get var|set var value)"
    echo "FILE=$FILE, ACT=$ACT, VAR=$VAR, VALUE=$VALUE"
    exit 1
}

if [ "x$VAR" == "x" ]; then
    usage
fi

#
#
#
do_get () {
    local name
    local value

    IFS="="

    while read -r name value
    do
	# strip
	name=`eval echo $name`

	if [ "x$VAR" == "x$name" ]; then
	    # strip
	    value=`eval echo $value`
	    echo $value
	    break
	fi
    done < $FILE

    return
}

#
#
#
do_delete () {
    local filetmp="$FILE.tmp"

    grep -v "$VAR *=" $FILE > $filetmp

    mv $filetmp $FILE

    return
}

#
#
#
do_set () {
    do_delete

#    if [ "x$VALUE" == "x" ]; then
#	return
#    fi

    echo "$VAR = $VALUE" >> $FILE

    return
}

#
#
#
main () {
    if [ $ACT == "get" ]; then
	do_get
    elif [ $ACT == "delete" ]; then
	do_delete
    elif [ $ACT == "set" ]; then
	do_set
    else
	usage
    fi
}

main "$@"

