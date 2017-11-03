<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:dcterms="http://purl.org/dc/terms/"
    xmlns:dctype="http://purl.org/dc/dcmitype/" xmlns:oa="http://www.w3.org/ns/openannotation/core/"
    xmlns:cnt="http://www.w3.org/2008/content#" xmlns:dms="http://dms.stanford.edu/ns/"
    xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" 

    xmlns:ore="http://www.openarchives.org/ore/terms/"
    xmlns:exif="http://www.w3.org/2003/12/exif/ns#" version="1.0">
    <xsl:output method="xml" version="1.0" encoding="iso-8859-1" indent="yes"/>
    <xsl:template match="/">
        <rdf:RDF>
            <xsl:for-each select="//div[@about]">
                
                <rdf:Description>
                    <xsl:attribute name="rdf:about">
                        <xsl:value-of select="./@about"/>
                    </xsl:attribute>
                    <xsl:for-each select="./a">
                        <xsl:element name="{@rel}">
                            <xsl:attribute name="rdf:resource">
                                <xsl:value-of select="./@href"/>
                            </xsl:attribute>

                        </xsl:element>
                    </xsl:for-each>

                    <xsl:for-each select="./span">
                        <xsl:element name="{@property}">
                            <xsl:value-of select="./@content"/>
                            <xsl:value-of select="."/>
                        </xsl:element>
                    </xsl:for-each>
                </rdf:Description>
                
            </xsl:for-each>
        </rdf:RDF>
    </xsl:template>
</xsl:stylesheet>
