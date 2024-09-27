var dhcpEnableGroup = new Array();
var FlagStartIPRangeLimit;
var FlagEndIPRangeLimit;
var startIPRangeLimit = new Array();
var endIPRangeLimit = new Array();
var alertStartRangeText;
var dhcpGatewayArray;
var dhcpNetMaskArray;
var row_num;

function auth()
{
	var url = "/cgi-bin/auth.cgi";
	param = "Command=LoginCheck";
	XHRPost(authCheckhandle, url, param, true);
}
function authCheckhandle(xmlDoc)
{
	if (hasError(xmlDoc))
	{
		showError(xmlDoc);
		return;
	}

	var authcheck = getDataFromTag(xmlDoc,"LoginMode");
	var Session_name = getDataFromTag(xmlDoc,"session");
	setCookie( Session_name, getDataFromTag(xmlDoc,"sessionkey"));
	var objSessionKeys = document.getElementsByName( "SessionKey" );
	for(i=0; i<objSessionKeys.length; i++)
	{
		objSessionKeys[i].value = getDataFromTag(xmlDoc,"sessionkey");
	}

	if(authcheck == "1")
	{
        top_init();
        var language = getDataFromTag(xmlDoc, "lang");
				
        top_init();
        commonInit(language);
		//page_init();
	}
	else
	{
		window.top.location.replace(page_load_url_header() + "/overview/sys_info.html");
	}
}

function updateState_dhcp()
{
	var objCheck = document.formDhcpSet.dhcpServer;

	if(objCheck.checked)
	{
		objCheck.value = 1;
	}
	else
	{
		objCheck.value = 0;
	}

	for(var i=0; i<dhcpEnableGroup.length; i++)
	{
		var objTemp = document.getElementById(dhcpEnableGroup[i]);
		if(objTemp)
		{
			objTemp.style.display = (objCheck.value == 1 ? "" : "none");
		}
	}
}

function initIPRangeSetting()
{
	var objCheck = document.formDhcpSet.IPRangeLimitEnable;
	if(objCheck.checked)
	{
		//public egg IP ragne enable
		FlagStartIPRangeLimit = "172.30.128.1";
		FlagEndIPRangeLimit = "172.30.191.254";
		startIPRangeLimit = FlagStartIPRangeLimit.split('.');
		endIPRangeLimit = FlagEndIPRangeLimit.split('.');
		alertStartRangeText=("다시 입력해 주세요. (172.30.128.1 ~ 172.30.191.254)");
	}
	else
	{
		//kt ip range enable
		FlagStartIPRangeLimit = "172.16.1.1";
		FlagEndIPRangeLimit = "172.32.254.254";
		startIPRangeLimit = FlagStartIPRangeLimit.split('.');
		endIPRangeLimit = FlagEndIPRangeLimit.split('.');
		alertStartRangeText=("다시 입력해 주세요. (172.16.1.1 ~ 172.32.254.254)");
	}
}

function updateState_publicEgglimit()
{
	var objCheck = document.formDhcpSet.IPRangeLimitEnable;
	if(objCheck.checked){
		objCheck.value = 1;
	}
	else{
		//kt ip range enable
		objCheck.value = 0;
	}
	initIPRangeSetting();
}

function initStartIP()
{
	var objFullIP = document.formDhcpSet.dhcpStartIp;
	var ipValue1 = document.formDhcpSet.start1;
	var ipValue2 = document.formDhcpSet.start2;
	var ipValue3 = document.formDhcpSet.start3;
	var ipValue4 = document.formDhcpSet.start4;

	objFullIP.value = ipValue1.value+"."+ipValue2.value+"."+ipValue3.value+"."+ipValue4.value;
}

function endIP(ipValue)
{
	var objMax = document.formDhcpSet.maxip;
	var maxNum = parseInt(objMax.value);
	var up=0;

	ipValue[3] = ipValue[3]+(maxNum-1);
	//create d, c class -> 1~254
	//create b class -> up
	for(i=3; i>=2; i--){
		if(i!=3)
		{
			up=0;
		}

		while(1)
		{
			var tmp = parseInt(ipValue[i]/255);
			ipValue[i] = (ipValue[i] % 255);

			if(tmp==0)
			{
				break;
			}else
			{
				tmp --;
				up ++;
				ipValue[i]++;
				ipValue[i] = (255 * (tmp)) + ipValue[i];
			}
		}
		ipValue[i-1] = ipValue[i-1]+up;
	}

	//create A class -> up
	if(up >= 1)
	{
		tmp = parseInt(ipValue[1]/255);
		if(tmp >= 1)
		{
			ipValue[0] = (ipValue[0]+tmp);
		}
	}
	return ipValue;
}

