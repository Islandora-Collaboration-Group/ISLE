<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" 
  xmlns:mads="http://www.loc.gov/mads/v2"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

  
  <!-- Remove leading / trailing / repeating white space from all elements -->
  <xsl:strip-space elements="*"/>
  
  <xsl:template match="node() | @*">
    <xsl:copy>
      <xsl:apply-templates select="node() | @*"/>
    </xsl:copy>
  </xsl:template>
  
  <xsl:template match="*[normalize-space() = '']"/>
  
  <!-- Reorder mads:mads child elements -->
  <xsl:template match="mads:mads">
    <xsl:copy>
      <xsl:apply-templates select="@*"/>
      <xsl:apply-templates select="mads:authority"/>
      <xsl:apply-templates select="mads:related"/>
      <xsl:apply-templates select="mads:variant"/>
      <xsl:apply-templates select="mads:affiliation"/>
      <xsl:apply-templates select="mads:classification"/>
      <xsl:apply-templates select="mads:fieldOfActivity"/>
      <xsl:apply-templates select="mads:identifier"/>
      <xsl:apply-templates select="mads:language"/>
      <xsl:apply-templates select="mads:note"/>
      <xsl:apply-templates select="mads:url"/>
      <xsl:apply-templates select="mads:extension"/>
      <xsl:apply-templates select="mads:recordInfo"/>
    </xsl:copy>
  </xsl:template>
  
  <!-- Reorder <mads:authority> child elements -->
  <xsl:template match="mads:authority">
    <xsl:copy>
      <xsl:apply-templates select="mads:name"/>
      <xsl:apply-templates select="mads:titleInfo"/>
      <xsl:apply-templates select="mads:topic"/> 
      <xsl:apply-templates select="mads:temporal"/> 
      <xsl:apply-templates select="mads:genre"/>
      <xsl:apply-templates select="mads:geographic"/>
      <xsl:apply-templates select="mads:hierarchicalGeographic"/> 
      <xsl:apply-templates select="mads:occupation"/> 
    </xsl:copy>
  </xsl:template>
  
  <!-- Reorder <mads:related> child elements -->
  <xsl:template match="mads:related">
    <xsl:copy>
      <xsl:apply-templates select="mads:name"/>
      <xsl:apply-templates select="mads:titleInfo"/>
      <xsl:apply-templates select="mads:topic"/> 
      <xsl:apply-templates select="mads:temporal"/> 
      <xsl:apply-templates select="mads:genre"/>
      <xsl:apply-templates select="mads:geographic"/>
      <xsl:apply-templates select="mads:hierarchicalGeographic"/> 
      <xsl:apply-templates select="mads:occupation"/> 
    </xsl:copy>
  </xsl:template>
  <!-- Reorder <mads:variant> child elements -->
  <xsl:template match="mads:variant">
    <xsl:copy>
    <xsl:apply-templates select="mads:name"/>
    <xsl:apply-templates select="mads:titleInfo"/>
    <xsl:apply-templates select="mads:topic"/> 
    <xsl:apply-templates select="mads:temporal"/> 
    <xsl:apply-templates select="mads:genre"/>
    <xsl:apply-templates select="mads:geographic"/>
    <xsl:apply-templates select="mads:hierarchicalGeographic"/> 
    <xsl:apply-templates select="mads:occupation"/>
    </xsl:copy>
  </xsl:template>
  

  <!-- Reorder <mads:name> child elements -->
  <xsl:template match="mads:name">
    <xsl:copy>
      <xsl:apply-templates select="mads:namePart"/>
      <xsl:apply-templates select="mads:description"/>
    </xsl:copy>
  </xsl:template>
    
  <!-- Reorder <mads:affiliation> child elements -->
  <xsl:template match="mads:affiliation">
    <xsl:copy>
      <xsl:apply-templates select="mads:position"/>
      <xsl:apply-templates select="mads:organization"/>
      <xsl:apply-templates select="mads:address"/> 
      <xsl:apply-templates select="mads:email"/> 
      <xsl:apply-templates select="mads:phone"/>
      <xsl:apply-templates select="mads:fax"/>
      <xsl:apply-templates select="mads:hours"/> 
      <xsl:apply-templates select="mads:dateValid"/> 
    </xsl:copy>
  </xsl:template>
  

  <!-- Reorder <mads:titleInfo> child elements -->
  <xsl:template match="mads:titleInfo">
    <xsl:copy>
      <xsl:apply-templates select="mads:nonSort"/>
      <xsl:apply-templates select="mads:title"/>
      <xsl:apply-templates select="mads:subTitle"/>
      <xsl:apply-templates select="mads:partNumber"/>
      <xsl:apply-templates select="mads:partName"/>
     </xsl:copy>
  </xsl:template>
  
  
  <!-- Reorder <mads:hierarchicalGeographic> child elements -->
  <xsl:template match="mads:hierarchicalGeographic">
    <xsl:copy>
      <xsl:apply-templates select="mads:continent"/>
      <xsl:apply-templates select="mads:country"/>
      <xsl:apply-templates select="mads:province"/>
      <xsl:apply-templates select="mads:region"/>
      <xsl:apply-templates select="mads:state"/>
      <xsl:apply-templates select="mads:territory"/>
      <xsl:apply-templates select="mads:county"/>
      <xsl:apply-templates select="mads:city"/>
      <xsl:apply-templates select="mads:island"/>
      <xsl:apply-templates select="mads:area"/>
      <xsl:apply-templates select="mads:extraterrestrialArea"/>
      <xsl:apply-templates select="mads:citySection"/>
    </xsl:copy>
  </xsl:template>
    
  <!-- Reorder <mads:address> child elements -->
  <xsl:template match="mads:address">
    <xsl:copy>
      <xsl:apply-templates select="mads:street"/>
      <xsl:apply-templates select="mads:city"/>
      <xsl:apply-templates select="mads:state"/> 
      <xsl:apply-templates select="mads:country"/> 
      <xsl:apply-templates select="mads:postcode"/>
    </xsl:copy>
  </xsl:template>
  
  <!-- Reorder <mads:language> child elements -->
  <xsl:template match="mads:language">
    <xsl:copy>
      <xsl:apply-templates select="mads:languageTerm"/>
      <xsl:apply-templates select="mads:scriptTerm"/>
    </xsl:copy>
  </xsl:template>


  <!-- Reorder <mads:recordInfo> child elements -->
  <xsl:template match="mads:recordInfo">
    <xsl:copy>
      <xsl:apply-templates select="mads:recordContentSource"/>
      <xsl:apply-templates select="mads:recordCreationDate"/>
      <xsl:apply-templates select="mads:recordChangeDate"/> 
      <xsl:apply-templates select="mads:recordIdentifier"/> 
      <xsl:apply-templates select="mads:recordOrigin"/>
      <xsl:apply-templates select="mads:languageOfCataloging"/>
      <xsl:apply-templates select="mads:descriptionStandard"/>  
    </xsl:copy>
  </xsl:template>
  
  <!-- Reorder <mads:languageOfCataloging> child elements -->
  <xsl:template match="mads:languageOfCataloging">
    <xsl:copy>
      <xsl:apply-templates select="mads:languageTerm"/>
      <xsl:apply-templates select="mads:scriptTerm"/>
    </xsl:copy>
  </xsl:template>
  
 
</xsl:stylesheet>
