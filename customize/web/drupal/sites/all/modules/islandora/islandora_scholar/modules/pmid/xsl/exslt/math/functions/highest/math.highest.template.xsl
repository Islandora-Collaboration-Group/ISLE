<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:math="http://www.exslt.org/math"
                exclude-result-prefixes="math">

<xsl:template name="math:highest">
   <xsl:param name="nodes" select="/.." />
   <xsl:if test="$nodes and not($nodes[number(.) != number(.)])">
      <xsl:variable name="max">
         <xsl:for-each select="$nodes">
            <xsl:sort data-type="number" order="descending" />
            <xsl:if test="position() = 1">
               <xsl:value-of select="number(.)" />
            </xsl:if>
         </xsl:for-each>
      </xsl:variable>
      <xsl:copy-of select="$nodes[. = $max]" />
   </xsl:if>
</xsl:template>

</xsl:stylesheet>