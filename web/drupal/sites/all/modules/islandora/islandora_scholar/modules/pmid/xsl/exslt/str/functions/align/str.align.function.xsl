<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:str="http://exslt.org/strings"
                xmlns:func="http://exslt.org/functions"
                extension-element-prefixes="str func">

<func:function name="str:align">
	<xsl:param name="string" select="''" />
   <xsl:param name="padding" select="''" />
   <xsl:param name="alignment" select="'left'" />
   <xsl:variable name="str-length" select="string-length($string)" />
   <xsl:variable name="pad-length" select="string-length($padding)" />
   <xsl:choose>
      <xsl:when test="$str-length >= $pad-length">
         <func:result select="substring($string, 1, $pad-length)" />
      </xsl:when>
      <xsl:when test="$alignment = 'center'">
         <xsl:variable name="half-remainder" select="floor(($pad-length - $str-length) div 2)" />
         <func:result select="concat(substring($padding, 1, $half-remainder), $string, substring($padding, $str-length + $half-remainder + 1))" />
      </xsl:when>
      <xsl:when test="$alignment = 'right'">
         <func:result select="concat(substring($padding, 1, $pad-length - $str-length), $string)" />
      </xsl:when>
      <xsl:otherwise>
         <func:result select="concat($string, substring($padding, $str-length + 1))" />
      </xsl:otherwise>
   </xsl:choose>
</func:function>

</xsl:stylesheet>