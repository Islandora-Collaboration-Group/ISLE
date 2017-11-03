<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:str="http://exslt.org/strings"
                xmlns:func="http://exslt.org/functions"
                extension-element-prefixes="str func">

<func:function name="str:padding">
	<xsl:param name="length" select="0" />
   <xsl:param name="chars" select="' '" />
   <xsl:choose>
      <xsl:when test="not($length) or not($chars)" />
      <xsl:otherwise>
         <xsl:variable name="string" 
                       select="concat($chars, $chars, $chars, $chars, $chars, 
                                      $chars, $chars, $chars, $chars, $chars)" />
         <xsl:choose>
            <xsl:when test="string-length($string) >= $length">
               <func:result select="substring($string, 1, $length)" />
            </xsl:when>
            <xsl:otherwise>
               <func:result select="str:padding($length, $string)" />
            </xsl:otherwise>
         </xsl:choose>
      </xsl:otherwise>
   </xsl:choose>
</func:function>

</xsl:stylesheet>