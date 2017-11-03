<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:exsl="http://exslt.org/functions"
                xmlns:set="http://exslt.org/sets"
                extension-element-prefixes="exsl"
                exclude-result-prefixes="set">

<exsl:function name="set:difference">
   <xsl:param name="nodes1" select="/.." />
   <xsl:param name="nodes2" select="/.." />
   <exsl:result select="$nodes1[count(.|$nodes2) != count($nodes2)]" />
</exsl:function>

</xsl:stylesheet>