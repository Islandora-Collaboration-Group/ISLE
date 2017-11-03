<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" media-type="text/xml"/>
<xsl:strip-space elements="*"/>
<xsl:template match="*[not(node())]"/>    
<xsl:template match="node()|@*">
    <xsl:copy>
        <xsl:apply-templates select="node()[normalize-space()]|@*[normalize-space()]"/>
    </xsl:copy>
</xsl:template>
</xsl:stylesheet>

