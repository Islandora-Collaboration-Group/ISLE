<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <!-- 
        This document transforms a Form Definition of version 1 to version 2. Version 2 just adds the version number attribute.
    -->
    <xsl:output omit-xml-declaration="yes" indent="yes" method="xml"/>
    <xsl:strip-space elements="*"/>
    <xsl:template match="/definition">
        <definition xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" version="3">
            <xsl:copy-of select="properties"/>
            <xsl:apply-templates select="form"/>
        </definition>
    </xsl:template>
  <xsl:template match="@name[parent::element]">
    <xsl:attribute name="name">
      <!-- Meta characters not allowed: [!"#$%&'()*+,./:;<=>?@[\]^`{|}~]
          XML prevents &<>'" from being used anyway so we don't need to translate them.
          Convert all metacharacters to '-' characters.
        -->
      <xsl:value-of select="translate(.,'!#$%()*+,./:;=?@[\]^`{|}~','-------------------------')"/>
    </xsl:attribute>
  </xsl:template>
  <xsl:template match="@*|node()">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
