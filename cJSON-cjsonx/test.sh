
CJSON=./cjson

json_add_string () {
    $CJSON add_string "$1" "$2"
}

json_add_int () {
    $CJSON add_int "$1" "$2"
}

json_add_boolean () {
    $CJSON add_bool "$1" "$2"
}

json_add_object () {
    $CJSON add_object "$1"
}

json_close_object () {
    $CJSON close_object
}

json_add_array () {
    $CJSON add_array "$1"
}

json_close_array () {
    $CJSON close_array
}

json_init () {
    $CJSON init
}

json_dump () {
    $CJSON dump "" "0"
}

json_load_file () {
    $CJSON load_file "$1"
}

json_select () {
    $CJSON select "$1"
}

json_get_var () {
    local _v_=$( $CJSON get_var "$2" )

    eval "export -- \"${1}=\${_v_}\""
}

json_get_var_array () {
    local _v_=$( $CJSON get_var_array "$2" )

    eval "export -- \"${1}=\${_v_}\""
}


DEBUG_DATA_FILE="./nms.debug"

enc_debug_data () {
    local data

    [ ! -f "$DEBUG_DATA_FILE" ] && return

    json_add_array "debug_data"

    while read line
    do
	json_add_string "" "$line"
    done < $DEBUG_DATA_FILE

    json_close_array

#    rm -f $DEBUG_DATA_FILE
}

json_init

enc_debug_data

json_dump
