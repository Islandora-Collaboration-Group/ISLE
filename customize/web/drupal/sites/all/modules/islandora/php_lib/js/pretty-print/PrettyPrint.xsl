<?xml version="1.0" encoding="UTF-8"?>

<!--
    Document   : PrettyPrint.xsl
    Created on : March 23, 2011, 3:19 PM
    Author     : nbanks
    Description:
        To prettify outputed XML.
-->

<xsl:stylesheet version="1.0"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output omit-xml-declaration="yes" indent="yes" method="xml"/>
    <xsl:strip-space elements="*"/>
    <xsl:template match="/">  
      <xsl:copy-of select="node()"/>
    </xsl:template>
</xsl:stylesheet>
