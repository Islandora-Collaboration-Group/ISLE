<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
    xmlns:mads="http://www.loc.gov/mads/v2" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:dc="http://purl.org/dc/terms/">
    <xsl:output method="xml" indent="yes"/>

    <xsl:template match="/">
        <oai_dc:dc xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc/ http://www.openarchives.org/OAI/2.0/oai_dc.xsd"
            xmlns:dc="http://purl.org/dc/elements/1.1/"
            xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/">
            <xsl:apply-templates exclude-result-prefixes="#all"/>
            <xsl:for-each select="//mads:authority//mads:namePart">

                <xsl:choose>
                    <xsl:when test="@type = 'given'">
                        <xsl:element name="dc:subject">
                            <xsl:value-of select="//mads:authority//mads:namePart[@type = 'family']"/>
                            <xsl:text>, </xsl:text>
                            <xsl:value-of select="//mads:authority//mads:namePart[@type = 'given']"
                            />
                        </xsl:element>

                        <xsl:element name="dc:title">
                            <xsl:value-of select="//mads:authority//mads:namePart[@type = 'family']"/>
                            <xsl:text>, </xsl:text>
                            <xsl:value-of select="//mads:authority//mads:namePart[@type = 'given']"
                            />

                        </xsl:element>
                    </xsl:when>
                    <xsl:when test="not(@*)">
                        <xsl:element name="dc:title">
                            <xsl:value-of select="."/>
                        </xsl:element>
                        <xsl:element name="dc:subject">
                            <xsl:value-of select="."/>
                        </xsl:element>
                    </xsl:when>
                    <xsl:when test="@type = 'date'">
                        <xsl:element name="dc:date">
                            <xsl:value-of select="."/>
                        </xsl:element>
                    </xsl:when>
                </xsl:choose>
            </xsl:for-each>

            <xsl:for-each select="//mads:note">
                <xsl:element name="dc:description">
                    <xsl:value-of select="."/>
                </xsl:element>
            </xsl:for-each>
            
        </oai_dc:dc>
    </xsl:template>

    <!-- suppress all else:-->

    <xsl:template match="*"/>

</xsl:stylesheet>

