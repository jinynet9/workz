function go1( url )
{
	var iframe = document.getElementById("menu_content");
	
	iframe.setAttribute("src", page_load_url_header() + url);
	activeMenu(url);
}

var _auth_handler = null;

function CheckLogin (handler)
{
	_auth_handler = handler;

	var url = "/cgi-bin/auth.cgi";
	var param = "Command=CheckAuth";

	XHRPost(AuthCheckhandle, url, param, true);
}

function AuthCheckhandle(xmlDoc)
{
	var authcheck = getDataFromTag(xmlDoc, "AuthCompleted");

	if (hasError(xmlDoc))
	{
		showError(xmlDoc);
		return;
	}

	if (authcheck == "1")
	{
		var org_lang = parent._lang;

		parent._lang = getDataFromTag(xmlDoc, "lang");
		commonInit (parent._lang);

		if (_auth_handler)
			_auth_handler();

		if (org_lang != parent._lang)
			commonInit (parent._lang);
	}
	else
	{
		parent.location.href="/login.html";
	}

	_auth_handler = null;
}

function activeMenu(url) {
	$("a.on").removeClass("on");
    if(url.indexOf("/overview/") > 0) {
        $("#menu_overview").addClass("active");
        $("#menu_overview .hidden-ul").css("display", "block");

        if(url.indexOf("/sys_info") > 0) {
            $("#menu_sys_info").find("a").addClass("on");
        }
    }else if(url.indexOf("/wan/") > 0) {
        $("#menu_wan").addClass("active");
        $("#menu_wan .hidden-ul").css("display", "block");

        if(url.indexOf("/basic_setting") > 0) {
            $("#menu_basic_setting").find("a").addClass("on");
        }else if(url.indexOf("/connection_info") > 0) {
            $("#menu_connection_info").find("a").addClass("on");
        }else if(url.indexOf("/lte_setting") > 0) {
            $("#menu_lte_setting").find("a").addClass("on");
        }else if(url.indexOf("/lte_state") > 0) {
            $("#menu_lte_state").find("a").addClass("on");
        }
    }else if(url.indexOf("/lan/") > 0) {
        $("#menu_lan").addClass("active");
        $("#menu_lan .hidden-ul").css("display", "block");

	if(url.indexOf("/lan_basic") > 0) {
            $("#menu_basic_setup").find("a").addClass("on");
        }
    }else if(url.indexOf("/wireless24/") > 0) {
        $("#menu_24g").addClass("active");
        $("#menu_24g .hidden-ul").css("display", "block");

        if(url.indexOf("/wireless24_setup") > 0) {
            $("#menu_wireless24_setup").find("a").addClass("on");
        }else if(url.indexOf("/wireless24_vap_list") > 0) {
            $("#menu_wireless24_vap_list").find("a").addClass("on");
        }else if(url.indexOf("/wireless24_macfilter") > 0) {
            $("#menu_wireless24_macfilter").find("a").addClass("on");
        }else if(url.indexOf("/wireless24_connect_limit") > 0) {
            $("#menu_wireless24_connect_limit").find("a").addClass("on");
        }else if(url.indexOf("/wireless24_ssid_priority") > 0) {
            $("#menu_wireless24_ssid_priority").find("a").addClass("on");
        }else if(url.indexOf("/wireless24_wmm") > 0) {
            $("#menu_wireless24_wmm").find("a").addClass("on");
        }else if(url.indexOf("/wireless24_channel_scan") > 0) {
            $("#menu_wireless24_channel_scan").find("a").addClass("on");
        }
    }else if(url.indexOf("/wireless5/") > 0) {
        $("#menu_5g").addClass("active");
        $("#menu_5g .hidden-ul").css("display", "block");

        if(url.indexOf("/wireless5_setup") > 0) {
            $("#menu_wireless5_setup").find("a").addClass("on");
        }else if(url.indexOf("/wireless5_vap_list") > 0) {
            $("#menu_wireless5_vap_list").find("a").addClass("on");
        }else if(url.indexOf("/wireless5_macfilter") > 0) {
            $("#menu_wireless5_macfilter").find("a").addClass("on");
        }else if(url.indexOf("/wireless5_connect_limit") > 0) {
            $("#menu_wireless5_connect_limit").find("a").addClass("on");
        }else if(url.indexOf("/wireless5_ssid_priority") > 0) {
            $("#menu_wireless5_ssid_priority").find("a").addClass("on");
        }else if(url.indexOf("/wireless5_wmm") > 0) {
            $("#menu_wireless5_wmm").find("a").addClass("on");
        }else if(url.indexOf("/wireless5_channel_scan") > 0) {
            $("#menu_wireless5_channel_scan").find("a").addClass("on");
        }

    }else if(url.indexOf("/firewall/") > 0) {
        $("#menu_firewall").addClass("active");
        $("#menu_firewall .hidden-ul").css("display", "block");

        if(url.indexOf("/portfw") > 0) {
            $("#menu_portfw").find("a").addClass("on");
        }else if(url.indexOf("/dmz") > 0) {
            $("#menu_dmz_set").find("a").addClass("on");
        }else if(url.indexOf("/firewall") > 0) {
            $("#menu_firewall_set").find("a").addClass("on");
        }
    }else if(url.indexOf("/advanced/") > 0) {
        $("#menu_advanced").addClass("active");
        $("#menu_advanced .hidden-ul").css("display", "block");

        if(url.indexOf("/aaa") > 0) {
            $("#menu_aaa").find("a").addClass("on");
        }else if(url.indexOf("/hotspot_setting") > 0) {
            $("#menu_hotspot_setting").find("a").addClass("on");
        }else if(url.indexOf("/web_auth") > 0) {
            $("#menu_web_auth").find("a").addClass("on");
        }else if(url.indexOf("/white_list") > 0) {
            $("#menu_white_list").find("a").addClass("on");
        }else if(url.indexOf("/ntp") > 0) {
            $("#menu_ntp").find("a").addClass("on");
        }else if(url.indexOf("/remote_access") > 0) {
            $("#menu_remote_access").find("a").addClass("on");
        }else if(url.indexOf("/acs") > 0) {
            $("#menu_acs").find("a").addClass("on");
        }else if(url.indexOf("/band_steering_setting") > 0) {
            $("#menu_band_steering_setting").find("a").addClass("on");
        }else if(url.indexOf("/backhaul_search") > 0) {
            $("#menu_backhaul_search").find("a").addClass("on");
        }else if(url.indexOf("/auto_reboot") > 0) {
            $("#menu_auto_reboot").find("a").addClass("on");
        }else if(url.indexOf("/system_setup") > 0) {
            $("#menu_system_setup").find("a").addClass("on");
        }else if(url.indexOf("/upgrade") > 0) {
            $("#menu_upgrade").find("a").addClass("on");
        }else if(url.indexOf("/ap_search") > 0) {
            $("#menu_ap_search").find("a").addClass("on");
        }else if(url.indexOf("/speed_check_fin") > 0) {
            $("#menu_speed_check_fin").find("a").addClass("on");
        }else if(url.indexOf("/ip_detective") > 0) {
            $("#menu_ip_detective").find("a").addClass("on");
        }else if(url.indexOf("/atf") > 0) {
            $("#menu_atf").find("a").addClass("on");
        }
    }else if(url.indexOf("/state/") > 0) {
        $("#menu_state").addClass("active");
        $("#menu_state .hidden-ul").css("display", "block");

        if(url.indexOf("/terminal_info") > 0) {
            $("#menu_terminal_info").find("a").addClass("on");
        }else if(url.indexOf("/ap_info") > 0) {
            $("#menu_ap_info").find("a").addClass("on");
        }else if(url.indexOf("/net_statistics") > 0) {
            $("#menu_net_statistics").find("a").addClass("on");
        }else if(url.indexOf("/asso_statistics") > 0) {
            $("#menu_asso_statistics").find("a").addClass("on");
        }else if(url.indexOf("/log_info") > 0) {
            $("#menu_log_info").find("a").addClass("on");
        }
    }
}

