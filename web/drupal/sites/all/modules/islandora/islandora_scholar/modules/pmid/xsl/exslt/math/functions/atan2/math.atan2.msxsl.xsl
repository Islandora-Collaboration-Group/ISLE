<?xml version='1.0'?>
<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:msxsl="urn:schemas-microsoft-com:xslt"
	xmlns:math="http://exslt.org/math"
	extension-element-prefixes="math msxsl">

<msxsl:script language="JavaScript" implements-prefix="math">
<![CDATA[
function atan2(arg1, arg2){ return Math.atan2(arg1, arg2);}
]]>
</msxsl:script>

</xsl:stylesheet>