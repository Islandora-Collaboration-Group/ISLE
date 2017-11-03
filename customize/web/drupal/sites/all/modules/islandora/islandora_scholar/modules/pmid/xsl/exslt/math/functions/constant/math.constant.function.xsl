<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:math="http://exslt.org/math"
                xmlns:exsl="http://exslt.org/functions"
                extension-element-prefixes="exsl"
                exclude-result-prefixes="math">

<exsl:function name="math:constant">
  <xsl:param name="name" select="/.." />
  <xsl:param name="precision" select="/.." />


<xsl:choose>
<xsl:when test="$name='PI'">
<xsl:variable name="return">3.1415926535897932384626433832795028841971693993751</xsl:variable>
<exsl:result select="number(substring($return,1,$precision))"/>
</xsl:when>

<xsl:when test="$name='E'">
<xsl:variable name="return">2.71828182845904523536028747135266249775724709369996</xsl:variable>
<exsl:result select="number(substring($return,1,$precision))"/>
</xsl:when>

<xsl:when test="$name='SQRRT2'">
<xsl:variable name="return">1.41421356237309504880168872420969807856967187537694</xsl:variable>
<exsl:result select="number(substring($return,1,$precision))"/>
</xsl:when>

<xsl:when test="$name='LN2'">
<xsl:variable name="return">0.69314718055994530941723212145817656807550013436025</xsl:variable>
<exsl:result select="number(substring($return,1,$precision))"/>
</xsl:when>

<xsl:when test="$name='LN10'">
<xsl:variable name="return">2.302585092994046</xsl:variable>
<exsl:result select="number(substring($return,1,$precision))"/>
</xsl:when>

<xsl:when test="$name='LOG2E'">
<xsl:variable name="return">1.4426950408889633</xsl:variable>
<exsl:result select="number(substring($return,1,$precision))"/>
</xsl:when>

<xsl:when test="$name='SQRT1_2'">
<xsl:variable name="return">0.7071067811865476</xsl:variable>
<exsl:result select="number(substring($return,1,$precision))"/>
</xsl:when>
 
</xsl:choose>

</exsl:function>

   

</xsl:stylesheet>