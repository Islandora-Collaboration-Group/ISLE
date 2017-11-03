<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:date="http://exslt.org/dates-and-times"
                extension-element-prefixes="date">

<xsl:template name="date:add-duration">
	 <xsl:param name="duration1" />
   <xsl:param name="duration2" />
   <xsl:variable name="du1-neg" select="starts-with($duration1, '-')" />
   <xsl:variable name="du1">
      <xsl:choose>
         <xsl:when test="$du1-neg"><xsl:value-of select="substring($duration1, 2)" /></xsl:when>
         <xsl:otherwise><xsl:value-of select="$duration1" /></xsl:otherwise>
      </xsl:choose>
   </xsl:variable>
   <xsl:variable name="du2-neg" select="starts-with($duration2, '-')" />
   <xsl:variable name="du2">
      <xsl:choose>
         <xsl:when test="$du2-neg"><xsl:value-of select="substring($duration2, 2)" /></xsl:when>
         <xsl:otherwise><xsl:value-of select="$duration2" /></xsl:otherwise>
      </xsl:choose>
   </xsl:variable>
   <xsl:variable name="duration">
      <xsl:if test="starts-with($du1, 'P') and
                    not(translate($du1, '0123456789PYMDTHS.', '')) and
                    starts-with($du2, 'P') and
                    not(translate($du2, '0123456789PYMDTHS.', ''))">
         <xsl:variable name="du1-date">
            <xsl:choose>
               <xsl:when test="contains($du1, 'T')"><xsl:value-of select="substring-before(substring($du1, 2), 'T')" /></xsl:when>
               <xsl:otherwise><xsl:value-of select="substring($du1, 2)" /></xsl:otherwise>
            </xsl:choose>
         </xsl:variable>
         <xsl:variable name="du1-time">
            <xsl:if test="contains($du1, 'T')"><xsl:value-of select="substring-after($du1, 'T')" /></xsl:if>
         </xsl:variable>
         <xsl:variable name="du2-date">
            <xsl:choose>
               <xsl:when test="contains($du2, 'T')"><xsl:value-of select="substring-before(substring($du2, 2), 'T')" /></xsl:when>
               <xsl:otherwise><xsl:value-of select="substring($du2, 2)" /></xsl:otherwise>
            </xsl:choose>
         </xsl:variable>
         <xsl:variable name="du2-time">
            <xsl:if test="contains($du2, 'T')"><xsl:value-of select="substring-after($du2, 'T')" /></xsl:if>
         </xsl:variable>
         <xsl:if test="(not($du1-date) or
                        (not(translate($du1-date, '0123456789YMD', '')) and
                         not(substring-after($du1-date, 'D')) and
                         (contains($du1-date, 'D') or 
                          (not(substring-after($du1-date, 'M')) and
                           (contains($du1-date, 'M') or
                            not(substring-after($du1-date, 'Y'))))))) and
                       (not($du1-time) or
                        (not(translate($du1-time, '0123456789HMS.', '')) and
                         not(substring-after($du1-time, 'S')) and
                         (contains($du1-time, 'S') or
                          not(substring-after($du1-time, 'M')) and
                          (contains($du1-time, 'M') or
                           not(substring-after($du1-time, 'Y')))))) and
                       (not($du2-date) or
                        (not(translate($du2-date, '0123456789YMD', '')) and
                         not(substring-after($du2-date, 'D')) and
                         (contains($du2-date, 'D') or 
                          (not(substring-after($du2-date, 'M')) and
                           (contains($du2-date, 'M') or
                            not(substring-after($du2-date, 'Y'))))))) and
                       (not($du2-time) or
                        (not(translate($du2-time, '0123456789HMS.', '')) and
                         not(substring-after($du2-time, 'S')) and
                         (contains($du2-time, 'S') or
                          not(substring-after($du2-time, 'M')) and
                          (contains($du2-time, 'M') or
                           not(substring-after($du2-time, 'Y'))))))">
            <xsl:variable name="du1y-str">
               <xsl:choose>
                  <xsl:when test="contains($du1-date, 'Y')"><xsl:value-of select="substring-before($du1-date, 'Y')" /></xsl:when>
                  <xsl:otherwise>0</xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="du1m-str">
               <xsl:choose>
                  <xsl:when test="contains($du1-date, 'M')">
                     <xsl:choose>
                        <xsl:when test="contains($du1-date, 'Y')"><xsl:value-of select="substring-before(substring-after($du1-date, 'Y'), 'M')" /></xsl:when>
                        <xsl:otherwise><xsl:value-of select="substring-before($du1-date, 'M')" /></xsl:otherwise>
                     </xsl:choose>
                  </xsl:when>
                  <xsl:otherwise>0</xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="du1d-str">
               <xsl:choose>
                  <xsl:when test="contains($du1-date, 'D')">
                     <xsl:choose>
                        <xsl:when test="contains($du1-date, 'M')"><xsl:value-of select="substring-before(substring-after($du1-date, 'M'), 'D')" /></xsl:when>
                        <xsl:when test="contains($du1-date, 'Y')"><xsl:value-of select="substring-before(substring-after($du1-date, 'Y'), 'D')" /></xsl:when>
                        <xsl:otherwise><xsl:value-of select="substring-before($du1-date, 'D')" /></xsl:otherwise>
                     </xsl:choose>
                  </xsl:when>
                  <xsl:otherwise>0</xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="du1h-str">
               <xsl:choose>
                  <xsl:when test="contains($du1-time, 'H')"><xsl:value-of select="substring-before($du1-time, 'H')" /></xsl:when>
                  <xsl:otherwise>0</xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="du1min-str">
               <xsl:choose>
                  <xsl:when test="contains($du1-time, 'M')">
                     <xsl:choose>
                        <xsl:when test="contains($du1-time, 'H')"><xsl:value-of select="substring-before(substring-after($du1-time, 'H'), 'M')" /></xsl:when>
                        <xsl:otherwise><xsl:value-of select="substring-before($du1-time, 'M')" /></xsl:otherwise>
                     </xsl:choose>
                  </xsl:when>
                  <xsl:otherwise>0</xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="du1s-str">
               <xsl:choose>
                  <xsl:when test="contains($du1-time, 'S')">
                     <xsl:choose>
                        <xsl:when test="contains($du1-time, 'M')"><xsl:value-of select="substring-before(substring-after($du1-time, 'M'), 'S')" /></xsl:when>
                        <xsl:when test="contains($du1-time, 'H')"><xsl:value-of select="substring-before(substring-after($du1-time, 'H'), 'S')" /></xsl:when>
                        <xsl:otherwise><xsl:value-of select="substring-before($du1-time, 'S')" /></xsl:otherwise>
                     </xsl:choose>
                  </xsl:when>
                  <xsl:otherwise>0</xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="mult1" select="($du1-neg * -2) + 1" />
            <xsl:variable name="du1y" select="$du1y-str * $mult1" />
            <xsl:variable name="du1m" select="$du1m-str * $mult1" />
            <xsl:variable name="du1d" select="$du1d-str * $mult1" />
            <xsl:variable name="du1h" select="$du1h-str * $mult1" />
            <xsl:variable name="du1min" select="$du1min-str * $mult1" />
            <xsl:variable name="du1s" select="$du1s-str * $mult1" />

            <xsl:variable name="du2y-str">
               <xsl:choose>
                  <xsl:when test="contains($du2-date, 'Y')"><xsl:value-of select="substring-before($du2-date, 'Y')" /></xsl:when>
                  <xsl:otherwise>0</xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="du2m-str">
               <xsl:choose>
                  <xsl:when test="contains($du2-date, 'M')">
                     <xsl:choose>
                        <xsl:when test="contains($du2-date, 'Y')"><xsl:value-of select="substring-before(substring-after($du2-date, 'Y'), 'M')" /></xsl:when>
                        <xsl:otherwise><xsl:value-of select="substring-before($du2-date, 'M')" /></xsl:otherwise>
                     </xsl:choose>
                  </xsl:when>
                  <xsl:otherwise>0</xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="du2d-str">
               <xsl:choose>
                  <xsl:when test="contains($du2-date, 'D')">
                     <xsl:choose>
                        <xsl:when test="contains($du2-date, 'M')"><xsl:value-of select="substring-before(substring-after($du2-date, 'M'), 'D')" /></xsl:when>
                        <xsl:when test="contains($du2-date, 'Y')"><xsl:value-of select="substring-before(substring-after($du2-date, 'Y'), 'D')" /></xsl:when>
                        <xsl:otherwise><xsl:value-of select="substring-before($du2-date, 'D')" /></xsl:otherwise>
                     </xsl:choose>
                  </xsl:when>
                  <xsl:otherwise>0</xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="du2h-str">
               <xsl:choose>
                  <xsl:when test="contains($du2-time, 'H')"><xsl:value-of select="substring-before($du2-time, 'H')" /></xsl:when>
                  <xsl:otherwise>0</xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="du2min-str">
               <xsl:choose>
                  <xsl:when test="contains($du2-time, 'M')">
                     <xsl:choose>
                        <xsl:when test="contains($du2-time, 'H')"><xsl:value-of select="substring-before(substring-after($du2-time, 'H'), 'M')" /></xsl:when>
                        <xsl:otherwise><xsl:value-of select="substring-before($du2-time, 'M')" /></xsl:otherwise>
                     </xsl:choose>
                  </xsl:when>
                  <xsl:otherwise>0</xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="du2s-str">
               <xsl:choose>
                  <xsl:when test="contains($du2-time, 'S')">
                     <xsl:choose>
                        <xsl:when test="contains($du2-time, 'M')"><xsl:value-of select="substring-before(substring-after($du2-time, 'M'), 'S')" /></xsl:when>
                        <xsl:when test="contains($du2-time, 'H')"><xsl:value-of select="substring-before(substring-after($du2-time, 'H'), 'S')" /></xsl:when>
                        <xsl:otherwise><xsl:value-of select="substring-before($du2-time, 'S')" /></xsl:otherwise>
                     </xsl:choose>
                  </xsl:when>
                  <xsl:otherwise>0</xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="mult2" select="($du2-neg * -2) + 1" />
            <xsl:variable name="du2y" select="$du2y-str * $mult2" />
            <xsl:variable name="du2m" select="$du2m-str * $mult2" />
            <xsl:variable name="du2d" select="$du2d-str * $mult2" />
            <xsl:variable name="du2h" select="$du2h-str * $mult2" />
            <xsl:variable name="du2min" select="$du2min-str * $mult2" />
            <xsl:variable name="du2s" select="$du2s-str * $mult2" />

            <xsl:variable name="min-s" select="60" />
            <xsl:variable name="hour-s" select="60 * 60" />
            <xsl:variable name="day-s" select="60 * 60 * 24" />

            <xsl:variable name="seconds" select="($du1s + $du2s) +
                                                 (($du1min + $du2min) * $min-s) +
                                                 (($du1h + $du2h) * $hour-s) +
                                                 (($du1d + $du2d) * $day-s)" />
            <xsl:variable name="months" select="($du1m + $du2m) +
                                                (($du1y + $du2y) * 12)" />


            <xsl:choose>
               <xsl:when test="($months * $seconds) &lt; 0" />
               <xsl:when test="$months or $seconds">
                  <xsl:if test="$months &lt; 0 or $seconds &lt; 0">-</xsl:if>
                  <xsl:text>P</xsl:text>
                  <xsl:variable name="m" select="$months * ((($months >= 0) * 2) - 1)" />
                  <xsl:variable name="years" select="floor($m div 12)" />
                  <xsl:variable name="mnths" select="$m - ($years * 12)" />
                  <xsl:variable name="s" select="$seconds * ((($seconds >= 0) * 2) - 1)" />
                  <xsl:variable name="days" select="floor($s div $day-s)" />
                  <xsl:variable name="hours" select="floor(($s - ($days * $day-s)) div $hour-s)" />
                  <xsl:variable name="mins" select="floor(($s - ($days * $day-s) - ($hours * $hour-s)) div $min-s)" />
                  <xsl:variable name="secs" select="$s - ($days * $day-s) - ($hours * $hour-s) - ($mins * $min-s)" />
                  <xsl:if test="$years">
                     <xsl:value-of select="$years" />
                     <xsl:text>Y</xsl:text>
                  </xsl:if>
                  <xsl:if test="$mnths">
                     <xsl:value-of select="$mnths" />
                     <xsl:text>M</xsl:text>
                  </xsl:if>
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
               </xsl:when>
               <xsl:otherwise>P</xsl:otherwise>
            </xsl:choose>
         </xsl:if>
      </xsl:if>
   </xsl:variable>
   <xsl:value-of select="$duration" />
</xsl:template>

</xsl:stylesheet>