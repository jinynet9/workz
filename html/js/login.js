//  0706  callback_logged 로그인시 첫페이지추가, macaddress request & responses

var bLoginPush = false;
var timerid;
var wm;

function submit_mac()
{
	var url = "/cgi-bin/auth.cgi";
	var param = "Command=Macaddr"
	XHRPost(machandle, url, param, true);
}

function machandle(xmlDoc)
{
	if (hasError(xmlDoc))
	{
		showError(xmlDoc);
		return;
	}
	
	var macaddress = getDataFromTag(xmlDoc,"macaddress");
	document.getElementById("macaddress_view").innerHTML=macaddress;
}

function submit_apply_login()
{	
	if( bLoginPush )
	{
		return;
	}
	bLoginPush = true;
	
	var url = "/cgi-bin/auth.cgi";
	var param = "Command=Login"
			+"&id="+document.getElementById( "log_id" ).value
			+"&pw="+document.getElementById( "log_pw" ).value;
			
	XHRPost(loginhandle, url, param, true);
}

function loginhandle(xmlDoc)
{
	if (hasError(xmlDoc))
	{
		showError(xmlDoc);
		return;
	}
	
	var accept = getDataFromTag(xmlDoc,"accept");

	//count = parseInt(getDataFromTag(xmlDoc,"count"));
	
	if (accept == "1")
	{
		var make_TID = getDataFromTag(xmlDoc,"make_TID");
		var SessionKey = getDataFromTag(xmlDoc,"sessionkey");
		if(parent.callback_logged)
		{
			clearTimeout(timerid);
			parent.callback_logged(make_TID, SessionKey);
		}
		else
		{
			bLoginPush = false;
		}
	}
	else if(accept == "Lock")
	{
		document.getElementById("default").style.display = "none";
		document.getElementById("login_fail").style.display = "";
		bLoginPush = false;
		login_lock();
	}
	else
	{
		bLoginPush = false;
	}
}

function login_lock()
{
	var url = "/cgi-bin/auth.cgi";
	var param = "Command=login_lock"
	
	XHRPost(lockhandle, url, param, true);
}
function lockhandle(xmlDoc)
{
	if (hasError(xmlDoc))
	{
		showError(xmlDoc);
		return;
	}
	
	var LoginState = getDataFromTag(xmlDoc,"LoginState");
	if(LoginState == "unlock")
	{
		clearTimeout(timerid);
		document.getElementById("log_id").disabled = false;
		document.getElementById("log_pw").disabled = false;
		document.getElementById("default").style.display = "";
		document.getElementById("login_fail").style.display = "none";
		document.getElementById("login_lock").style.display = "none";
		document.getElementById("login_btn").style.display = "";
		
	}
	else if(LoginState == "lock")
	{	
		timerid = setTimeout("login_lock()", (1 * 1000)); //1sec
		document.getElementById("log_id").value = "";
		document.getElementById("log_pw").value = "";
		document.getElementById("log_id").disabled = true;
		document.getElementById("log_pw").disabled = true;
		document.getElementById("default").style.display = "none";
		document.getElementById("login_fail").style.display = "none";
		document.getElementById("login_lock").style.display = "";
		document.getElementById("login_btn").style.display = "none";
		document.getElementById("lock_font").innerHTML = "This user account has been locked." 
		+ " (" + getDataFromTag(xmlDoc,"min") + "Min" + getDataFromTag(xmlDoc,"sec") + "Sec" + ")";
	}
}
function prs_key( e )
{
	if ( !e )
	{ e = window.event; }

	var key = (e.keyCode? e.keyCode : e.which);
	if ( key == 13 )
	{
		submit_apply_login();
	}
}
function authhandle(xmlDoc)
{
	if (hasError(xmlDoc))
	{
		showError(xmlDoc);
		return;
	}
	
	vers = getDataFromTag(xmlDoc,"version");	
	document.getElementById("version").innerHTML = vers;
	
	client_ip = getDataFromTag(xmlDoc,"client_ip");
	document.getElementById("ip").innerHTML = client_ip;
	
	wm = getDataFromTag(xmlDoc,"accCon");	
	wm=wm.substring(8, 12);
	var LoginState = getDataFromTag(xmlDoc,"LoginState");
	if(LoginState == "Lock")
	{
		document.getElementById("default").style.display = "none";
		document.getElementById("login_fail").style.display = "";
		bLoginPush = false;
		login_lock();
	}
}

function macon(){

        document.getElementById("accdiv").innerHTML=wm;
        document.getElementById("accdiv").style.display="";
}

function page_init()
{
	var url = "/cgi-bin/auth.cgi?Command=loginfo";
	XHRGet(authhandle, url, true);
}

function init()
{
	page_init();
}


function callback_logged( tid, SessionKey )
{
	setCookie( 'TID_NAME', tid);
	setCookie( 'Session_name', SessionKey);
	location.href="/home.html";
}