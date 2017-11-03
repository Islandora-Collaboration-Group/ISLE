<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3">

    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" media-type="text/xml"/>
    <xsl:strip-space elements="*"/>
    <xsl:template match="mods:abstract">
        <note>
            <xsl:value-of select="."/>
        </note>
        <typeOfResource>Resource</typeOfResource>
    </xsl:template>
    <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()[normalize-space()]|@*[normalize-space()]"/>
        </xsl:copy>
    </xsl:template>

</xsl:stylesheet>