function initEndIP()
{
	var objFullIP = document.formDhcpSet.dhcpEndIp;
	var ipValue1 = parseInt(document.formDhcpSet.start1.value);
	var ipValue2 = parseInt(document.formDhcpSet.start2.value);
	var ipValue3 = parseInt(document.formDhcpSet.start3.value);
	var ipValue4 = parseInt(document.formDhcpSet.start4.value);

	//create end ip!!!
	var ipValue = new Array(ipValue1,ipValue2, ipValue3, ipValue4);

	ipValue = endIP(ipValue);

	objFullIP.value = ipValue[0]+"."+ipValue[1]+"."+ipValue[2]+"."+ipValue[3];
	document.getElementById("td_dhcp_end").innerHTML = objFullIP.value;
	return true;
}

function dhcp_info_init(xmlDoc)
{
	commonInit();
	SetButtons( ["btn_dhcp_set", "btn_dhcp_cancle", "btn_staticLease_add" ,"btn_staticLease_del", "btn_staticLease_delall", "refresh", "save"] );
	dhcpEnableGroup = ["tr_dhcp_start", "tr_dhcp_end","tr_dhcp_max", "tr_dhcp_subnetmask", "tr_dhcp_gateway", "tr_dhcp_pridns", "tr_dhcp_secdns","tr_dhcp_lease"];


	var dhcpServerEnable = getDataFromTag(xmlDoc,"dhcpEnable");
	var publicEggIPRangeEnable = getDataFromTag(xmlDoc,"IPrangeEnable");
	var dhcpStart = getDataFromTag(xmlDoc,"dhcpStart");
	var leasetime = parseInt(getDataFromTag(xmlDoc,"leasetime"));
	var lan_netmask = getDataFromTag(xmlDoc,"lan_netmask");
	var lan_gateway = getDataFromTag(xmlDoc,"lan_gateway");

	var dhcpStartArray = dhcpStart.split('.');
	dhcpNetMaskArray = lan_netmask.split('.');
	dhcpGatewayArray = lan_gateway.split('.');


	if(dhcpServerEnable == "1")
	{
		document.formDhcpSet.dhcpServer.checked = true;
	}
	else
	{
		document.formDhcpSet.dhcpServer.checked = false;
	}
	updateState_dhcp();

	if(publicEggIPRangeEnable == "1")
	{
		 document.formDhcpSet.IPRangeLimitEnable.checked = true;
	}
	else
	{
		 document.formDhcpSet.IPRangeLimitEnable.checked = false;
	}

	updateState_publicEgglimit();

	for(i=0; i<4; i++)
	{
		document.getElementById("start"+(i+1)).value = dhcpStartArray[i];
		if(dhcpNetMaskArray[i]=="255")
		{
			document.getElementById("start"+(i+1)).disabled=true;
			document.getElementById("ip_addr"+(i+1)).value = dhcpGatewayArray[i];
			document.getElementById("ip_addr"+(i+1)).disabled=true;
		}
	}

	initStartIP();
	initEndIP();
	document.formDhcpSet.dhcpLease.value = leasetime;
}

function change_state(){

	if(document.formBasicSetup.relay_enable.checked == true){
		document.formBasicSetup.relay_mac.disabled = false;
	}else if(document.formBasicSetup.relay_disable.checked == true){
		document.formBasicSetup.relay_mac.disabled = true;
	}
}

function enable()
{
	document.formBasicSetup.relay_enable.checked = true;
	document.formBasicSetup.relay_disable.checked = false;
	change_state();
}

function disable(){
	document.formBasicSetup.relay_enable.checked = false;
	document.formBasicSetup.relay_disable.checked = true;
	change_state();
}