function IgnoreEnterKey( e )
{
	if ( !e )
	{ e = window.event; }

	var key = (e.keyCode? e.keyCode : e.which);
	if ( key == 13 )
	{
		if ( window.event )
		{ e.returnValue = false; }
		else if ( e.preventDefault )
		{ e.preventDefault(); }

		return false;
	}
}

function isAllNum( str, exceptions )
{
	var ch;
	for ( var i=0; i < str.length; i++ )
	{
		ch = str.charAt( i );
		if ( (ch >= '0' && ch <= '9') || (exceptions != undefined && exceptions.indexOf( ch ) != -1) )
		{ continue; }
		return 0;
	}
	return 1;
}

function atoi(str, num)
{
	var i=1;
	if ( num != 1 )
	{
		while ( i != num && str.length != 0 )
		{
			if ( str.charAt( 0 ) == '.' )
			{ i++; }
			str = str.substring( 1 );
		}
		if ( i != num )
		{ return -1; }
	}

	for ( var i=0; i < str.length; i++ )
	{
		if ( str.charAt( i ) == '.' )
		{
			str = str.substring( 0, i );
			break;
		}
	}
	if ( str.length == 0 )
	{ return -1; }
	return parseInt( str, 10 );
}

// ip to hex (ex: 255.255.255.255 -> 0xffffffff)
function aton( ipAddr )
{
	var ret = ((atoi( ipAddr, 1 ) << 24) & 0xff000000) |
		((atoi( ipAddr, 2 ) << 16) & 0x00ff0000) |
		((atoi( ipAddr, 3 ) << 8) & 0x0000ff00) |
		(atoi( ipAddr, 4 ) & 0x000000ff);
	return ret;
}

