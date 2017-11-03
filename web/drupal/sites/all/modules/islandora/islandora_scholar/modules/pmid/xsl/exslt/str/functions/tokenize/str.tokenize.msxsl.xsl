<?xml version="1.0"?>
<xsl:stylesheet version="1.0" 
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:msxsl="urn:schemas-microsoft-com:xslt"
                xmlns:str="http://exslt.org/strings"
                extension-element-prefixes="str msxsl">

<msxsl:script language="JavaScript" implements-prefix="str">
<![CDATA[
/**
<doc:module date="2001-06-26">
   <doc:module>exslt:strings</doc:module>
   <doc:name>str</doc:name>
   <doc:version>1.0</doc:version>
   <doc:language>exslt:javascript</doc:language>
   <doc:meta>
      <doc:author email="chris@bayes.co.uk" url="http://www.bayes.co.uk">Chris Bayes</doc:author>
      <doc:summary>Implementation of EXSLT - RegExp (http://www.exslt.org/regExp)</doc:summary>
      <doc:todo></doc:todo>
   </doc:meta>
</doc:module>
**/
/**
<doc:function date="2001-06-26">
	<doc:name>tokenize</doc:name>
	<doc:version>2.0</doc:version>
	<doc:return type="node-set" />
	<doc:args>
		<doc:arg name="context" type="node-set" default="null" optional="no"></doc:arg>
		<doc:arg name="delimiters" type="string" default="&#x9;&#xA;&#xD;&#x20;" optional="yes"></doc:arg>
	</doc:args>
</doc:function>
**/
function tokenize(ctx, re){
	var ipString = "";
	if (typeof(ctx) == "object"){
		if (ctx.length){
			for (var i=0; i < 1; i++){
				ctxN  = ctx.item(i);
				if (ctxN.nodeType == 1){
					ipString +=  _wander(ctxN);
				}
				if (ctxN.nodeType == 2){
					ipString += ctxN.nodeValue;
				}
			}
		}else{
			return false;
		}
	}else{
		ipString = ctx;
	}
	var tdom = new ActiveXObject("MSXML2.FreeThreadedDOMDocument");
	var od = tdom.createNode(1, 'myRE', '');
	var mp = (re!=null&&re!=''?re:'\n\r\t ');
	var s = 0;
	for (var i=0; i < ipString.length; i++){
		for (var j=0; j < mp.length; j++){
			if (ipString.substr(i, 1) == mp.substr(j, 1)){
				var mn = tdom.createNode(1, 'token', '');
				var tn = tdom.createTextNode(ipString.substr(s, i-s));
				mn = od.appendChild(mn);
				tn = mn.appendChild(tn);
				s = i+1;
			}
		}
	}
	var tn = tdom.createTextNode(ipString.substr(s));
	var mn = tdom.createNode(1, 'token', '');
	mn = od.appendChild(mn);
	tn = mn.appendChild(tn);
	return od.selectNodes("*");
}
function   _wander(ctx){
	var retStr = "";
	for (var i=0; i < ctx.childNodes.length; i++){
		ctxN = ctx.childNodes[i];
		switch(ctxN.nodeType){
			case 1:
				retStr +=   _wander(ctxN);
				break;
			case 3:
				retStr += ctxN.nodeValue;
				break;
			default:
				break;
		}
	}
	return retStr;
}
]]>
</msxsl:script>
</xsl:stylesheet>