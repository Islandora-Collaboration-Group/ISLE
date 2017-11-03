<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
                xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#">
    <xsl:output method="text"/>
    <xsl:template match="/">
        {
        <xsl:apply-templates select="/rdf:RDF/rdf:Property"/>
        }
    </xsl:template>
    <xsl:template match="/rdf:RDF/rdf:Property">
        "<xsl:value-of select="@rdf:ID" />":"<xsl:value-of select="rdfs:label" />"<xsl:if test="position()!=last()">,</xsl:if>
    </xsl:template>
</xsl:stylesheet>