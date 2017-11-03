/**
<doc:module date="2001-06-12">
   <doc:module>exslt:regular-expressions</doc:module>
   <doc:name>regExp</doc:name>
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
	<doc:name>match</doc:name>
	<doc:version>1.0</doc:version>
	<doc:return type="node-set" />
	<doc:args>
		<doc:arg name="context" type="node-set" default="null" optional="no"></doc:arg>
		<doc:arg name="regExp" type="string" default="''" optional="no"></doc:arg>
		<doc:arg name="flags" type="string" default="''" optional="no"></doc:arg>
	</doc:args>
</doc:function>
**/
function match(ctx, re, flags){
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
			return ctx;// empty set
		}
	}else{
		ipString = ctx;
	}
	var tdom = new ActiveXObject("MSXML2.FreeThreadedDOMDocument");
	var od = tdom.createNode(1, 'myRE', '');
	var oRe = new RegExp(re, flags);

	var parts,tmp='';
	if ((parts = ipString.match(oRe)) != null){
		//return parts.length;
		for (var i=0; i < parts.length; i++){
			tmp+=parts[i]+';';
			var mn = tdom.createNode(1, 'match', '');
			var tn = tdom.createTextNode(parts[i]);
			mn = od.appendChild(mn);
			tn = mn.appendChild(tn);
		}
		//return tmp
		return od.selectNodes("*");
	}else{
		return od.selectNodes("*");
	}
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