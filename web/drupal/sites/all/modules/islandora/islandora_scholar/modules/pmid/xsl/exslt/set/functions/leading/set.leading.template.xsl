<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:set="http://exslt.org/sets"
                exclude-result-prefixes="set">

<xsl:template name="set:leading">
   <xsl:param name="nodes" select="/.." />
   <xsl:param name="node" select="/.." />
   <xsl:variable name="end-node" select="$node[1]" />
   <xsl:choose>
      <xsl:when test="not($end-node) or not($nodes)">
         <xsl:apply-templates select="$nodes"
                              mode="set:leading" />
      </xsl:when>
      <xsl:when test="count($end-node|$nodes) != count($nodes) or 
                      count($nodes[1] | $end-node) = 1" />
      <xsl:otherwise>
         <xsl:apply-templates select="$nodes[1]"
                              mode="set:leading" />
         <xsl:call-template name="set:leading">
            <xsl:with-param name="nodes" select="$nodes[position() > 1]" />
            <xsl:with-param name="node" select="$end-node" />
         </xsl:call-template>
      </xsl:otherwise>
   </xsl:choose>
</xsl:template>

<xsl:template match="node()|@*" mode="set:leading">
   <xsl:copy-of select="." />
</xsl:template>

</xsl:stylesheet>