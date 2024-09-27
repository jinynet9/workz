#!/bin/sh


if [ $# -lt 1 ]; then
    ME=`basename "$0"`
    echo "Usage: $ME FILE [NAME]"
    exit 1
fi

#. ./zut.sh
#. ./dut.sh
#
#. ./nms.env

. ./cjson.sh

#
#
#
dec_ack () {
    local result
    local reason

    json_get_var result "result"
    json_get_var reason "reason"
    logger "result: $result"
    logger "reason: $reason"

    [ ! -z $result ] && {
	tuci set $TUCI_NMS_RESULT=$result
    }

    [ ! -z "$reason" ] && {
	tuci set $TUCI_NMS_REASON="$reason"
    }
}

#
#
#
dec_register_ack () {
    local device_fw_ver device_fw_path
    local modem_fw_ver modem_fw_path
    local defer_range

    local device_ver=$( device_fw_ver )
    local modem_ver=$( modem_fw_ver )

    json_get_var device_fw_ver device_fw_ver
    json_get_var device_fw_path device_fw_path
    json_get_var modem_fw_ver modem_fw_ver
    json_get_var modem_fw_path modem_fw_path
    json_get_var defer_range defer_range

    logger "device_fw_ver : $device_fw_ver"
    logger "device_fw_path: $device_fw_path"
    logger "modem_fw_ver  : $modem_fw_ver"
    logger "modem_fw_path : $modem_fw_path"
    logger "defer_range   : $defer_range"
    logger "device_ver    : $device_ver"
    logger "modem_ver     : $modem_ver"

    [ "$device_fw_ver" = "$device_ver" ] &&
    [ "$modem_fw_ver" = "$modem_ver" ] && return
}

dec_set_msg_report_interval () {
    local val

    json_get_var val msg_report_interval
    [ $? -ne 0 ] && return

    uci set $UCI_NMS_MSG_REPORT_INTERVAL=$val

    NMS_COMMIT="true"
}

dec_set_msg_timeout () {
    local val

    json_get_var val msg_timeout
    [ $? -ne 0 ] && return

    uci set $UCI_NMS_MSG_TIMEOUT=$val

    NMS_COMMIT="true"
}

dec_set_file_timeout () {
    local val

    json_get_var val file_timeout
    [ $? -ne 0 ] && return

    uci set $UCI_NMS_FILE_TIMEOUT=$val

    NMS_COMMIT="true"
}

dec_set_factory_reset () {
    local val

    json_get_var val factory_reset
    [ $? -ne 0 ] && return

    [ "x$val" = "x1" ] && RX_MESSAGE=$MSG_FACTORY_RESET
}

dec_set_remote_management () {
    local val

    json_get_var val remote_management
    [ $? -ne 0 ] && return

    echo "remote_management $val"
}

dec_set_alm_cpu_threshold () {
    local val

    json_get_var val cpu_threshold
    [ $? -ne 0 ] && return

    if [ $val -lt 1 ] || [ $val -gt 100 ]; then
	RESULT="CPU: Invalid value"
	return
    fi

    uci set $UCI_NMS_ALM_CPU_THR=$val

    NMS_COMMIT="true"
}

dec_set_alm_memory_threshold () {
    local val

    json_get_var val memory_threshold
    [ $? -ne 0 ] && return

    if [ $val -lt 1 ] || [ $val -gt 100 ]; then
	RESULT="MEMORY: Invalid value"
	return
    fi

    uci set $UCI_NMS_ALM_MEM_THR=$val

    NMS_COMMIT="true"
}

dec_set_firewall_entry () {
    local entry=$1
    local dst_ip src_ip
    local report_interval proto
    local dst_port src_port

    json_select $entry > /dev/null
    [ $? -ne 0 ] && return 1

    json_get_var dst_ip dst_ip
    json_get_var src_ip src_ip
    json_get_var proto proto
    json_get_var dst_port dst_port
    json_get_var src_port src_port
    echo "dst_ip  : $dst_ip"
    echo "src_ip  : $src_ip"
    echo "proto   : $proto"
    echo "dst_port: $dst_port"
    echo "src_port: $src_port"

    json_select ..

    return 0
}

dec_set_firewall () {
    local i=1
    local rv

    json_select firewall > /dev/null
    [ $? -ne 0 ] && return

    while [ true ]; do
        dec_set_firewall_entry "entry$i"
	[ $? -ne 0 ] && break

	i=$(( i + 1 ))
    done

    json_select ..
}

apn_set () {
    sleep 5

    modem-apn-set.sh $1 $2 $3
}

dec_set_apn () {
    local val
    local apn_index apn_name pdp_type

    json_select "APN" > /dev/null
    [ $? -ne 0 ] && return

    json_get_var apn_index "apn_index"
    json_get_var apn_name "apn_name"
    json_get_var pdp_type "pdp_type"

    json_select ..

    [ -z $apn_index ] && return

    apn_set $apn_index $apn_name $pdp_type &
}


#
#
#
#dec_reboot () {
#    local val
#
#    json_get_var val reboot
#    [ $? -ne 0 ] && return
#
#    [ "x$val" != "x1" ] && return
#
#    json_select ..
#
#    RX_MESSAGE=$MSG_REBOOT
#}

#
#
#
dec_cli () {
    local loop
    local key cli

    loop=1
    while [ true ]; do
	key=$(printf 'cli%d' $loop)
	json_get_var cli $key

	[ $? -ne 0 ] && break

	echo "$key: $cli"

	loop=$(expr $loop + 1)
    done

    json_select ..
}

#
#
#
dec_config () {
    local val

    dec_set_factory_reset
    [ ! -z $RX_MESSAGE ] && return

    dec_set_alm_cpu_threshold
    dec_set_alm_memory_threshold
    dec_set_msg_report_interval
    dec_set_msg_timeout
    dec_set_file_timeout
    dec_set_apn

    # cli message
    json_select "cli" > /dev/null
    [ $? -eq 0 ] && dec_cli

    [ ! -z "$NMS_COMMIT" ] && {
	uci commit nms
    }

    RX_MESSAGE=$MSG_CONFIG

    json_select ..

#    dec_set_remote_management
#    dec_set_firewall
}

#
#
#
dec_status () {
    local result
    local interval
    local val

    json_get_var result result
    json_get_var interval report_interval

    RX_RESULT=$result

    [ "x$result" != "xsuccess" ] && {
	logger "NMS ERROR RECEIVED: $result"
	return
    }

    # reboot message
    json_select "reboot" > /dev/null
    [ $? -eq 0 ] && {
        json_get_var val reboot
        [ "x$val" == "x1" ] && {
	    RX_MESSAGE=$MSG_REBOOT
	}

	return
    }

    # config message
    json_get_var val "config_profile"
    json_select "config" > /dev/null
    [ $? -eq 0 ] && {
	[ -z $val ] && {
	    tuci set $TUCI_NMS_CONFIG_PROFILE="EMPTY"
	    RESULT="Config profile is empty"
	    return
	}

	tuci set $TUCI_NMS_CONFIG_PROFILE=$val

	dec_config
	return
    }

    # report ack message
    RX_MESSAGE=$MSG_REPORT_ACK
    tuci set $TUCI_NMS_RX_INTERVAL=$interval
}

#
#
#
dec_firmware () {
    local result model version size checksum url
    local val

    json_get_var result result

    RX_RESULT=$result

    [ "x$result" != "xupdatable" ] && return

    json_select "info" > /dev/null
    [ $? -ne 0 ] && return

    json_get_var model model
    json_get_var version version
    json_get_var size size
    json_get_var checksum checksum
    json_get_var url url

    json_select ..

    echo "*** FW INFO ***"
    echo "model $model"
    echo "version $version"
    echo "size $size"
    echo "checksum $checksum"
    echo "url $url"
    echo ""

    RX_MESSAGE=$MSG_FOTA

    # model
    val=$(sys_model)
    [ "x$val" != "x$model" ] && {
	RESULT="Model mismatch"
	return
    }

    version_compare $version $(device_fw_ver)
    [ $? -ne 2 ] && {
	RESULT="No higher version"
	return
    }

    NMS="/mnt/nike/control-project/SMSG-100W/qsdk/build_dir/target-arm_cortex-a7_musl-1.1.16_eabi/nms-test/nms"
    FWFILE="/tmp/newfw.enc"

    # download
    $NMS -d $url $FWFILE
    [ $? -ne 0 ] && {
	RESULT="Fail to download"
	return
    }

    # checksum
    val=$(sha256sum $FWFILE | awk '{ print $1 }')
    [ "x$val" != "x$checksum" ] && {
	RESULT="Checksum mismatch"
	return
    }

    # upgrade
#    $NMS -u $FWFILE
#    [ $? -ne 0 ] && {
#	RESULT="Fail to upgrade"
#	return
#    }
}

main () {
    local file=$1
    local req_msg=$2
    local val

    # initialize JSON output structure
    json_init

    json_load_file $file
    [ $? -ne 0 ] && exit 1

    tuci delete $TUCI_NMS_RX_MESSAGE
    tuci delete $TUCI_NMS_REASON
    tuci delete $TUCI_NMS_RESULT

    if [ "x$req_msg" != "xfota" ]; then
	dec_status
    else
	dec_firmware
    fi

    tuci set $TUCI_NMS_RX_MESSAGE=$RX_MESSAGE
    tuci set $TUCI_NMS_RX_RESULT="$RX_RESULT"
    tuci set $TUCI_NMS_RESULT="$RESULT"
}

main2 () {
    local file=$1
    local req_msg=$2
    local val

    json_init

    json_load_file $file

    json_dump

    json_get_var val "IMEI"
    echo "$val"
}

main2 "$@"