// hex to ip (ex: 0xffffffff -> 255.255.255.255)
function ntoa( ipAddr )
{
	var ret = String( (ipAddr >> 24) & 0x000000ff ) + "." +
		String( (ipAddr >> 16) & 0x000000ff ) + "." +
		String( (ipAddr >> 8) & 0x000000ff ) + "." +
		String( ipAddr & 0x000000ff );
	return ret;
}

var http_request;
function makeRequest( url, content, handler )
{
	http_request = false;
	if ( window.XMLHttpRequest )
	{  // Mozilla, Safari,...
		http_request = new XMLHttpRequest();
		if ( http_request.overrideMimeType )
		{
			http_request.overrideMimeType( "text/xml" );
		}
	}
	else if ( window.ActiveXObject )
	{  // IE
		try {
			http_request = new ActiveXObject( "Msxml2.XMLHTTP" );
		} catch ( e ) {
			try {
			http_request = new ActiveXObject( "Microsoft.XMLHTTP" );
			} catch ( e ) {}
		}
	}
	if ( !http_request )
	{
		alert( "Giving up :( Cannot create an XMLHTTP instance" );
		return false;
	}
	if ( handler == undefined )
	{ http_request.onreadystatechange = alertContents; }
	else
	{ http_request.onreadystatechange = handler; }
	http_request.open( "POST", url, true );
	http_request.send( content );
}

function alertContents()
{
	if ( http_request.readyState == 4 )
	{
		if ( http_request.status == 200 )
		{  // refresh
			window.location.reload();
		}
		else
		{
			alert( "There was a problem with the request." );
		}
	}
}

