<?xml version="1.0" encoding="UTF-8"?>
<!--
    Document   : FixBibutilsModsOutput.xsl
    Created on : March 23, 2011, 3:19 PM
    Author     : nbanks
    Description:
    Transforms the invalid mods that Bibutils generates into valid v1.1 MODS.
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:mods="http://www.loc.gov/mods/v3">
    <xsl:output omit-xml-declaration="yes" indent="yes" method="xml"/>
    <xsl:strip-space elements="*"/>
    <xsl:template match="//mods:mods[1]/mods:mods | @* | node()[local-name() != 'modsCollection']">
        <xsl:copy>
            <xsl:apply-templates select="@* | node()"/>
        </xsl:copy>
    </xsl:template>
    <!-- Move the language into the proper languageTerm field -->
    <xsl:template match="mods:language[parent::mods:mods[1]]">
        <xsl:copy>
            <xsl:apply-templates select="@*"/>
            <xsl:element name="languageTerm" namespace="http://www.loc.gov/mods/v3">
                <xsl:value-of select="current()"/>
            </xsl:element>
        </xsl:copy>
    </xsl:template>
    <!-- Move content from placeTerm into an affliated note 
    <xsl:template match="//mods:mods[1]">
        <xsl:copy>
            <xsl:apply-templates/>
            <xsl:for-each select="mods:originInfo/mods:place/mods:placeTerm">
                <xsl:element name="note" namespace="http://www.loc.gov/mods/v3">
                    <xsl:attribute name="note">affiliation</xsl:attribute>
                    <xsl:value-of select="//mods:originInfo/mods:place/mods:placeTerm/text()"/>
                </xsl:element>
            </xsl:if>
        </xsl:copy>
    </xsl:template>-->
    <!-- Remove place where placeTerm that was moved into an affliated note 
    <xsl:template match="//mods:mods[1]/mods:originInfo/mods:place"/>-->
</xsl:stylesheet>