function update_state(xmlDoc)
{
	var enable_relay = getDataFromTag(xmlDoc,"enable_relay");
	var lan_dhcp = getDataFromTag(xmlDoc,"lan_dhcp");

	if(enable_relay == "1")
	{
		enable();
	}
	else
	{
		disable();
	}

	change_state();
}

function page_init()
{
	var url = "/cgi-bin/getinfo.cgi?Command=basic_setup";
	XHRGet(getinfohandle, url, true);
}

function ip_range(ipValue1, ipValue2, ipValue3, ipValue4){
	var regex=/^[0-9]+$/;
	if(!ipValue1.value.match(regex))
	{
		alert("숫자만 입력 가능합니다.");
		ipValue1.value="";
		ipValue1.select();
		return false;
	}

	if(ipValue2.value != "")
	{
		if(!ipValue2.value.match(regex))
		{
			alert("숫자만 입력 가능합니다.");
			ipValue2.value="";
			ipValue2.select();
			return false;
		}
	}

	if(ipValue3.value != "")
	{
		if(!ipValue3.value.match(regex))
		{
			alert("숫자만 입력 가능합니다.");
			ipValue3.value="";
			ipValue3.select();
			return false;
		}
	}

	if(ipValue4.value != "")
	{
		if(!ipValue4.value.match(regex))
		{
			alert("숫자만 입력 가능합니다.");
			ipValue4.value="";
			ipValue4.select();
			return false;
		}
	}

	if(ipValue1.value < parseInt(startIPRangeLimit[0]) || parseInt(endIPRangeLimit[0]) < ipValue1.value)
	{
		alert(alertStartRangeText);
		ipValue1.value="";
		ipValue1.select();
		return false;
	}
	else
	{
		if(ipValue2.value < parseInt(startIPRangeLimit[1]) || parseInt(endIPRangeLimit[1]) < ipValue2.value)
		{
			alert(alertStartRangeText);
			ipValue2.value="";
			ipValue2.select();
			return false;
		}
		else
		{
			if(ipValue3.value < parseInt(startIPRangeLimit[2]) || parseInt(endIPRangeLimit[2]) < ipValue3.value)
			{
				alert(alertStartRangeText);
				ipValue3.value="";
				ipValue3.select();
				return false;
			}
			else
			{
				if(ipValue4.value < parseInt(startIPRangeLimit[3]) || parseInt(endIPRangeLimit[3]) < ipValue4.value)
				{
					alert(alertStartRangeText);
					ipValue4.value="";
					ipValue4.select();
					return false;
				}
			}
		}
	}
	return true;
}
function updateStartIP_range()
{
	var objFullIP = document.formDhcpSet.dhcpStartIp;
	var ipValue1 = document.formDhcpSet.start1;
	var ipValue2 = document.formDhcpSet.start2;
	var ipValue3 = document.formDhcpSet.start3;
	var ipValue4 = document.formDhcpSet.start4;

	objFullIP.value = ipValue1.value+"."+ipValue2.value+"."+ipValue3.value+"."+ipValue4.value;

	if(!ip_range(ipValue1,ipValue2,ipValue3,ipValue4))
	{
		return false;
	}

	if(!initEndIP())
	{
		return false;
	}
	return true;
}

function ip_end_range_check(ipValue1, ipValue2, ipValue3, ipValue4)
{
	if(ipValue1 < parseInt(startIPRangeLimit[0]) || parseInt(endIPRangeLimit[0]) < ipValue1)
	{
		return false;
	}
	else
	{
		if(ipValue2 < parseInt(startIPRangeLimit[1]) || parseInt(endIPRangeLimit[1]) < ipValue2)
		{
			return false;
		}
		else
		{
			if(ipValue3 < parseInt(startIPRangeLimit[2]) || parseInt(endIPRangeLimit[2]) < ipValue3)
			{
				return false;
			}
			else
			{
				if(ipValue4 < parseInt(startIPRangeLimit[3]) || parseInt(endIPRangeLimit[3]) < ipValue4)
				{
					return false;
				}
			}
		}
	}

	return true;
}

