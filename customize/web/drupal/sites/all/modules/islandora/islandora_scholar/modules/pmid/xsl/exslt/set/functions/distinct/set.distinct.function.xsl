<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:exsl="http://exslt.org/functions"
                xmlns:set="http://exslt.org/sets"
                extension-element-prefixes="exsl"
                exclude-result-prefixes="set">

<exsl:function name="set:distinct">
   <xsl:param name="nodes" select="/.." />
   <xsl:choose>
      <xsl:when test="not($nodes)">
         <exsl:result select="/.." />
      </xsl:when>
      <xsl:otherwise>
         <xsl:variable name="distinct" 
                       select="set:distinct($nodes[position() > 1])" />
         <exsl:result select="$distinct | $nodes[1][. != $distinct]" />
      </xsl:otherwise>
   </xsl:choose>
</exsl:function>

</xsl:stylesheet>