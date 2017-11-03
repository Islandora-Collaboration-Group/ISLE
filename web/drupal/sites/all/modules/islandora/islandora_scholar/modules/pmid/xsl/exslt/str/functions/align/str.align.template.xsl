<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:str="http://exslt.org/strings"
                extension-element-prefixes="str">

<xsl:template name="str:align">
	<xsl:param name="string" select="''" />
   <xsl:param name="padding" select="''" />
   <xsl:param name="alignment" select="'left'" />
   <xsl:variable name="str-length" select="string-length($string)" />
   <xsl:variable name="pad-length" select="string-length($padding)" />
   <xsl:choose>
      <xsl:when test="$str-length >= $pad-length">
         <xsl:value-of select="substring($string, 1, $pad-length)" />
      </xsl:when>
      <xsl:when test="$alignment = 'center'">
         <xsl:variable name="half-remainder" select="floor(($pad-length - $str-length) div 2)" />
         <xsl:value-of select="substring($padding, 1, $half-remainder)" />
         <xsl:value-of select="$string" />
         <xsl:value-of select="substring($padding, $str-length + $half-remainder + 1)" />
      </xsl:when>
      <xsl:when test="$alignment = 'right'">
         <xsl:value-of select="substring($padding, 1, $pad-length - $str-length)" />
         <xsl:value-of select="$string" />
      </xsl:when>
      <xsl:otherwise>
         <xsl:value-of select="$string" />
         <xsl:value-of select="substring($padding, $str-length + 1)" />
      </xsl:otherwise>
   </xsl:choose>
</xsl:template>

</xsl:stylesheet>