function ip_subnet_check(ipValue1, ipValue2, ipValue3, ipValue4)
{
	var	minIPlimit = new Array();
		minIPlimit[0] = dhcpGatewayArray[0] & dhcpNetMaskArray[0];
		minIPlimit[1] = dhcpGatewayArray[1] & dhcpNetMaskArray[1];
		minIPlimit[2] = (dhcpGatewayArray[2] & dhcpNetMaskArray[2])+1;
		minIPlimit[3] = (dhcpGatewayArray[3] & dhcpNetMaskArray[3])+1;

	var	maxIPlimit = new Array();
		maxIPlimit[0] = dhcpGatewayArray[0] | (~dhcpNetMaskArray[0]+256);
		maxIPlimit[1] = dhcpGatewayArray[1] | (~dhcpNetMaskArray[1]+256);
		maxIPlimit[2] = (dhcpGatewayArray[2] | (~dhcpNetMaskArray[2]+256))-1;
		maxIPlimit[3] = (dhcpGatewayArray[3] | (~dhcpNetMaskArray[3]+256))-1;

	if(ipValue1 < parseInt(minIPlimit[0]) || parseInt(maxIPlimit[0]) < ipValue1)
	{
			return false;
	}
	else
	{
		if(ipValue2 < parseInt(minIPlimit[1]) || parseInt(maxIPlimit[1]) < ipValue2)
		{
			return false;
		}
		else
		{
			if(ipValue3 < parseInt(minIPlimit[2]) || parseInt(maxIPlimit[2]) < ipValue3)
			{
				return false;
			}
			else
			{
				if(ipValue4 < parseInt(minIPlimit[3]) || parseInt(maxIPlimit[3]) < ipValue4)
				{
					return false;
				}
			}
		}
	}
	return true;
}
function MaxLimitCheck()
{
	var objMax=document.formDhcpSet.maxip;
	var regex=/^[0-9]+$/;
	if(objMax.value=="")
	{
		alert("설정값을 입력해주세요.");
		objMax.select();
		return false;
	}

	if(!objMax.value.match(regex))
	{
		alert("숫자만 입력 가능합니다.");
		objMax.select();
		return false;
	}

	if( parseInt(objMax.value) < 1 || 1016 < parseInt(objMax.value) )
	{
		alert("다시 입력해 주세요. (1~1016)");
		objMax.select();
		return false;
	}
	return true;
}

function DhcpsetFormCheck()
{
	initStartIP();

	var objStartIP = document.formDhcpSet.dhcpStartIp;
	var objConnectMax = document.formDhcpSet.maxip;
	var objLeaseTime =document.formDhcpSet.dhcpLease;
	var ipValue1 = parseInt(document.formDhcpSet.start1.value);
	var ipValue2 = parseInt(document.formDhcpSet.start2.value);
	var ipValue3 = parseInt(document.formDhcpSet.start3.value);
	var ipValue4 = parseInt(document.formDhcpSet.start4.value);
	var ipValue = new Array(ipValue1,ipValue2, ipValue3, ipValue4);

	if(objStartIP.value == "")
	{
		alert("IP 주소가 설정되어 있지 않습니다.");
		return false;
	}

	if ( !checkIPAddr(objStartIP) )
	{
		alert("IP 주소가 올바르지 않습니다.");
		return false;
	}

	if(!MaxLimitCheck())
	{
		return false;
	}

	if(objLeaseTime.value=="")
	{
		alert("임대 시간이 설정되어 있지 않습니다.");
		objLeaseTime.select();
		return false;
	}

	var regex=/^[0-9]+$/;
	if(!objLeaseTime.value.match(regex))
	{
		alert("숫자만 입력 가능합니다.");
		objLeaseTime.select();
		return false;
	}

	if( parseInt(objLeaseTime.value) < 2 || parseInt(objLeaseTime.value) > 1440)
	{
		alert("IP 임대 시간의 범위는 2 ~ 1440분 사이 입니다.");
		objLeaseTime.select();
		return false;
	}

	ipValue=endIP(ipValue);
	if(!ip_subnet_check(ipValue[0],ipValue[1],ipValue[2],ipValue[3]))
	{
		alert("할당된 DHCP 범위가 서브넷을 벗어납니다.");
		return false;
	}

	if(!ip_end_range_check(ipValue[0],ipValue[1],ipValue[2],ipValue[3]))
	{
		alert("최대 허용 접속자 수가 DHCP 할당 범위를 벗어납니다. 다시 입력해 주시기 바랍니다.");
		return false;
	}

	return true;
}

