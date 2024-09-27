/*
 * IMPORTANT NOTE:
 * synchronize with back-end, error.h
 *
 * DAYOU_200912
 */


/*
 *
 */
var ERR = function()
{
    /* Initializes the enumerations */
    var err_list = [
	 "NO_ERROR"
        ,"GENERAL_ERROR"
        ,"PASSWD_MISMATCH"
        ,"PASSWD_INVALID"
    ];

    err_obj = {};
    err_list.forEach((item, index)=>err_obj[item] = index); 
    
    /* Do not allow the object to be changed */
    Object.freeze(err_obj);

    return err_obj;
}();

/*
 *
 */
function err_handle ( xmlDoc )
{
    if ( ! xmlDoc )
	return false;

    var ret = getDataFromTag ( xmlDoc, "ret" );
    var at = "";

    ret *= -1;

    if ( ret == 0 )
	return false;	/* ! error */

    switch ( ret )
    {
    case ERR.PASSWD_MISMATCH:	at = 'passwd_mismatch';		break;
    case ERR.PASSWD_INVALID:	at = 'passwd_invalid';		break;

    default:
	break;
    }

    if ( at != "" )
	alert ( xlang_alert(at) );
    else
    {
	if ( ret == 1 )
	    alert ( "Command fail" );
	else
	    alert ( "Error return code: -" + ret );
    }

    return true;	/* error */
}

function err_handle_reload ( xmlDoc )
{
    var err = err_handle ( xmlDoc );
    
    if ( err )
    {
	/* local error handle */
	if ( ! empty_(_err_handle) )
	    _err_handle ( xmlDoc );

	return;
    }

    window.location.reload ( true );
}

////////////////////////////////////////////////////////////////////////////////
//
// JS utility for DAYOU
//

function XHR_POST ( handle, cgi, cmd, formid, async )
{
    EnableButtons ( false );

    var url = "/cgi-bin/" + cgi + ".cgi";
    var param = "Command=" + cmd + "&" + serializeFormElement(formid);

    XHRPost ( err_handle_reload, url, param, async );
}

function page_fullname ()
{
    /*
    var pageName = "";
 
    var tempPageName = window.location.href;
    var strPageName = tempPageName.split("/");
    pageName = strPageName[strPageName.length-1].split("?")[0];
 
    return pageName;
    */

    return window.location.pathname.split("?")[0];
}

function jump_inform ( type )
{
    parent._last_page = page_fullname ();

    parent.jump_inform ( type );
}

function jump_inform_reboot ()
{
    jump_inform ( 'reboot' );
}

function jump_inform_upgrade ()
{
    jump_inform ( 'upgrade' );
}

function jump_inform_wifi_restart ()
{
    var rv = xconfirm ( 'wifi_restart' );
    if ( rv != true )
	return;

    jump_inform ( 'wifi_restart' );
}

function xconfirm ( type )
{
    languageFileLoad ( "/lang/lang_common.json" );

    commonInit ( parent._lang );

    var ret = confirm ( xlang_alert(type) );

    languageFileLoad ( _lang_file );

    commonInit ( parent._lang );

    if ( ret == true )
    {
	var url = "/cgi-bin/getinfo.cgi";
	var param = "Command=uptime";

	XHRPost ( xconfirm_reply, url, param, true );
    }

    return ret;
}

function xconfirm_reply ( xmlDoc )
{
    parent._uptime = getDataFromTag ( xmlDoc, "uptime" );
}

function xalert ( type )
{
    languageFileLoad ( "/lang/lang_common.json" );

    commonInit ( parent._lang );

    alert ( xlang_alert(type) );

    languageFileLoad ( _lang_file );

    commonInit ( parent._lang );
}

function xlang_alert ( type )
{
    return i18next.store.data[i18next.language].translation.alert[type];
}

function xlang_reboot ( type )
{
    return i18next.store.data[i18next.language].translation.Reboot[type];
}

function xlang_upgrade ( type )
{
    return i18next.store.data[i18next.language].translation.Upgrade[type];
}

function xlang_wifi_restart ( type )
{
    return i18next.store.data[i18next.language].translation.WifiRestart[type];
}

function elm_remove ( id )
{
    document.getElementById( id ).style.display = "none";
}

function doc_elm ( id )
{
    return document.getElementById ( id );
}

function elm_val_empty_ ( e )
{
    return e.value == "" ? true : false;
}

function int_range_ ( val, low, high )
{
    var v = parseInt ( val );

    return ( isNaN(v) || v < low || v > high ) ? false : true;
}

function passwordize ( val )
{
    var ast = "";

    for ( var i = 0; i < val.length; i++ )
	ast += "*";

    return ast;
}

function empty_ ( v )
{
    if ( typeof v == 'undefined' )
	return true;

    if ( v == null )
	return true;

    if ( v == "" )
	return true;

    /*
    if ( typeof v == 'object' )
    {
	if ( ! Object.keys(v).length )
	    return true;
    }
    */

    return false;
}

/*
 * interval (sec)
 * counter = 0 ; forever
 */
function itimer_ms ( cb, interval, counter = 0, oncb = false )
{
    var timer;
    var _cnt = counter;

    function beat () {
	var rv = cb ( _cnt );
	if ( rv < 0 )
	    _cnt = -1;

	if ( counter > 0 )
	    _cnt--;

	if ( _cnt < 0 )
	    clearInterval ( timer );
    }

    timer = setInterval ( beat, interval );

    if ( oncb )
	beat ();

    return timer;
}

function itimer ( cb, interval, counter = 0, oncb = false )
{
    return itimer_ms ( cb, interval * 1000, counter, oncb );
}

