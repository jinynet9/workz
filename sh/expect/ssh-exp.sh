#!/usr/bin/expect
#
# ssh-exp.sh <user@remote> <password>
#
# ver 5
#

#log_file /var/log/expect.log

set REMOTE_HOST [lindex $argv 0]
set PW [lindex $argv 1]
set PROMPT "\[#$%>\]"

set timeout 5

spawn env LANG=C ssh ${REMOTE_HOST}
expect {
    -glob "(yes/no)?" {
        send "yes\n"
        exp_continue
    }
    -glob "password:" {
        send -- "${PW}\n"
    }
}

expect {
    -glob "${PROMPT}" {
        interact
        exit 0
    }

#    "\\\$" {
#        exit 0
#    }
}

exit 0

################################################################################
#
# ver 3
#

#!/usr/bin/expect


set HOST "172.17.0.21"
set ID "jinynet9"
set PW ""

set timeout 5

spawn env LANG=C ssh ${ID}@${HOST}
expect {
    "(yes/no)?" {
        send "yes\n"
        exp_continue
    }
    "password:" {
        send "${PW}\n"
    }
}

expect {
    "\\\$" {
        exit 0
    }
}

exit 0

################################################################################
#
# ver 2
#


#!/bin/sh

HOST=$1
ID=$2
PW=$3

expect -c "
set timeout 5
spawn env LANG=C ssh ${ID}@${HOST}
expect \"password:\"
send \"${PW}\n\"
expect \"$\"
exit 0
"

exit 0



################################################################################
#
# ver 1
#


#!/usr/bin/expect

usage () {
    echo "usage: <user@remote> <password>"
    exit 1
}

set argc [llength $argv]

if [ $argc -ne 2 ]; then
    usage
fi

set timeout -1
set USER_HOST [lindex $argv 0]
set PASSWD [lindex $argv 1]

spawn ssh $USER_HOST

expect {
    -re "No route" { exit 1 }
    -re "try again" { exit 1 }
    -re "yes/no" { send "yes\r"; exp_continue }
    -re "password:" { send "$PASSWD\r"; exp_continue }
}

interact


################################################################################
#
# ver 0
#

#!/usr/bin/expect 

set USER_HOST [lindex $argv 0]
set PASSWD [lindex $argv 1]

spawn ssh ${USER_HOST}

expect -re "yes/no" {
    send "yes\n"
    exp_continue
}  -re "password: " {
    send ${PASSWD}\n"
    exp_continue
}

interact