var secs;
var timerID = null;
var timerRunning = false;
var Progressing = false;
var TIMEOUT_DEFAULT_TIME = 3;
var MaxTime = TIMEOUT_DEFAULT_TIME;
function StartTheTimer(withProgress)
{
	if(withProgress || Progressing) {
        Progressing = true;
        var progress = 100 - Math.round( (secs / MaxTime) * 100);
        $("#update_progress_bar").css("width",progress+"%");
	}

	if ( secs == 0 )
	{
		//TimeoutReload( TIMEOUT_DEFAULT_TIME );
		window.location.reload( true );
		//window.location.href=window.location.href;	//reload page
	}
	else
	{
		self.status = secs;
		secs = secs - 1;
		timerRunning = true;
		timerID = self.setTimeout( "StartTheTimer()", 1000 );
	}
}

function TimeoutReload( timeout, withProgress )
{
	if ( timeout == undefined )
	{ secs = TIMEOUT_DEFAULT_TIME; }
	else
	{
		secs = timeout;
        MaxTime = timeout;
	}

	if ( timerRunning )
	{ clearTimeout( timerID ); }

	timerRunning = false;
	StartTheTimer(withProgress);
}

function startProgress(timeout) {
    if(secs <= timeout) {

    }else {
        clearInterval();
        //window.location.reload();
    }
}

/*
 * DAYOU_JINY_200914
 */
function reload_timer_stop ()
{
	if ( ! timerRunning )
	    return;

	clearTimeout ( timerID );

	timerRunning = false;
}

function DimIndicator() {
    document.getElementById( "loading" ).style.display = "";
    document.getElementById( "dim" ).style.display = "";
}

var objButtons = new Array();
function SetButtons( btn_ids )
{
	for ( var i=0; i < btn_ids.length; i++ )
	{ objButtons.push( document.getElementById( btn_ids[i] ) ); }
}

function EnableButtons( enable )
{
	for ( var i=0; i < objButtons.length; i++ )
	{ objButtons[i].disabled = !enable; }
}

function SelectAlert( obj, msg )
{
	obj.select();
	alert( msg );
}


function checkMac( field )
{
	var re = /^[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}$/;
	if ( !re.test( field.value ) )
	{
		//field.value = field.defaultValue;
		field.select();
		return false;
	}
	var j = 0;
	var str = field.value;
	for ( var i=0; i < str.length; i++ )
	{
		if ( str.charAt( i ) == ':' )
		{
			j++;
			if ( j > 5 )
			{
				return false;
			}
		}
	}

	return true;
}

function checkRange( str, num, min, max )
{
	d = atoi( str, num );
	if ( d > max || d < min )
	{ return false; }
	return true;
}

function checkIPAddr( field )
{
	if ( isAllNum( field.value, "." ) == 0 )
	{
		//field.value = field.defaultValue;
		field.select();
		return false;
	}

	var re = /^(\d{1,3}\.){3}\d{1,3}$/;
	if ( !re.test( field.value ) )
	{
		//field.value = field.defaultValue;
		field.select();
		return false;
	}

	if ( (!checkRange( field.value, 1, 0, 255 )) ||
			(!checkRange( field.value, 2, 0, 255 )) ||
			(!checkRange( field.value, 3, 0, 255 )) ||
			(!checkRange( field.value, 4, 0, 255 )) )
	{
		//field.value = field.defaultValue;
		field.select();
		return false;
	}

	return true;
}

function checkSubnetMask( field )
{
	if ( isAllNum( field.value, "." ) == 0 )
	{
		//field.value = field.defaultValue;
		field.select();
		return false;
	}

	var re = /^(\d{1,3}\.){3}\d{1,3}$/;
	if ( !re.test( field.value ) )
	{
		//field.value = field.defaultValue;
		field.select();
		return false;
	}

	if ( (!checkRange( field.value, 1, 0, 255 )) ||
			(!checkRange( field.value, 2, 0, 255 )) ||
			(!checkRange( field.value, 3, 0, 255 )) ||
			(!checkRange( field.value, 4, 0, 255 )) )
	{
		//field.value = field.defaultValue;
		field.select();
		return false;
	}

	return true;
}

