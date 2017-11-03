<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:exsl="http://exslt.org/functions"
                xmlns:set="http://exslt.org/sets"
                extension-element-prefixes="exsl"
                exclude-result-prefixes="set">

<exsl:function name="set:trailing">
   <xsl:param name="nodes" select="/.." />
   <xsl:param name="node" select="/.." />
   <xsl:variable name="end-node" select="$node[1]" />
   <xsl:choose>
      <xsl:when test="not($end-node) or not($nodes)">
         <exsl:result select="$nodes" />
      </xsl:when>
      <xsl:when test="count($nodes | $end-node) != count($nodes)">
         <exsl:result select="/.." />
      </xsl:when>
      <xsl:when test="count($nodes[1] | $end-node) = 1">
         <exsl:result select="$nodes[position() > 1]" />
      </xsl:when>
      <xsl:otherwise>
         <exsl:result select="set:trailing($nodes[position() > 1], $end-node)" />
      </xsl:otherwise>
   </xsl:choose>
</exsl:function>

</xsl:stylesheet>