function maketable(xmlDoc)
{
	var static_table = getDataFromTag(xmlDoc,"static_table");
	if(static_table == "1")
	{

		var table_ip_set = getDataFromTag(xmlDoc,"table_ip_set");
		var table_mac_set = getDataFromTag(xmlDoc,"table_mac_set");
		row_num = parseInt(getDataFromTag(xmlDoc,"table_row_num"));
		var table_ip_array = table_ip_set.split(',');
		var table_mac_array = table_mac_set.split(',');

		var table_name = document.getElementById("StaticListTable");

		for(i=0; i<row_num; i++)
		{
			trs = document.createElement("tr");

			td = document.createElement("td");
			element = document.createElement("input");
			element.setAttribute("type","checkbox");
			element.setAttribute("id","element"+i);
			td.innerHTML = i+1;
			td.appendChild(element);
			trs.appendChild(td);

			td = document.createElement("td");
			td.setAttribute("id","staticip"+i);
			td.innerHTML = table_ip_array[i];
			trs.appendChild(td);

			td = document.createElement("td");
			td.setAttribute("id","staticmac"+i);
			td.innerHTML = table_mac_array[i];
			trs.appendChild(td);

			table_name.appendChild(trs);
		}
	}
}

function set_value(xmlDoc)
{
	document.getElementById("lan_ipaddr").value = getDataFromTag(xmlDoc,"lan_ipaddr");
	document.getElementById("lan_netmask").value = getDataFromTag(xmlDoc,"lan_netmask");
	document.getElementById("dhcpDns1").value = getDataFromTag(xmlDoc,"DNS1");
	document.getElementById("dhcpDns2").value = getDataFromTag(xmlDoc,"DNS2");
	document.getElementById("maxip").value = getDataFromTag(xmlDoc,"maxassign");
}

function getinfohandle(xmlDoc)
{
	if (hasError(xmlDoc))
	{
		showError(xmlDoc);
		return;
	}
	document.getElementById("main_page").style.display="";
	set_value(xmlDoc);
	dhcp_info_init(xmlDoc);
	update_state(xmlDoc);
	maketable(xmlDoc);

	var relay_macaddr = getDataFromTag(xmlDoc,"lan_macaddr");
	if(relay_macaddr == "-"){
		document.getElementById("relay_mac").value = "00:17:C3:1F:86:2A";
	}else{
		document.getElementById("relay_mac").value = relay_macaddr;
	}
    parent.callResize();
}

function sync_netmask() // Sync subnet mask with lanIP
{

	var lanip = document.getElementById("lan_ipaddr").value;
	var classA  = "255.0.0.0";
	var classB  = "255.255.0.0";
	var classB2 = "255.240.0.0";
	var lanAry;
	
	if ( lanip == "" )	{
			document.getElementById("lan_netmask").value = ""
			return false;
	}else {
	
		if ( !checkIPAddr( document.formBasicSetup.lan_ipaddr ) )
		{
			document.getElementById("lan_netmask").value = ""
			return false;
		}
	}
	
/* standard
	A : 0~127   255.0.0.0
	B : 128~191 255.255.0.0 
	C : 192~223 255.255.255.0
	D : 224~239 ""
	E : 240~255 ""
*/		
	lanAry = lanip.split('.');
	
	if ( lanAry[0] > 0 && lanAry[0] <= 127 )	{	
	
		document.getElementById("lan_netmask").value = classA;
	}
	else if ( lanAry[0] >= 128 && lanAry[0] <= 254 ) {
	
		if( lanAry[0] == 172 && lanAry[1] == 30 ) {
		
			document.getElementById("lan_netmask").value = classB2;
		}
		else{
		
			document.getElementById("lan_netmask").value = classB;
		}
	}
}



