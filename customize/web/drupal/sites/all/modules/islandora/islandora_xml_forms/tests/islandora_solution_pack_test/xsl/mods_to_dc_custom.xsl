<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns:mods="http://www.loc.gov/mods/v3" exclude-result-prefixes="mods"
	xmlns:dc="http://purl.org/dc/elements/1.1/"
	xmlns:srw_dc="info:srw/schema/1/dc-schema"
	xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" indent="yes"/>
	
	<xsl:template match="/">
		<xsl:for-each select="mods:mods">
		<oai_dc:dc xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc/ http://www.openarchives.org/OAI/2.0/oai_dc.xsd">
			<xsl:apply-templates/>
		</oai_dc:dc>
		</xsl:for-each>
	</xsl:template>
	
	<xsl:template match="mods:titleInfo">
        <xsl:if test=".='Title'">
            <dc:title/>
        </xsl:if>
	</xsl:template>

	<xsl:template match="mods:note">
        <xsl:if test=".='Abstract'">
            <dc:description>
                Transformed Note
            </dc:description>
        </xsl:if>
	</xsl:template>

	<xsl:template match="mods:genre">
        <xsl:if test=".='alternate'">
		    <dc:type>
			    Transformed Genre
		    </dc:type>
        </xsl:if>
	</xsl:template>

	<xsl:template match="mods:typeOfResource">
		<xsl:if test=".='Resource'">
			<dc:type>Transformed Resource</dc:type>
		</xsl:if>
	</xsl:template>

    <xsl:template match="mods:language">
        <xsl:if test=".='Language'">
            <dc:language>
                Transformed Language
            </dc:language>
        </xsl:if>
    </xsl:template>

	<xsl:template match="*"/>
	
</xsl:stylesheet>
