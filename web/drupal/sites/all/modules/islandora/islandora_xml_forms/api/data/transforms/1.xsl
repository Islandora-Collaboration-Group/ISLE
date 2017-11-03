<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <!-- 
        This document transforms a Form Definition of version 0 to version 1 
    -->
    <xsl:output omit-xml-declaration="yes" indent="yes" method="xml"/>
    <xsl:strip-space elements="*"/>
    <xsl:template match="/definition">
        <definition xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
            <properties>
                <root_name>
                    <xsl:value-of select="properties/document/root/local_name"/>
                </root_name>
                <xsl:if test="properties/document/root/uri | properties/document/namespaces">
                    <namespaces>
                        <xsl:if test="properties/document/root/uri">
                            <xsl:attribute name="default">
                                <xsl:value-of select="properties/document/root/uri"/>
                            </xsl:attribute>
                        </xsl:if>
                        <xsl:copy-of select="properties/document/namespaces/namespace"/>
                    </namespaces>
                </xsl:if>
                <xsl:if test="properties/document/schema">
                    <schema_uri><xsl:value-of select="properties/document/schema"/></schema_uri>
                </xsl:if>
            </properties>
            <xsl:copy-of select="form"/>
        </definition>
    </xsl:template>
</xsl:stylesheet>
