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
	<doc:name>concat</doc:name>
	<doc:version>1.0</doc:version>
	<doc:return type="string" />
	<doc:args>
		<doc:arg name="context" type="context" default="null" optional="no"></doc:arg>
	</doc:args>
</doc:function>
**/
function concat(ctx){
    var ipString = "";
	if (typeof(ctx) == "object"){
		if (ctx.length){
			for (var i=0; i < ctx.length; i++){
				ctxN  = ctx.item(i);
				if (ctxN.nodeType == 1){
					ipString +=  _wander(ctxN);
				}
				if (ctxN.nodeType == 2){
					ipString += ctxN.nodeValue;
				}
			}
		}else{
			return '';
		}
	}else{
		ipString = ctx;
	}
    return ipString
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