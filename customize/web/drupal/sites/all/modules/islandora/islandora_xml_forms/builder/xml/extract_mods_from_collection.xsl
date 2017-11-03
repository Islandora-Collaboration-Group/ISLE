<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output encoding="UTF-8" indent="yes" media-type="text/xml" method="xml" version="1.0"/>
  <xsl:strip-space elements="*"/>
  <xsl:template match="*[not(node())]"/>
  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()[normalize-space()]|@*[normalize-space()]"/>
    </xsl:copy>
  </xsl:template>
  <!-- We only care about mods:modsCollections that only have a single mods:mods document, 
       ones that have multiple are another can of beans -->
  <xsl:template match="/mods:modsCollection[count(mods:mods) = 1]">
    <xsl:for-each select="mods:mods">
      <xsl:copy>
        <!-- Copy the Parent mods:modsCollection attributes as well -->
        <xsl:apply-templates select="../@*[normalize-space()]"/>
        <xsl:apply-templates select="node()[normalize-space()]|@*[normalize-space()]"/>
      </xsl:copy>
    </xsl:for-each>
  </xsl:template>
</xsl:stylesheet>
