<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:math="http://exslt.org/math"
                xmlns:exsl="http://exslt.org/functions"
                extension-element-prefixes="exsl"
                exclude-result-prefixes="math">

<exsl:function name="math:highest">
   <xsl:param name="nodes" select="/.." />
   <xsl:choose>
      <xsl:when test="$nodes and not($nodes[number(.) != number(.)])">
         <xsl:variable name="max">
            <xsl:for-each select="$nodes">
               <xsl:sort data-type="number" order="descending" />
               <xsl:if test="position() = 1">
                  <xsl:value-of select="number(.)" />
               </xsl:if>
            </xsl:for-each>
         </xsl:variable>
         <exsl:return select="$nodes[. = $max]" />
      </xsl:when>
      <xsl:otherwise>
         <exsl:return select="/.." />
      </xsl:otherwise>
   </xsl:choose>
</exsl:function>

</xsl:stylesheet>