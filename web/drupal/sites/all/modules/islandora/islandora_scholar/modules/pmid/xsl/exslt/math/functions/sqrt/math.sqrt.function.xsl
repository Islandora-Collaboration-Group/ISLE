<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:math="http://exslt.org/math"
                xmlns:func="http://exslt.org/functions"
                extension-element-prefixes="math func">

<func:function name="math:sqrt">
  <!-- The number you want to find the square root of -->
  <xsl:param name="number" select="0" />  
  <!-- The current 'try'.  This is used internally. -->
  <xsl:param name="try" select="1" />
  <!-- The current iteration, checked against maxiter to limit loop count -->  
  <xsl:param name="iter" select="1" />
  <!-- Set this up to ensure against infinite loops -->
  <xsl:param name="maxiter" select="10" />

  <!-- This template was written by Nate Austin using Sir Isaac Newton's
       method of finding roots -->

  <xsl:choose>
    <xsl:when test="$try * $try = $number or $iter > $maxiter">
      <func:result select="$try"/>
    </xsl:when>
    <xsl:otherwise>
      <func:result select="math:sqrt($number,
                                     $try - (($try * $try - $number) div (2 * $try)),
                                     $iter + 1, $maxiter)" />
    </xsl:otherwise>
  </xsl:choose>
</func:function>

</xsl:stylesheet>