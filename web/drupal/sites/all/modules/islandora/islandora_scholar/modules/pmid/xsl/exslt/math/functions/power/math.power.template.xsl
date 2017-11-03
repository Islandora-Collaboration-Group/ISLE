<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:math="http://exslt.org/math"
                extension-element-prefixes="math">

<xsl:template name="math:power">
  <xsl:param name="base" select="0" />
  <xsl:param name="power" select="1" />
  <xsl:choose>
    <xsl:when test="$power &lt; 0 or contains(string($power), '.')">
      <xsl:message terminate="yes">
        The XSLT template math:power doesn't support negative or
        fractional arguments.
      </xsl:message>
      <xsl:text>NaN</xsl:text>
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="math:_power">
        <xsl:with-param name="base" select="$base" />
        <xsl:with-param name="power" select="$power" />
        <xsl:with-param name="result" select="1" />
      </xsl:call-template>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="math:_power">
  <xsl:param name="base" select="0" />
  <xsl:param name="power" select="1" />
  <xsl:param name="result" select="1" />
  <xsl:choose>
    <xsl:when test="$power = 0">
      <xsl:value-of select="$result" />
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="math:_power">
        <xsl:with-param name="base" select="$base" />
        <xsl:with-param name="power" select="$power - 1" />
        <xsl:with-param name="result" select="$result * $base" />
      </xsl:call-template>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

</xsl:stylesheet>