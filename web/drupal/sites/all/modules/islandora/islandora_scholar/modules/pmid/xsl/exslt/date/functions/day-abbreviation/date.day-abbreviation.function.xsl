<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:date="http://exslt.org/dates-and-times"
                xmlns:func="http://exslt.org/functions"
                extension-element-prefixes="date func">

<xsl:param name="date:date-time" select="'2000-01-01T00:00:00Z'" />

<date:month-lengths>
   <date:month>31</date:month>
   <date:month>28</date:month>
   <date:month>31</date:month>
   <date:month>30</date:month>
   <date:month>31</date:month>
   <date:month>30</date:month>
   <date:month>31</date:month>
   <date:month>31</date:month>
   <date:month>30</date:month>
   <date:month>31</date:month>
   <date:month>30</date:month>
   <date:month>31</date:month>
</date:month-lengths>

<date:days>
   <date:day abbr="Sun">Sunday</date:day>
   <date:day abbr="Mon">Monday</date:day>
   <date:day abbr="Tue">Tuesday</date:day>
   <date:day abbr="Wed">Wednesday</date:day>
   <date:day abbr="Thu">Thursday</date:day>
   <date:day abbr="Fri">Friday</date:day>
   <date:day abbr="Sat">Saturday</date:day>
</date:days>

<func:function name="date:day-abbreviation">
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
   <xsl:variable name="neg" select="starts-with($date-time, '-')" />
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
   <xsl:variable name="day-of-week">
      <xsl:if test="not(string($timezone)) or
                    $timezone = 'Z' or 
                    (substring($timezone, 2, 2) &lt;= 23 and
                     substring($timezone, 5, 2) &lt;= 59)">
         <xsl:variable name="dt" select="substring($dt-no-neg, 1, $dt-no-neg-length - string-length($timezone))" />
         <xsl:variable name="dt-length" select="string-length($dt)" />
         <xsl:variable name="year" select="substring($dt, 1, 4)" />
         <xsl:variable name="leap" select="(not($year mod 4) and $year mod 100) or not($year mod 400)" />
         <xsl:variable name="month" select="substring($dt, 6, 2)" />
         <xsl:variable name="day" select="substring($dt, 9, 2)" />
         <xsl:if test="number($year) and
                       substring($dt, 5, 1) = '-' and
                       $month &lt;= 12 and
                       substring($dt, 8, 1) = '-' and
                       $day &lt;= 31 and
                       ($dt-length = 10 or
                        (substring($dt, 11, 1) = 'T' and
                         substring($dt, 12, 2) &lt;= 23 and
                         substring($dt, 14, 1) = ':' and
                         substring($dt, 15, 2) &lt;= 59 and
                         substring($dt, 17, 1) = ':' and
                         substring($dt, 18) &lt;= 60))">
            <xsl:variable name="month-days" select="sum(document('')/*/date:month-lengths/date:month[position() &lt; $month])" />
            <xsl:variable name="days">
               <xsl:choose>
                  <xsl:when test="$leap and $month > 2">
                     <xsl:value-of select="$month-days + $day + 1" />
                  </xsl:when>
                  <xsl:otherwise>
                     <xsl:value-of select="$month-days + $day" />
                  </xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="y-1" select="$year - 1" />
            <xsl:value-of select="(($y-1 + floor($y-1 div 4) -
                                    floor($y-1 div 100) + floor($y-1 div 400) +
                                    $days) 
                                   mod 7) + 1" />
         </xsl:if>
      </xsl:if>
   </xsl:variable>
   <func:result select="string(document('')/*/date:days/date:day[number($day-of-week)]/@abbr)" />   
</func:function>

</xsl:stylesheet>