function RuleFormCheck()
{
	if( document.formBasicSetup.relay_enable.checked == true ){
		if ( document.formBasicSetup.relay_mac.value == "" )
		{
			alert( "Relay MAC 주소를 입력해 주세요" );
			return false;
		}

		if ( document.formBasicSetup.relay_mac.value != "" )
		{
			if ( !checkMac( document.formBasicSetup.relay_mac ) )
			{
				alert( "유효하지 않은 Relay MAC 주소 입니다." );
				return false;
			}
		}
		document.formBasicSetup.relay_value.value = "1";
	}
	else
	{
		document.formBasicSetup.relay_value.value = "0";
	}

	// LAN IP check
	if ( document.formBasicSetup.lan_ipaddr.value == "" )
	{
		alert( "IP 주소를 입력해 주세요" );
		return false;
	}
	if ( document.formBasicSetup.lan_ipaddr.value != "" )
	{
		if ( !checkIPAddr( document.formBasicSetup.lan_ipaddr ) )
		{
			alert( "유효하지 않은 IP 주소 입니다." );
			return false;
		}
	}
	
		
	// SUBNETMASK check
	if ( document.formBasicSetup.lan_netmask.value == "" )
	{
		alert( "서브넷 마스크를 입력해 주세요" );
		return false;
	}
	if ( document.formBasicSetup.lan_netmask.value != "" )
	{
		if ( !checkSubnetMask( document.formBasicSetup.lan_netmask ) )
		{
			alert( "유효하지 않은 서브넷 마스크 입니다." );
			return false;
		}
	}
	
	return true;
}

function CheckStaticDhcpRange()
{
	var ipValue1 = document.formStaticLeaseAdd.ip_addr1;
	var ipValue2 = document.formStaticLeaseAdd.ip_addr2;
	var ipValue3 = document.formStaticLeaseAdd.ip_addr3;
	var ipValue4 = document.formStaticLeaseAdd.ip_addr4;

	var startValue1 = document.formDhcpSet.start1.value;
	var startValue2 = document.formDhcpSet.start2.value;
	var startValue3 = document.formDhcpSet.start3.value;
	var startValue4 = document.formDhcpSet.start4.value;

	var endValue = document.formDhcpSet.dhcpEndIp.value;
	var endArray = endValue.split('.');

	if(parseInt(ipValue1.value) < parseInt(startValue1) || parseInt(endArray[0]) < parseInt(ipValue1.value) )
	{
		ipValue1.value="";
		ipValue1.select();
		return false;
	}
	else
	{
		var inputNumber=((ipValue2.value*256)*256)+(ipValue3.value*256)+parseInt(ipValue4.value);
		var startNumber=((startValue2*256)*256)+(startValue3*256)+parseInt(startValue4);
		var endNumber=((parseInt(endArray[1])*256)*256)+(parseInt(endArray[2])*256)+parseInt(endArray[3]);

		if( parseInt(inputNumber) < parseInt(startNumber) || parseInt(endNumber) < parseInt(inputNumber) )
		{
			return false;
		}
	}
	return true;
}

function check_redundancy()
{
	for(i=0; i < document.getElementById("StaticListTable").rows.length-2; i++)
	{
		//ip
		//if( document.getElementsByName("staticip")[i].innerHTML == document.formStaticLeaseAdd.ip_addr.value)
		if( document.getElementById("staticip"+i).innerHTML == document.formStaticLeaseAdd.ip_addr.value)
		{
			return false;
		}
		//mac
		if( document.getElementById("staticmac"+i).innerHTML == document.formStaticLeaseAdd.mac.value)
		{
			return false;
		}
	}
	return true;
}

function initStaticDHCP(){
	var objFullIP = document.formStaticLeaseAdd.ip_addr;
	var ipValue1 = document.formStaticLeaseAdd.ip_addr1;
	var ipValue2 = document.formStaticLeaseAdd.ip_addr2;
	var ipValue3 = document.formStaticLeaseAdd.ip_addr3;
	var ipValue4 = document.formStaticLeaseAdd.ip_addr4;

	objFullIP.value = ipValue1.value+"."+ipValue2.value+"."+ipValue3.value+"."+ipValue4.value;
}

