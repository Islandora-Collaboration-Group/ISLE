<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <!-- 
        This document transforms a Form Definition of version 1 to version 2. Version 2 just adds the version number attribute.
    -->
    <xsl:output omit-xml-declaration="yes" indent="yes" method="xml"/>
    <xsl:strip-space elements="*"/>
    <xsl:template match="/definition">
        <definition xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" version="2">
            <xsl:copy-of select="properties"/>
            <xsl:copy-of select="form"/>
        </definition>
    </xsl:template>
</xsl:stylesheet>
