<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:math="http://exslt.org/math"
                xmlns:func="http://exslt.org/functions"
                extension-element-prefixes="func math">

<func:function name="math:power">
  <xsl:param name="base" select="0" />
  <xsl:param name="power" select="1" />
  <xsl:choose>
    <xsl:when test="$power &lt; 0 or contains(string($power), '.')">
      <xsl:message terminate="yes">
        The XSLT implementation of math:power() doesn't support
        negative or fractional arguments.
      </xsl:message>
      <func:result select="number('NaN')" />
    </xsl:when>
    <xsl:otherwise>
      <func:result select="math:_power($base, $power, 1)" />
    </xsl:otherwise>
  </xsl:choose>
</func:function>

<func:function name="math:_power">
  <xsl:param name="base" select="0" />
  <xsl:param name="power" select="1" />
  <xsl:param name="result" select="1" />
  <xsl:choose>
    <xsl:when test="$power = 0">
      <func:result select="$result" />
    </xsl:when>
    <xsl:otherwise>
      <func:result
        select="math:_power($base, $power - 1, $result * $base)" />
    </xsl:otherwise>
  </xsl:choose>
</func:function>

</xsl:stylesheet>