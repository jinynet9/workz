#!/bin/sh

if [ $# -lt 1 ]; then
    ME=`basename "$0"`
    echo "Usage: $ME (report|ack|)"
    exit 1
fi

#. ./zut.sh
#. ./dut.sh
#
#. ./nms.env

. ./cjson.sh

ONLINE=$( tuci get $TUCI_NMS_ONLINE )


enc_status_header() {
    local msg_type="$1"
#    local imei=$( mob_imei )
    local imei="imei_sample"

    json_add_string "IMEI" $imei
    json_add_string "message" $msg_type

    json_add_object "status"
}

enc_status_tailer() {
    json_close_object
}

enc_firmware_header() {
    local msg_type="$1"
    local imei=$( mob_imei )

    json_add_string "IMEI" $imei
    json_add_string "message" $msg_type
}

enc_firmware_tailer() {
    return
}

enc_ip_addr() {
    local ifname=$1
    local name=$2
    local val=$( if_ip_addr $ifname )

#    name="ifname"
    val="1.1.1.1"
    json_add_string "$name" $val
}

enc_model () {
    local val=$( sys_model )

    json_add_string "model" $val
}

enc_serial () {
    local val=$( sys_serial )

    json_add_string "serial" $val
}

enc_device_fw_ver () {
    local val=$( device_fw_ver )

    json_add_string "device_fw_ver" $val
}

enc_modem_fw_ver () {
    local val=$( modem_fw_ver )

    json_add_string "modem_fw_ver" $val
}

# International Mobile Equipment Identity
enc_mob_imei () {
    local val=$( mob_imei )

    json_add_string "IMEI" $val
}

# Cellular Telephone Network
enc_mob_ctn () {
    local val=$( mob_ctn )

    json_add_string "CTN" $val
}

enc_uptime () {
    local val=$( sys_uptime )

    json_add_int "uptime" $val
}

enc_cpu_load () {
    local val min1 min5 min15

    cpu_load min1 min5 min15 > /dev/null
#    val=$( cpu_load )

#    min1=$(  echo $val | cut -d' ' -f1 )
#    min5=$(  echo $val | cut -d' ' -f2 )
#    min15=$( echo $val | cut -d' ' -f3 )

    json_add_array cpuload

    json_add_int "" $min1
    json_add_int "" $min5
    json_add_int "" $min15

    json_close_array
}

enc_memory_usage () {
    local val=$( memory_usage )

    json_add_int "memory_usage" $val
}

enc_if_stats () {
    local ifname=$1
    local rxname=$2
    local txname=$3
    local val rx tx

    val=$( if_stats $ifname )

    rx=$( echo $val | cut -d' ' -f1 )
    tx=$( echo $val | cut -d' ' -f2 )

    json_add_string $rxname $rx
    json_add_string $txname $tx

    local tuci_opt_rx="${TUCI_NMS_STATUS}.${ifname}_${rxname}"
    local tuci_opt_tx="${TUCI_NMS_STATUS}.${ifname}_${txname}"
    local rx_prev=$( tuci get $tuci_opt_rx )
    local tx_prev=$( tuci get $tuci_opt_tx )
    local rx_delta tx_delta

    tuci set $tuci_opt_rx=$rx
    tuci set $tuci_opt_tx=$tx

    if [ -z $rx_prev ]; then
	rx_delta=$rx
	tx_delta=$tx
    else
	rx_delta=$((rx - rx_prev))
	tx_delta=$((tx - tx_prev))
    fi

    json_add_string "${rxname}_delta" $rx_delta
    json_add_string "${txname}_delta" $tx_delta
}

enc_band () {
    json_add_array band

    json_add_string "" "band1"
    json_add_string "" "band5"
    json_add_string "" "band7"

    json_close_array
}

enc_modem_signal () {
    local val

    json_add_object "modem_signal"

    val=$( modem_5g_nr_tx_power )
    val="tx power value"
    json_add_string "tx_power" "$val"

    val=$( modem_5g_nr_rsrp )
    val="rsrp value"
    json_add_string "rsrp" "$val"

    val=$( modem_5g_nr_sinr )
    val="sinr value"
    json_add_string "sinr" "$val"

    json_close_object
}

enc_net_mobile() {
    json_add_object "MOBILE"

    enc_if_stats $WWANIF "traffic_rx" "traffic_tx"

    json_close_object
}

_enc_net_ether() {
    local ifname=$1
    local val

    json_add_object $ifname

    val=$( if_is_up $ifname )

    json_add_boolean "link" $val

    enc_if_stats $ifname "traffic_rx" "traffic_tx"

    json_close_object
}

enc_net_ether() {
    local ifname
    local i

    json_add_object "ETHERNET"

    SET=$( seq 0 4 )
    for i in $SET; do
        ifname="eth$i"
        _enc_net_ether $ifname
    done

    json_close_object
}

#
#
#
enc_reboot_ack () {
    local result="$@"

    enc_status_header "reboot"

    json_add_string "reboot_result" "$result"

    enc_status_tailer
}

#
#
#
enc_config_ack () {
    local profile=$1
    local result

    shift 1
    result="$@"

    enc_status_header "config"

    json_add_string "config_profile" $profile
    json_add_string "config_result" "$result"

    enc_status_tailer
}

#
#
#
_enc_alarm () {
    local file="${NMS_TMP_PATH}/alarm"
    local line

    if [ ! -e $file ]; then
	return
    fi

    json_add_array alarm

    while read line
    do
	json_add_string "" $line
    done < $file

    json_close_array
}

enc_alarm () {
    local str
    local alarms
    local alm

    enc_status_header "report"

    _enc_alarm

    enc_status_tailer
}

#
#
#
enc_report () {
    enc_status_header "report"

    # OFFLINE
    [ "x$ONLINE" != "x1" ] && {
	enc_model
	enc_serial

	enc_device_fw_ver
	enc_modem_fw_ver

	enc_mob_ctn

	enc_band

	_enc_alarm
    }

    enc_uptime
    enc_cpu_load
    enc_memory_usage

    enc_ip_addr $WWANIF "mobile_ip"
    enc_ip_addr $EWANIF "ethernet_ip"

    enc_modem_signal

    enc_net_mobile
    enc_net_ether

    enc_status_tailer
}

#
#
#
enc_fota_query () {
    enc_firmware_header "fota_query"

    enc_model
    enc_device_fw_ver

    enc_firmware_tailer
}

#
#
#
enc_fota_ack () {
    local result="$@"

    enc_status_header "fota"

    json_add_string "fota_result" "$result"

    enc_status_tailer
}

#
#
#
main () {
    local msg=$1
    local args

    # initialize JSON output structure
    json_init

    shift 1
    args="$@"

    if [ $msg = "report" ]; then
	enc_report $args
    elif [ $msg = "alarm" ]; then
	enc_alarm $args
    elif [ $msg = "reboot_ack" ]; then
	enc_reboot_ack $args
    elif [ $msg = "config_ack" ]; then
	enc_config_ack $args
    elif [ $msg = "fota" ]; then
	enc_fota_query $args
    elif [ $msg = "fota_ack" ]; then
	enc_fota_ack $args
    else
	exit 2
    fi

    json_dump
}

main "$@"
exit 0

while [ true ]; do
main "$@"
sleep 0.01
done

