<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:set="http://exslt.org/sets"
                exclude-result-prefixes="set">

<xsl:template name="set:has-same-node">
   <xsl:param name="nodes1" select="/.." />
   <xsl:param name="nodes2" select="/.." />
   <xsl:value-of
      select="boolean($nodes1[count(.|$nodes2) = count($nodes2)])" />
</xsl:template>

</xsl:stylesheet>