<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:eac-cpf="urn:isbn:1-931666-33-4"
    xmlns:oai-dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
    xmlns:dc="http://purl.org/dc/elements/1.1/"
    exclude-result-prefixes="eac-cpf">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <xsl:template match="eac-cpf:eac-cpf">
    <oai-dc:dc>
      <dc:identifier><xsl:value-of select="eac-cpf:control/eac-cpf:recordId/text()"/></dc:identifier>
      <xsl:apply-templates select="eac-cpf:cpfDescription"/>
    </oai-dc:dc>
  </xsl:template>

  <xsl:template match="eac-cpf:entityType">
    <dc:type><xsl:value-of select="text()"/></dc:type>
  </xsl:template>

  <xsl:template match="eac-cpf:nameEntry">
    <xsl:choose>
      <xsl:when test="@localType='primary'">
        <dc:title><xsl:call-template name="eac-cpf_name"/></dc:title>
      </xsl:when>
      <xsl:otherwise>
        <dc:alternative><xsl:call-template name="eac-cpf_name"/></dc:alternative>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="eac-cpf_name">
    <xsl:variable name="last_name" select="eac-cpf:part[@localType='surname']/text()"/>
    <xsl:variable name="first_name" select="eac-cpf:part[@localType!='surname']/text()"/>

    <xsl:choose>
      <xsl:when test="$last_name and $first_name">
        <xsl:value-of select="normalize-space(concat($last_name, ', ', $first_name))"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="normalize-space(concat($last_name, $first_name))"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="eac-cpf:existDates/eac-cpf:dateRange">
    <dc:date><xsl:value-of select="normalize-space(concat(eac-cpf:fromDate/@standardDate, '/', eac-cpf:toDate/@standardDate))"/></dc:date>
  </xsl:template>

  <xsl:template match="*">
    <xsl:apply-templates/>
  </xsl:template>
</xsl:stylesheet>