/*
 * xstring is combined with 'name value pairs' by ", "
 * xstr = "ssid=KT24, freq=2.4123, users=5";
 */
function xstr_parse ( s, lv, trim = true )
{
    var cs = s.split ( ", " );
    var rv = new Array ( lv.length );
    var _lv;

    for ( var i in cs )
    {
	for ( var j in lv )
	{
	    if ( rv[j] != undefined )
		continue;

	    _lv = lv[j] + "=";

	    if ( cs[i].indexOf (_lv) >= 0 )
	    {
		rv[j] = cs[i].substring ( _lv.length );

		if ( trim )
		    rv[j] = rv[j].trim ();
		break;
	    }
	}
    }

    return rv;
}

/*
 * JINY_210924
 */
function prevent_enterkey ()
{
    document.addEventListener('keydown', function(event) {
        if (event.keyCode === 13) {
            event.preventDefault();
            };
    }, true);
}

/*
 * Element nodes do not have a text value.
 * The text value of an element node is stored in a child node. This node is called a text node.
 */
function node_value ( node, tag )
{
    var nv;

    if ( typeof node == 'undefined' || node.length == 0 )
	return null;

    if ( typeof tag == 'undefined' )
	return node.childNodes[0].nodeValue;

    nv = node.getElementsByTagName( tag );

    return node_value ( nv[0] );
}

/*
 *
 */
function node_attr ( node, tag )
{
    var nv;

    if ( typeof node == 'undefined' || node.length == 0 )
	return null;

    if ( typeof tag == 'undefined' )
	return node.childNodes[0].nodeValue;

    nv = node.getElementsByTagName( tag );

    return node_value ( nv[0] );

    /*
    if ( nv.length == 0 )
	return "";

    return nv[0].childNodes[0].nodeValue;
    */
}

/*
 *
 */
function node_tree_first_child ( n )
{
    var y = n.firstChild;

    while ( y.nodeType != Node.ELEMENT_NODE ) {
        y = y.nextSibling;
    }

    return y;
}

/*
 *
 */
function node_tree_traverse ( node, callback, aux, idChk = true, leafChk = true )
{
    var c, i;

    c = node.childNodes;
    for ( i = 0; i < c.length; i++ )
	node_tree_traverse ( c[i], callback, aux, idChk, leafChk );

    /* avoid empty text nodes (spaces and new-line characters) */
    if ( node.nodeType != Node.ELEMENT_NODE )
	return;

    //console.log ( node );

    /* ! concerned node */
    if ( idChk == true && node.id.length == 0 )
	return;

    /* only leaf node, except first node */
    if ( leafChk == true && node.childNodes.length != 0 )
	return;

    callback ( node, aux );
}

/*
function node_load_xml ( http_id, xmlDoc, xml_tag )
{
    var n = document.getElementById ( http_id );
    var v;

    if ( typeof xml_tag == 'undefined' )
	xml_tag = http_id;

    v = getDataFromTag ( xmlDoc, xml_tag );

    switch ( n.type )
    {
    case "checkbox":
	n.checked = (v == 1) ? true : false;
	break;
    case "text":
	n.value = v;
	break;
    }
}
*/

function __node_load_xml ( node, xmlDoc )
{
    var v;

    v = getDataFromTag ( xmlDoc, node.id );

    switch ( node.type )
    {
    case "checkbox":
	node.checked = (v == 1) ? true : false;
	break;
    case "text":
	node.value = v;
	break;
    }
}

function node_tree_load_xml ( node, xmlDoc )
{
    node_tree_traverse ( node, __node_load_xml, xmlDoc );
}

function __node_enable ( node, enable ) {
    node.disabled = !enable;
}

function node_tree_enable ( node, enable )
{
    node_tree_traverse ( node, __node_enable, enable );
}

/*
 * nodeType
 *
    for ( var name in Node ) {
	console.log ( name, Node[name] );
    }

 *
 *
ELEMENT_NODE 1
ATTRIBUTE_NODE 2
TEXT_NODE 3
CDATA_SECTION_NODE 4
ENTITY_REFERENCE_NODE 5
ENTITY_NODE 6
PROCESSING_INSTRUCTION_NODE 7
COMMENT_NODE 8
DOCUMENT_NODE 9
DOCUMENT_TYPE_NODE 10
DOCUMENT_FRAGMENT_NODE 11
NOTATION_NODE 12
DOCUMENT_POSITION_DISCONNECTED 1
DOCUMENT_POSITION_PRECEDING 2
DOCUMENT_POSITION_FOLLOWING 4
DOCUMENT_POSITION_CONTAINS 8
DOCUMENT_POSITION_CONTAINED_BY 16
DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC 32
 */

/*
 * call by reference
 *
function _node_enable ( node, data ) {
    if ( node == data.start_node )
	return;

    console.log(node);
    node.disabled = !data.enable;
}
function node_tree_enable ( node, enable )
{
    var data = { node:node, enable:enable };

    node_tree_traverse ( node, _node_enable, data );
}
*/

/*
 * call by value
 *
function _node_enable ( node, data ) {
    let [ start_node, enable ] = data;

    if ( node == start_node )
	return;

    console.log(node);
    node.disabled = !enable;
}
function node_tree_enable ( node, enable )
{
    let data = [ node, enable ];

    node_tree_traverse ( node, _node_enable, data );
}
*/



function isEmptyObj(obj)
{
    if ( obj.constructor === Object && Object.keys(obj).length === 0 )
	return true;

    return false;
}

function init()
{
    console.log ( `common onload:: ${_lang_file}` );
    prevent_enterkey ();
    languageFileLoad ( _lang_file );
    CheckLogin(page_init);
}

