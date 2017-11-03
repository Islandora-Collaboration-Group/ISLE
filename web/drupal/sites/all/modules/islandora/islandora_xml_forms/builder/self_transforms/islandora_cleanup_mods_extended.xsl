<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:mods="http://www.loc.gov/mods/v3">
    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" media-type="text/xml"/>
    <xsl:strip-space elements="*"/>
    <xsl:template
        match="*[not(node())] | *[not(node()[2]) and node()/self::text() and not(normalize-space())]"/>
    <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()[normalize-space()]|@*[normalize-space()]"/>
        </xsl:copy>
    </xsl:template>
    <xsl:template match="mods:language">
        <xsl:copy>
            <xsl:apply-templates select="@*[normalize-space()]"/>
            <xsl:apply-templates select="mods:languageTerm"/>
            <xsl:apply-templates select="mods:scriptTerm"/>
            <!-- Do not strip any custom elements. -->
            <xsl:apply-templates select="node()[not(self::mods:languageTerm|self::mods:scriptTerm)]"/>
        </xsl:copy>
    </xsl:template>
    <xsl:template match="mods:location">
        <xsl:copy>
            <xsl:apply-templates select="@*[normalize-space()]"/>
            <xsl:apply-templates select="mods:physicalLocation"/>
            <xsl:apply-templates select="mods:shelfLocator"/>
            <xsl:apply-templates select="mods:url"/>
            <xsl:apply-templates select="mods:holdingSimple"/>
            <xsl:apply-templates select="mods:holdingExternal"/>
            <!-- Do not strip any custom elements. -->
            <xsl:apply-templates select="node()[not(self::mods:physicalLocation|self::mods:shelfLocator|self::mods:url|self::mods:holdingSimple|self::mods:holdingExternal)]"/>
        </xsl:copy>
    </xsl:template>
    <xsl:template match="mods:copyInformation">
        <xsl:copy>
            <xsl:apply-templates select="@*[normalize-space()]"/>
            <xsl:apply-templates select="mods:form"/>
            <xsl:apply-templates select="mods:subLocation"/>
            <xsl:apply-templates select="mods:shelfLocator"/>
            <xsl:apply-templates select="mods:electronicLocator"/>
            <xsl:apply-templates select="mods:note"/>
            <xsl:apply-templates select="mods:enumerationAndChronology"/>
            <xsl:apply-templates select="mods:itemIdentifier"/>
            <!-- Do not strip any custom elements. -->
            <xsl:apply-templates select="node()[not(self::mods:form|self::mods:subLocation|self::mods:shelfLocator|self::mods:electronicLocator|self::mods:note|self::mods:enumerationAndChronology|self::mods:itemIdentifier)]"/>
        </xsl:copy>
    </xsl:template>
    <!--
        The top level mods:name element, can choose between an 'normal' name and mods:etal.
        When mods:etal is pressent than it must be the first element, order is not imposed
        on it's other siblings.

        Note this is different than the name element used within mods:subject.
    -->
    <xsl:template match="mods:name[mods:etal]">
        <xsl:copy>
            <xsl:apply-templates select="@*[normalize-space()]"/>
            <xsl:apply-templates select="mods:etal"/>
            <!-- Do not strip any custom elements. -->
            <xsl:apply-templates select="node()[not(self::mods:etal)]"/>
        </xsl:copy>
    </xsl:template>
    <!--
        There is more than one type of mods:extent in MODS, and the ordering of
        elements only applies to the mods:extent that's a child of mods:part.
    -->
    <xsl:template match="mods:extent[parent::mods:part]">
        <xsl:copy>
            <xsl:apply-templates select="@*[normalize-space()]"/>
            <xsl:apply-templates select="mods:start"/>
            <xsl:apply-templates select="mods:end"/>
            <xsl:apply-templates select="mods:total"/>
            <xsl:apply-templates select="mods:list"/>
            <!-- Do not strip any custom elements. -->
            <xsl:apply-templates select="node()[not(self::mods:start|self::mods:end|self::mods:total|self::mods:list)]"/>
        </xsl:copy>
    </xsl:template>
    <xsl:template match="mods:cartographics">
        <xsl:copy>
            <xsl:apply-templates select="@*[normalize-space()]"/>
            <xsl:apply-templates select="mods:scale"/>
            <xsl:apply-templates select="mods:projection"/>
            <xsl:apply-templates select="mods:coordinates"/>
            <xsl:apply-templates select="mods:cartographicExtension"/>
            <!-- Do not strip any custom elements. -->
            <xsl:apply-templates select="node()[not(self::mods:scale|self::mods:projection|self::mods:coordinates|self::mods:cartographicExtension)]"/>
        </xsl:copy>
    </xsl:template>
</xsl:stylesheet>
