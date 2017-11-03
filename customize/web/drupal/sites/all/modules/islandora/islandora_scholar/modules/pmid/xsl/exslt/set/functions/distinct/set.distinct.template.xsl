<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:set="http://exslt.org/sets"
                exclude-result-prefixes="set">

<xsl:template name="set:distinct">
  <xsl:param name="nodes" select="/.."/>
  <xsl:param name="distinct" select="/.."/>
  <xsl:choose>
    <xsl:when test="$nodes">
      <xsl:call-template name="set:distinct">
      <xsl:with-param name="distinct" select="$distinct | $nodes[1][not(. = $distinct)]"/>
      <xsl:with-param name="nodes" select="$nodes[position() > 1]"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
      <xsl:apply-templates select="$distinct" mode="set:distinct"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="node()|@*" mode="set:distinct">
   <xsl:copy-of select="." />
</xsl:template>

</xsl:stylesheet>