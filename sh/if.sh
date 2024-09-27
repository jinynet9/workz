#! /bin/bash

if [ "a" == "$1" ]; then
    echo "1"
elif [ "b" = "$1" ]; then
    echo "2"
else
    echo "0"
fi

    [ $client_delta -lt 0 ] && client_delta=0

    [ "x$ONLINE" != "x1" ] && {
	enc_sys_mac
	enc_vendor
	enc_model
    }

    [ ! -e $FOTA_TEST_FILE ] && {
	enc_device_fw_ver
    } || {
	json_add_string "device_fw_ver" "0.0.0"
    }

