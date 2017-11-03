/**
<doc:module date="2001-06-16">
   <doc:module>exslt:strings</doc:module>
   <doc:name>strings</doc:name>
   <doc:version>1.0</doc:version>
   <doc:language>exslt:javascript</doc:language>
   <doc:meta>
      <doc:author email="chris@bayes.co.uk" url="http://www.bayes.co.uk">Chris Bayes</doc:author>
      <doc:summary>Implementation of exslt:strings</doc:summary>
      <doc:todo></doc:todo>
   </doc:meta>
</doc:module>
**/
/**
<doc:function date="2001-06-16">
        <doc:name>align</doc:name>
        <doc:version>1.0</doc:version>
        <doc:return type="string" />
        <doc:args>
                <doc:arg name="string" type="string" default="''" optional="no"></doc:arg>
                <doc:arg name="padding" type="string" default="''" optional="no"></doc:arg>
                <doc:arg name="alignment" type="string" default="''" optional="no">{ left | right |
center }</doc:arg>
        </doc:args>
</doc:function>
**/
function align(ctx, padding, alignment){
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
                        return false;
                }
        }else{
                ipString = ctx;
        }
        if (ipString.length > padding.length){
                return ipString.substr(0, padding.length);
        }else{
                switch(alignment){
                        case 'center':
                                return padding.substr(0, padding.length/2 - ipString.length/2) + 
                                           ipString + 
                                           padding.substr(padding.length/2 + ipString.length/2);
                        case 'right':
                                return padding.substr(0, padding.length - ipString.length) +
                                           ipString;
                        default:
                                return ipString + padding.substr(ipString.length);
                }
        }
}
