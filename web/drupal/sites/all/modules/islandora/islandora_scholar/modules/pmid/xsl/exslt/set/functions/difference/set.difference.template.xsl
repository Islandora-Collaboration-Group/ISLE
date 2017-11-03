<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:set="http://exslt.org/sets"
                exclude-result-prefixes="set">

<xsl:template name="set:difference">
   <xsl:param name="nodes1" select="/.." />
   <xsl:param name="nodes2" select="/.." />
   <xsl:apply-templates select="$nodes1[count(.|$nodes2) != count($nodes2)]"
                        mode="set:difference" />
</xsl:template>

<xsl:template match="node()|@*" mode="set:difference">
   <xsl:copy-of select="." />
</xsl:template>

</xsl:stylesheet>