function MaskingIP( ip, mask )
{
	var maskedIP = new Array();
	var retIP;

	for ( var i=0; i < 4; i++ )
	{
		maskedIP[i] = atoi( ip, i + 1 ) & atoi( mask, i + 1 );
	}

	retIP = maskedIP[0] + "." + maskedIP[1] + "." + maskedIP[2] + "." + maskedIP[3];
	return retIP;
}

function checkSpecial( str )
{
	//var re = /[ ~!\;\"<>,\{\}\|\`@\#$%^&*\()\+']/gi;
	var re = /[ ~!;"<>,{}|`@#$%^&*()+'\\\[\]]/;  //"
	if ( re.test( str ) )
	{
		return false;
	}
	return true;
}

function EnableGroup( grp, enable )
{
	for ( var i=0; i < grp.length; i++ )
	{ grp[i].disabled = !enable; }
}

function CleanValueGroup( grp )
{
	for ( var i=0; i < grp.length; i++ )
	{ grp[i].value = ""; }
}

function SelectFromValue( objSelect, value, defaultIndex )
{
	if ( objSelect == undefined || !objSelect || !objSelect.options || value == undefined )
	{ return false; }

	for ( var i=0; i < objSelect.options.length; i++ )
	{
		if ( objSelect.options[i].value == value )
		{
			objSelect.selectedIndex = i;
			return true;
		}
	}

	if ( defaultIndex != undefined )
	{
		var nIndex = parseInt( defaultIndex );
		if ( nIndex >= 0 && nIndex < objSelect.options.length )
		{
			objSelect.selectedIndex = nIndex;
		}
	}

	return false;
}

function ValidatePINChecksum( strPIN )
{
	var accum = 0;

	accum += 3 * (parseInt( strPIN / 10000000 ) % 10);
	accum += 1 * (parseInt( strPIN / 1000000 ) % 10);
	accum += 3 * (parseInt( strPIN / 100000 ) % 10);
	accum += 1 * (parseInt( strPIN / 10000 ) % 10);
	accum += 3 * (parseInt( strPIN / 1000 ) % 10);
	accum += 1 * (parseInt( strPIN / 100 ) % 10);
	accum += 3 * (parseInt( strPIN / 10 ) % 10);
	accum += 1 * (parseInt( strPIN / 1 ) % 10);

	return ((accum % 10) == 0);
}

function getHttpRequestObject()
{
	var objHttp;

	if ( window.XMLHttpRequest )
	{
		objHttp = new XMLHttpRequest();
	}
	else if ( window.ActiveXObject )
	{
		try
		{
			objHttp = new ActiveXObject( "Microsoft.XMLHTTP" );
		}
		catch ( e )
		{
			objHttp = new ActiveXObject( "Msxml2.XMLHTTP" );
		}
	}

	return objHttp;
}

function setAndroidHelpAlign()
{
	var objHelp = document.getElementById( "div_help" );

	if ( objHelp )
	{
		if ( navigator.userAgent.indexOf( "Android" ) < 0 )
		{
			objHelp.style.height = "100%";
		}
	}
}

// for Galaxy Tab 10.1 browser bug
function setAndroidViewRegion()
{
	var objBody = document.getElementById( "mybody" );

	if ( objBody && navigator.userAgent.indexOf( "Android" ) >= 0 )
	{
		var objIfr = document.createElement( "iframe" );

		if ( objIfr )
		{
			objIfr.style.width = "100%";
			objIfr.style.height = (window.clientHeight? window.clientHeight : 1024);
			objIfr.style.border = 0;
			objIfr.src = "about:blank";

			// view blank frame and extend view region
			objBody.appendChild( objIfr );

			// hide blank frame, but view region still extended
			//objIfr.style.display = "none";
		}
	}
}

function setBodyAlignCenter()
{
	if ( window.top.smenu )
	{
		window.top.smenu.callback_alignBody( document.getElementById( "tblBody" ) );
	}
}

function setAdjustHelpLayerWidth()
{
	var objHelp = document.getElementById( "div_help" );

	if ( objHelp )
	{
		var diff = 350 - parseInt( objHelp.offsetWidth );
		if ( diff != 0 )
		{
			objHelp.style.width = String( 330 + diff ) + "px";
		}
	}
}

function commonInit(language)
{
	setAndroidHelpAlign();
	setAndroidViewRegion();
	setAdjustHelpLayerWidth();

	if(language) {
        languageChange(language);
	}
}

function isMobile()
{
	var isie=(/msie/i).test(navigator.userAgent); //ie
	var isie6=(/msie 6/i).test(navigator.userAgent); //ie 6
	var isie7=(/msie 7/i).test(navigator.userAgent); //ie 7
	var isie8=(/msie 8/i).test(navigator.userAgent); //ie 8
	var isie9=(/msie 9/i).test(navigator.userAgent); //ie 9
	var isfirefox=(/firefox/i).test(navigator.userAgent); //firefox
	var isapple=(/applewebkit/i).test(navigator.userAgent); //safari,chrome
	var isopera=(/opera/i).test(navigator.userAgent); //opera
	var isios=(/(ipod|iphone|ipad)/i).test(navigator.userAgent);//ios
	var isipad=(/(ipad)/i).test(navigator.userAgent);//ipad
	var isandroid=(/android/i).test(navigator.userAgent);//android

	if(isie7 || isie8 || isie9) 
		isie6=false;
	if(isie9) 
		isie=false;
		
	if(isios || isipad || isandroid)
	{
		return true;
	}
	else
	{
		return false;
	}
}

function setCookie( name, value )
{
	document.cookie = name + "=" + escape( value ) + ";path=/";
}

function getCookie( name )
{
	var arrCookies = document.cookie.split( ";" );

	for ( var i=0; i < arrCookies.length; i++ )
	{
		var cookiePair = arrCookies[i].split( "=" );
		if ( cookiePair[0] == name )
		{
			return unescape( cookiePair[1] );
		}
	}

	return "";
}

function removeCookies()
{
	var res = document.cookie;
	var multiple = res.split(";");
	for (var i = 0; i < multiple.length; i++)
	{
		var key = multiple[i].split("=");
		document.cookie = key[0] + " =; expires = Thu, 01 Jan 1970 00:00:00 UTC";
	}
}

function put_security_info( security_info )
{	
	var security_key = "";

	if(security_info != 0)
	{
		for(var i=0; i<security_info; i++)
		{
			security_key += "*";
		}
		return security_key;
	}
	else
	{
		return "";
	}
}

function replace_security_info( security_info )
{
    var security_key = "";

    if(security_info != 0)
    {
        for(var i=0; i<security_info.length; i++)
        {
            security_key += "*";
        }
        return security_key;
    }
    else
    {
        return "";
    }
}


function delCookie( name )
{
	var dummyTime = new Date();
	var value = getCookie( name );

	if ( value != "" )
	{
		dummyTime.setTime( dummyTime.getTime( -1 ) );
		document.cookie = name + "=" + value + ";expires=" + dummyTime.toGMTString();
	}
}

function spec_display_replace( str )
{
    return str.replace(/\&lt;/g, "<").replace(/\&gt;/g, ">").replace(/\&quot;/g, "\"").replace(/\&amp/g, "&");
}

function trim_split(data, token) {
    return $.map(data.split(token),$.trim);
}

function page_load_url_header ()
{
	return "/cgi-bin/auth.cgi?Command=ReadPage&page="
}

function goto_vap_setting(band)
{
	window.location.replace (page_load_url_header() + "wireless/wireless_vap_list.html&band=" + band);
}

function getURLParams(url)
{
    var result = {};

	url.replace(/[?&]{1}([^=&#]+)=([^&#]*)/g, function(s, k, v) { result[k] = decodeURIComponent(v); });
    return result;
}

function checkSharedSecret (keystr)
{
	return /^(?=.*[^a-zA-Z0-9])(?=.*[0-9]).{16,32}$/.test(keystr);
}
