<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:math="http://exslt.org/math"
                extension-element-prefixes="math">

<xsl:output method="text" />

<xsl:template match="values">
	<xsl:text>Minimum: </xsl:text>
   <xsl:value-of select="math:min(number(value))" />
</xsl:template>

</xsl:stylesheet>