function StaticDhcpFormCheck(){

	initStaticDHCP();

	var objStaticIp = document.formStaticLeaseAdd.ip_addr;
	var objStaticMac = document.formStaticLeaseAdd.mac;

	var ipValue2 = document.formStaticLeaseAdd.ip_addr2;

	var ipValue3 = document.formStaticLeaseAdd.ip_addr3;
	var ipValue4 = document.formStaticLeaseAdd.ip_addr4;

	//static Ip
	if(ipValue2=="" || ipValue3.value == "" || ipValue4.value == "")
	{
		alert("IP 주소가 설정되어 있지 않습니다.");
		return false;
	}
	if ( !checkIPAddr(objStaticIp) )
	{
		alert("IP 주소가 올바르지 않습니다.");
		return false;
	}
	if( !CheckStaticDhcpRange() )
	{
		alert("DHCP 할당 범위가 잘못되었습니다.");
		return false;
	}
	//mac
	if ( objStaticMac.value == "" )
	{
		alert("MAC 주소가 설정되어 있지 않습니다.");
		return false;
	}
	if ( !checkMac(objStaticMac) )
	{
		objStaticMac.select();
		alert("MAC 주소가 올바르지 않습니다.");
		return false;
	}

	if ( !check_redundancy() )
	{
		alert( "이미 규칙이 등록되어 있습니다. 다른 규칙을 입력하십시오." );
		return false;
	}

	return true;
}

function submit_apply_staticLeaseAdd(){
	if (StaticDhcpFormCheck())
	{
		EnableButtons( false );
		TimeoutReload( TIMEOUT_DEFAULT_TIME );

		var url = "/cgi-bin/netset.cgi";
		var param = "Command=StaticLeaseAdd"
					+"&"
					+serializeFormElement("formStaticLeaseAdd");

		XHRPost(null, url, param, true);
	}
}

function submit_apply_dhcpSet(){
	if (DhcpsetFormCheck())
	{
		if( confirm("기능 적용을 위해  단말 재부팅이 필요합니다. 재부팅 하시겠습니까?")){
			document.formDhcpSet.reboot_allow.value = "1";

			EnableButtons( false );
			var url = "/cgi-bin/netset.cgi";
			var param = "Command=dhcpSet"
						+"&"
						+serializeFormElement("formDhcpSet");

			XHRPost(null, url, param, true);
			setTimeout(function(){
			location.href = "../web_noti.html";
			},2000);

		}
	}
}

function submit_apply_basic_setup()
{
	if( RuleFormCheck())
	{
		if( confirm("기능 적용을 위해  단말 재부팅이 필요합니다. 재부팅 하시겠습니까?"))
		{
			document.formBasicSetup.reboot_allow.value = "1";

			EnableButtons( false );

			var url = "/cgi-bin/netset.cgi";
			var param = "Command=BasicSetup"
						+"&"
						+serializeFormElement("formBasicSetup");

			XHRPost(null, url, param, true);
			setTimeout(function(){
			location.href = "../web_noti.html";
			},2000);
		}
	}
}

function deleteAllClick()
{
	if (confirm("모든 DHCP 규칙을 삭제하시겠습니까?" ))
	{
		document.getElementById( "submit_type" ).value = "1";
		document.getElementById( "table_num" ).value = row_num;

		EnableButtons( false );
		TimeoutReload( TIMEOUT_DEFAULT_TIME );

		var url = "/cgi-bin/netset.cgi";
		var param = "Command=deleteStatic"
					+"&"
					+serializeFormElement("formStaticLeaseDel");

		XHRPost(null, url, param, true);
	}
}

function deleteClick()
{
	var count = row_num;
	var pass = false;

	for (k=0; k<count; k++)
	{
		var tmp = document.getElementById("element"+k);
		if (tmp.checked == true)
		{
			pass = true;
			tmp.value="1";
		}
		else
		{
			tmp.value="0";
		}
	}

	if (!pass)
	{
		alert("DHCP 규칙이 선택되지 않았습니다.");
		return false;
	}
	return true;
}

function submit_apply_deleteClick()
{
	if(deleteClick())
	{
		document.getElementById( "submit_type" ).value = "0";
		document.getElementById( "table_num" ).value = row_num;

		EnableButtons( false );
		TimeoutReload( TIMEOUT_DEFAULT_TIME );

		var url = "/cgi-bin/netset.cgi";
		var param = "Command=deleteStatic"
					+"&"
					+serializeFormElement("formStaticLeaseDel");

		XHRPost(null, url, param, true);
	}
}
function init()
{
	auth();
}
