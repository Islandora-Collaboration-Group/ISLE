<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:date="http://exslt.org/dates-and-times"
                xmlns:func="http://exslt.org/functions"
                extension-element-prefixes="date func">

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

<func:function name="date:difference">
   <xsl:param name="start" />
   <xsl:param name="end" />

   <xsl:variable name="start-neg" select="starts-with($start, '-')" />
   <xsl:variable name="start-no-neg">
      <xsl:choose>
         <xsl:when test="$start-neg or starts-with($start, '+')">
            <xsl:value-of select="substring($start, 2)" />
         </xsl:when>
         <xsl:otherwise>
            <xsl:value-of select="$start" />
         </xsl:otherwise>
      </xsl:choose>
   </xsl:variable>
   <xsl:variable name="start-no-neg-length" select="string-length($start-no-neg)" />
   <xsl:variable name="start-timezone">
      <xsl:choose>
         <xsl:when test="substring($start-no-neg, $start-no-neg-length) = 'Z'">Z</xsl:when>
         <xsl:otherwise>
            <xsl:variable name="tz" select="substring($start-no-neg, $start-no-neg-length - 5)" />
            <xsl:if test="(substring($tz, 1, 1) = '-' or 
                           substring($tz, 1, 1) = '+') and
                          substring($tz, 4, 1) = ':'">
               <xsl:value-of select="$tz" />
            </xsl:if>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:variable>
   <xsl:variable name="end-neg" select="starts-with($end, '-')" />
   <xsl:variable name="end-no-neg">
      <xsl:choose>
         <xsl:when test="$end-neg or starts-with($end, '+')">
            <xsl:value-of select="substring($end, 2)" />
         </xsl:when>
         <xsl:otherwise>
            <xsl:value-of select="$end" />
         </xsl:otherwise>
      </xsl:choose>
   </xsl:variable>
   <xsl:variable name="end-no-neg-length" select="string-length($end-no-neg)" />
   <xsl:variable name="end-timezone">
      <xsl:choose>
         <xsl:when test="substring($end-no-neg, $end-no-neg-length) = 'Z'">Z</xsl:when>
         <xsl:otherwise>
            <xsl:variable name="tz" select="substring($end-no-neg, $end-no-neg-length - 5)" />
            <xsl:if test="(substring($tz, 1, 1) = '-' or 
                           substring($tz, 1, 1) = '+') and
                          substring($tz, 4, 1) = ':'">
               <xsl:value-of select="$tz" />
            </xsl:if>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:variable>

   <xsl:variable name="difference">
      <xsl:if test="(not(string($start-timezone)) or
                     $start-timezone = 'Z' or 
                     (substring($start-timezone, 2, 2) &lt;= 23 and
                      substring($start-timezone, 5, 2) &lt;= 59)) and
                    (not(string($end-timezone)) or
                     $end-timezone = 'Z' or 
                     (substring($end-timezone, 2, 2) &lt;= 23 and
                      substring($end-timezone, 5, 2) &lt;= 59))">
         <xsl:variable name="start-dt" select="substring($start-no-neg, 1, $start-no-neg-length - string-length($start-timezone))" />
         <xsl:variable name="start-dt-length" select="string-length($start-dt)" />
         <xsl:variable name="end-dt" select="substring($end-no-neg, 1, $end-no-neg-length - string-length($end-timezone))" />
         <xsl:variable name="end-dt-length" select="string-length($end-dt)" />

         <xsl:variable name="start-year" select="substring($start-dt, 1, 4) * (($start-neg * -2) + 1)" />
         <xsl:variable name="end-year" select="substring($end-dt, 1, 4) * (($end-neg * -2) + 1)" />
         <xsl:variable name="diff-year" select="$end-year - $start-year" />
         <xsl:choose>
            <xsl:when test="not(number($start-year) and number($end-year))" />
            <xsl:when test="$start-dt-length = 4 or $end-dt-length = 4">
               <xsl:choose>
                  <xsl:when test="$diff-year &lt; 0">-P<xsl:value-of select="$diff-year * -1" />Y</xsl:when>
                  <xsl:otherwise>P<xsl:value-of select="$diff-year" />Y</xsl:otherwise>
               </xsl:choose>
            </xsl:when>
            <xsl:when test="substring($start-dt, 5, 1) = '-' and 
                            substring($end-dt, 5, 1) = '-'">
               <xsl:variable name="start-month" select="substring($start-dt, 6, 2)" />
               <xsl:variable name="end-month" select="substring($end-dt, 6, 2)" />
               <xsl:variable name="diff-month" select="$end-month - $start-month" />
               <xsl:choose>
                  <xsl:when test="not($start-month &lt;= 12 and $end-month &lt;= 12)" />
                  <xsl:when test="$start-dt-length = 7 or $end-dt-length = 7">
                     <xsl:variable name="months" select="$diff-month + ($diff-year * 12)" />
                     <xsl:variable name="abs-months" select="$months * ((($months >= 0) * 2) - 1)" />
                     <xsl:variable name="y" select="floor($abs-months div 12)" />
                     <xsl:variable name="m" select="$abs-months mod 12" />
                     <xsl:if test="$months &lt; 0">-</xsl:if>
                     <xsl:text>P</xsl:text>
                     <xsl:if test="$y"><xsl:value-of select="$y" />Y</xsl:if>
                     <xsl:if test="$m"><xsl:value-of select="$m" />M</xsl:if>
                  </xsl:when>
                  <xsl:when test="substring($start-dt, 8, 1) = '-' and
                                  substring($end-dt, 8, 1) = '-'">
                     <xsl:variable name="start-day" select="substring($start-dt, 9, 2)" />
                     <xsl:variable name="end-day" select="substring($end-dt, 9, 2)" />
                     <xsl:if test="$start-day &lt;= 31 and $end-day &lt;= 31">
                        <xsl:variable name="month-lengths" select="document('')/*/date:month-lengths/date:month" />
                        <xsl:variable name="start-y-1" select="$start-year - 1" />
                        <xsl:variable name="start-leaps" 
                                      select="floor($start-y-1 div 4) -
                                              floor($start-y-1 div 100) +
                                              floor($start-y-1 div 400)" />
                        <xsl:variable name="start-leap" select="(not($start-year mod 4) and $start-year mod 100) or not($start-year mod 400)" />
                        <xsl:variable name="start-month-days" 
                                      select="sum($month-lengths[position() &lt; $start-month])" />
                        <xsl:variable name="start-days">
                           <xsl:variable name="days" 
                                         select="($start-year * 365) + $start-leaps +
                                                 $start-month-days + $start-day" />
                           <xsl:choose>
                              <xsl:when test="$start-leap">
                                 <xsl:value-of select="$days + 1" />
                              </xsl:when>
                              <xsl:otherwise>
                                 <xsl:value-of select="$days" />
                              </xsl:otherwise>
                           </xsl:choose>
                        </xsl:variable>
                        <xsl:variable name="end-y-1" select="$end-year - 1" />
                        <xsl:variable name="end-leaps" 
                                      select="floor($end-y-1 div 4) -
                                              floor($end-y-1 div 100) +
                                              floor($end-y-1 div 400)" />
                        <xsl:variable name="end-leap" select="(not($end-year mod 4) and $end-year mod 100) or not($end-year mod 400)" />
                        <xsl:variable name="end-month-days" 
                                      select="sum($month-lengths[position() &lt; $end-month])" />
                        <xsl:variable name="end-days">
                           <xsl:variable name="days" 
                                         select="($end-year * 365) + $end-leaps +
                                                 $end-month-days + $end-day" />
                           <xsl:choose>
                              <xsl:when test="$end-leap">
                                 <xsl:value-of select="$days + 1" />
                              </xsl:when>
                              <xsl:otherwise>
                                 <xsl:value-of select="$days" />
                              </xsl:otherwise>
                           </xsl:choose>
                        </xsl:variable>
                        <xsl:variable name="diff-days" select="$end-days - $start-days" />
                        <xsl:choose>
                           <xsl:when test="$start-dt-length = 10 or $end-dt-length = 10">
                              <xsl:choose>
                                 <xsl:when test="$diff-days &lt; 0">-P<xsl:value-of select="$diff-days * -1" />D</xsl:when>
                                 <xsl:otherwise>P<xsl:value-of select="$diff-days" />D</xsl:otherwise>
                              </xsl:choose>
                           </xsl:when>
                           <xsl:when test="substring($start-dt, 11, 1) = 'T' and
                                           substring($end-dt, 11, 1) = 'T' and
                                           substring($start-dt, 14, 1) = ':' and
                                           substring($start-dt, 17, 1) = ':' and
                                           substring($end-dt, 14, 1) = ':' and
                                           substring($end-dt, 17, 1) = ':'">
                              <xsl:variable name="start-hour" select="substring($start-dt, 12, 2)" />
                              <xsl:variable name="start-min" select="substring($start-dt, 15, 2)" />
                              <xsl:variable name="start-sec" select="substring($start-dt, 18)" />
                              <xsl:variable name="end-hour" select="substring($end-dt, 12, 2)" />
                              <xsl:variable name="end-min" select="substring($end-dt, 15, 2)" />
                              <xsl:variable name="end-sec" select="substring($end-dt, 18)" />
                              <xsl:if test="$start-hour &lt;= 23 and $end-hour &lt;= 23 and
                                            $start-min &lt;= 59 and $end-min &lt;= 59 and
                                            $start-sec &lt;= 60 and $end-sec &lt;= 60">
                                 <xsl:variable name="min-s" select="60" />
                                 <xsl:variable name="hour-s" select="60 * 60" />
                                 <xsl:variable name="day-s" select="60 * 60 * 24" />

                                 <xsl:variable name="start-tz-adj">
                                    <xsl:variable name="tz" 
                                                  select="(substring($start-timezone, 2, 2) * $hour-s) + 
                                                          (substring($start-timezone, 5, 2) * $min-s)" />
                                    <xsl:choose>
                                       <xsl:when test="starts-with($start-timezone, '-')">
                                          <xsl:value-of select="$tz" />
                                       </xsl:when>
                                       <xsl:when test="starts-with($start-timezone, '+')">
                                          <xsl:value-of select="$tz * -1" />
                                       </xsl:when>
                                       <xsl:otherwise>0</xsl:otherwise>
                                    </xsl:choose>
                                 </xsl:variable>
                                 <xsl:variable name="end-tz-adj">
                                    <xsl:variable name="tz" 
                                                  select="(substring($end-timezone, 2, 2) * $hour-s) + 
                                                          (substring($end-timezone, 5, 2) * $min-s)" />
                                    <xsl:choose>
                                       <xsl:when test="starts-with($end-timezone, '-')">
                                          <xsl:value-of select="$tz" />
                                       </xsl:when>
                                       <xsl:when test="starts-with($end-timezone, '+')">
                                          <xsl:value-of select="$tz * -1" />
                                       </xsl:when>
                                       <xsl:otherwise>0</xsl:otherwise>
                                    </xsl:choose>
                                 </xsl:variable>

                                 <xsl:variable name="start-secs" select="$start-sec + ($start-min * $min-s) + ($start-hour * $hour-s) + ($start-days * $day-s) + $start-tz-adj" />
                                 <xsl:variable name="end-secs" select="$end-sec + ($end-min * $min-s) + ($end-hour * $hour-s) + ($end-days * $day-s) + $end-tz-adj" />
                                 <xsl:variable name="diff-secs" select="$end-secs - $start-secs" />
                                 <xsl:variable name="s" select="$diff-secs * ((($diff-secs &lt; 0) * -2) + 1)" />
                                 <xsl:variable name="days" select="floor($s div $day-s)" />
                                 <xsl:variable name="hours" select="floor(($s - ($days * $day-s)) div $hour-s)" />
                                 <xsl:variable name="mins" select="floor(($s - ($days * $day-s) - ($hours * $hour-s)) div $min-s)" />
                                 <xsl:variable name="secs" select="$s - ($days * $day-s) - ($hours * $hour-s) - ($mins * $min-s)" />
                                 <xsl:if test="$diff-secs &lt; 0">-</xsl:if>
                                 <xsl:text>P</xsl:text>
                                 <xsl:if test="$days">
                                    <xsl:value-of select="$days" />
                                    <xsl:text>D</xsl:text>
                                 </xsl:if>
                                 <xsl:if test="$hours or $mins or $secs">T</xsl:if>
                                 <xsl:if test="$hours">
                                    <xsl:value-of select="$hours" />
                                    <xsl:text>H</xsl:text>
                                 </xsl:if>
                                 <xsl:if test="$mins">
                                    <xsl:value-of select="$mins" />
                                    <xsl:text>M</xsl:text>
                                 </xsl:if>
                                 <xsl:if test="$secs">
                                    <xsl:value-of select="$secs" />
                                    <xsl:text>S</xsl:text>
                                 </xsl:if>
                              </xsl:if>
                           </xsl:when>
                        </xsl:choose>
                     </xsl:if>
                  </xsl:when>
               </xsl:choose>
            </xsl:when>
         </xsl:choose>
      </xsl:if>
   </xsl:variable>
   <func:result select="$difference" />   
</func:function>

</xsl:stylesheet>