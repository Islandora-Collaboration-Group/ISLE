<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" xmlns:b="info:fedora/fedora-system:def/dsCompositeModel#" >

    <xsl:output method="text" indent="yes" encoding="UTF-8"/>

    <xsl:strip-space elements="*"/>
    <xsl:template match="/">{<xsl:apply-templates />}</xsl:template>


    <xsl:template match="//b:dsTypeModel">

        <xsl:text>"</xsl:text>
        <xsl:value-of select="@ID"/>
        <xsl:text>":"</xsl:text>
        <xsl:value-of select="b:form/@MIME"/>
        <xsl:text>"</xsl:text>

        <xsl:if test="position()!=last()">,</xsl:if>


    </xsl:template>

</xsl:stylesheet>