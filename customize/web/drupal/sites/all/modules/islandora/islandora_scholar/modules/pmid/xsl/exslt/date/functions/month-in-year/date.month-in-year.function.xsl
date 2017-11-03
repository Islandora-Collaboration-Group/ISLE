<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:date="http://exslt.org/dates-and-times"
                xmlns:func="http://exslt.org/functions"
                extension-element-prefixes="date func">

<xsl:param name="date:date-time" select="'2000-01-01T00:00:00Z'" />

<func:function name="date:month-in-year">
	<xsl:param name="date-time">
      <xsl:choose>
         <xsl:when test="function-available('date:date-time')">
            <xsl:value-of select="date:date-time()" />
         </xsl:when>
         <xsl:otherwise>
            <xsl:value-of select="$date:date-time" />
         </xsl:otherwise>
      </xsl:choose>
   </xsl:param>
   <xsl:variable name="neg" select="starts-with($date-time, '-') and 
                                    not(starts-with($date-time, '--'))" />
   <xsl:variable name="dt-no-neg">
      <xsl:choose>
         <xsl:when test="$neg or starts-with($date-time, '+')">
            <xsl:value-of select="substring($date-time, 2)" />
         </xsl:when>
         <xsl:otherwise>
            <xsl:value-of select="$date-time" />
         </xsl:otherwise>
      </xsl:choose>
   </xsl:variable>
   <xsl:variable name="dt-no-neg-length" select="string-length($dt-no-neg)" />
   <xsl:variable name="timezone">
      <xsl:choose>
         <xsl:when test="substring($dt-no-neg, $dt-no-neg-length) = 'Z'">Z</xsl:when>
         <xsl:otherwise>
            <xsl:variable name="tz" select="substring($dt-no-neg, $dt-no-neg-length - 5)" />
            <xsl:if test="(substring($tz, 1, 1) = '-' or 
                           substring($tz, 1, 1) = '+') and
                          substring($tz, 4, 1) = ':'">
               <xsl:value-of select="$tz" />
            </xsl:if>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:variable>
   <xsl:variable name="month">
      <xsl:if test="not(string($timezone)) or
                    $timezone = 'Z' or 
                    (substring($timezone, 2, 2) &lt;= 23 and
                     substring($timezone, 5, 2) &lt;= 59)">
         <xsl:variable name="dt" select="substring($dt-no-neg, 1, $dt-no-neg-length - string-length($timezone))" />
         <xsl:variable name="dt-length" select="string-length($dt)" />
         <xsl:choose>
            <xsl:when test="substring($dt, 1, 2) = '--' and
                            substring($dt, 3, 2) &lt;= 12 and
                            substring($dt, 5, 1) = '-' and
                            (substring($dt, 6) = '-' or
                             ($dt-length = 7 and
                              substring($dt, 6) &lt;= 31))">
               <xsl:value-of select="substring($dt, 3, 2)" />
            </xsl:when>
            <xsl:when test="number(substring($dt, 1, 4)) and
                            substring($dt, 5, 1) = '-' and
                            substring($dt, 6, 2) &lt;= 12 and
                            ($dt-length = 7 or
                             (substring($dt, 8, 1) = '-' and
                              substring($dt, 9, 2) &lt;= 31 and
                              ($dt-length = 10 or
                               (substring($dt, 11, 1) = 'T' and
                                substring($dt, 12, 2) &lt;= 23 and
                                substring($dt, 14, 1) = ':' and
                                substring($dt, 15, 2) &lt;= 59 and
                                substring($dt, 17, 1) = ':' and
                                substring($dt, 18) &lt;= 60))))">
               <xsl:value-of select="substring($dt, 6, 2)" />
            </xsl:when>
         </xsl:choose>
      </xsl:if>
   </xsl:variable>
   <func:result select="number($month)" />
</func:function>

</xsl:stylesheet>