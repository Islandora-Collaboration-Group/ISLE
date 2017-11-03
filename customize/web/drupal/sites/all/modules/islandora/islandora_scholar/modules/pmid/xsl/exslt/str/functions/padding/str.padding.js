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
        <doc:name>padding</doc:name>
        <doc:version>1.0</doc:version>
        <doc:return type="string" />
        <doc:args>
                <doc:arg name="length" type="number" default="''" optional="no"></doc:arg>
                <doc:arg name="chars" type="string" default="''" optional="no"></doc:arg>
        </doc:args>
</doc:function>
**/
function padding(num, str){
        var opString = "";
        var pad = " ";
        if (str != "" && str != null) pad = str;
        for (; opString.length < num;){
                opString += pad;
        }
        return opString.substr(0, num);
}