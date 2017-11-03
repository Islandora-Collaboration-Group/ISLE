<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:math="http://www.exslt.org/math"
                exclude-result-prefixes="math">

<xsl:template name="math:max">
   <xsl:param name="nodes" select="/.." />
   <xsl:choose>
      <xsl:when test="not($nodes)">NaN</xsl:when>
      <xsl:otherwise>
         <xsl:for-each select="$nodes">
            <xsl:sort data-type="number" order="descending" />
            <xsl:if test="position() = 1">
               <xsl:value-of select="number(.)" />
            </xsl:if>
         </xsl:for-each>
      </xsl:otherwise>
   </xsl:choose>
</xsl:template>

</xsl:stylesheet>