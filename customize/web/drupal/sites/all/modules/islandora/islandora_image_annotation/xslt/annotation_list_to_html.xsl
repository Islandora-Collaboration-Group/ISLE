<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:dcterms="http://purl.org/dc/terms/"
    xmlns:dctype="http://purl.org/dc/dcmitype/" xmlns:oa="http://www.w3.org/ns/openannotation/core/"
    xmlns:cnt="http://www.w3.org/2008/content#" xmlns:dms="http://dms.stanford.edu/ns/"
    xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
    xmlns:ore="http://www.openarchives.org/ore/terms/"
    xmlns:exif="http://www.w3.org/2003/12/exif/ns#" version="1.0">
    <xsl:output method="html" version="1.0" encoding="iso-8859-1" omit-xml-declaration="yes" indent="yes"/>
    <xsl:template match="/">
        <div xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:dcterms="http://purl.org/dc/terms/"
            xmlns:dctype="http://purl.org/dc/dcmitype/"
            xmlns:oa="http://www.w3.org/ns/openannotation/core/"
            xmlns:cnt="http://www.w3.org/2008/content#" xmlns:dms="http://dms.stanford.edu/ns/"
            xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
            xmlns:ore="http://www.openarchives.org/ore/terms/"
            xmlns:exif="http://www.w3.org/2003/12/exif/ns#">
            <xsl:for-each select="/rdf:RDF/rdf:Description[rdf:type/@rdf:resource ='http://www.w3.org/ns/openannotation/core/Annotation']">
                <div>
                    <xsl:attribute name="about">
                        <xsl:value-of select="./@rdf:about"/>
                    </xsl:attribute>
                    <xsl:call-template name="process_div" />
                    <xsl:for-each select="/rdf:RDF/rdf:Description[rdf:type/@rdf:resource ='http://www.w3.org/2008/content#ContentAsText']">
                        <xsl:choose>
                            <xsl:when test="./rdf:type/@rdf:resource = 'http://www.w3.org/ns/openannotation/core/SvgConstraint'">
                            </xsl:when>
                            <xsl:otherwise>
                                <div>
                                    <xsl:attribute name="about">
                                        <xsl:value-of select="./@rdf:about"/>
                                    </xsl:attribute>
                                    <xsl:call-template name="process_div" />
                                </div>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:for-each>
                    <xsl:for-each select="/rdf:RDF/rdf:Description[rdf:type/@rdf:resource ='http://www.w3.org/ns/openannotation/core/ConstrainedTarget']">
                        <div>
                            <xsl:attribute name="about">
                                <xsl:value-of select="./@rdf:about"/>
                            </xsl:attribute>
                            <xsl:call-template name="process_div" />
                            <xsl:for-each select="/rdf:RDF/rdf:Description[rdf:type/@rdf:resource ='http://www.w3.org/ns/openannotation/core/SvgConstraint']">
                                <div>
                                    <xsl:attribute name="about">
                                        <xsl:value-of select="./@rdf:about"/>
                                    </xsl:attribute>
                                    <xsl:call-template name="process_div" />
                                </div>
                            </xsl:for-each>
                        </div>
                    </xsl:for-each>
                </div>
            </xsl:for-each>
        </div>
    </xsl:template>


    <!-- template to handle internal assignment of a and span tags  -->

    <xsl:template name="process_div">
        <xsl:for-each select="./*">
            <xsl:choose>
                <xsl:when test="@rdf:resource">
                    <a>
                        <xsl:attribute name="rel">
                            <xsl:value-of select="name()"/>
                        </xsl:attribute>
                        <xsl:attribute name="href">
                            <xsl:value-of select="./@rdf:resource"/>
                        </xsl:attribute>
                    </a>
                </xsl:when>
                <xsl:otherwise>
                    <span>
                        <xsl:attribute name="property">
                            <xsl:value-of select="name()"/>
                        </xsl:attribute>
                        <xsl:value-of select="."/>
                    </span>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:for-each>
    </xsl:template>
</xsl:stylesheet>
