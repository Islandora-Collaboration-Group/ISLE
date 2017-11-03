<?xml version="1.0"?>
<xsl:stylesheet version="1.0" 
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:regexp="http://exslt.org/regular-expressions"
                extension-element-prefixes="regexp">

	<xsl:template match="a">
		<xsl:apply-templates/>
	</xsl:template>
	<xsl:template match="*">
      <out>
         <xsl:value-of select="." /> -
         <xsl:value-of select="regexp:replace(string(.), 'no', 'g', 'yes!!!')" />
         <xsl:value-of select="regexp:replace(string(.), 'no', 'gi', 'yes!!!')" />
         <xsl:apply-templates select = "*" />
      </out>
	</xsl:template>

</xsl:stylesheet>