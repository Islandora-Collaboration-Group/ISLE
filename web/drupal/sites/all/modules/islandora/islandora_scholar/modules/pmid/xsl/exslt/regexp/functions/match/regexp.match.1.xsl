<?xml version="1.0"?>
<xsl:stylesheet version="1.0" 
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:regexp="http://exslt.org/regular-expressions"
                extension-element-prefixes="regexp">

   <xsl:import href="regexp.match.msxsl.xsl" />

	<xsl:template match="c">
      <out>
         <xsl:value-of select="." /> - 
         <xsl:for-each select="regexp:match(., 'no', 'gi')">
            <xsl:value-of select="." />
         </xsl:for-each>
      </out>
	</xsl:template>

</xsl:stylesheet>