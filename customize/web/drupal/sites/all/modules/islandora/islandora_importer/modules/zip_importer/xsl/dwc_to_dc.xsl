<?xml version="1.0" encoding="UTF-8"?>

<!-- 
    Created by Paul R Butler
    University of Illinois - Graduate School of Library and Information Science
    Final Project for LIS590DPL: Document Processing - Spring 2010 
    Last Updated: May 5, 2010
    
    This XSLT transforms DarwinCore XML to Simple Dublin Core XML. 
-->
<!--

Edits and local customization by dm 
... DwC to OAI_DC June 2010.

-->

<xsl:stylesheet version="1.0"    
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:dc="http://purl.org/dc/terms/"
    xmlns:dwc="http://rs.tdwg.org/dwc/terms/" xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc.xsd">
  
    <xsl:output method="xml" indent="yes" />   
    
    <xsl:template match="/">
               
        <xsl:text> <!-- Used for formatting output file.  -->     
</xsl:text>
                
        <xsl:text> <!-- Used for formatting output file.  -->   
</xsl:text>
      
        <oai_dc:dc xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
            xsi:noNamespaceSchemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc.xsd">
            
            <xsl:apply-templates/>
           
        </oai_dc:dc>

    </xsl:template>
    
    <!-- Source elemnts are Dublin Core. -->
    <xsl:template match="dc:type">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <type xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></type>  <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>                
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template match="dc:language">        
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <language xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></language> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>               
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>


    <!-- Source elemnts are Darwin Core. -->
    <xsl:template match="dwc:basisOfRecord">      
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <source xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></source> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>             
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template match="dwc:scientificName">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <title xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></title> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>             
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>        
    </xsl:template>

    <xsl:template match="dwc:specificEpithet">      
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <subject xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></subject> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>            
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>   
    </xsl:template>
    
    <xsl:template match="dwc:family">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <subject xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></subject> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>            
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>   
    </xsl:template>

    <xsl:template match="dwc:genus">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <subject xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></subject> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>            
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>   
    </xsl:template>

    <xsl:template match="dwc:vernacularName">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <title xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></title> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>            
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>   
    </xsl:template>

    <xsl:template match="dwc:continent">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <coverage xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></coverage> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>        
    </xsl:template>       
    
    <xsl:template match="dwc:country">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <coverage xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></coverage> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
    
    <xsl:template match="dwc:countryCode">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <coverage xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></coverage> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
    
    <xsl:template match="dwc:stateProvince">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <coverage xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></coverage> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template match="dwc:county">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <coverage xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></coverage> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template match="dwc:locality">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <coverage xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></coverage> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template match="dwc:habitat">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <coverage xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></coverage> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template match="dwc:occurrenceRemarks">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <description xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></description> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>      
    </xsl:template>

    <xsl:template match="dwc:occurrenceID">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <identifier xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></identifier> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>           
    </xsl:template>

    <xsl:template match="dwc:institutionCode">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <publisher xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></publisher> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>      
      
    </xsl:template>

    <xsl:template match="dwc:collectionCode">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <identifier xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></identifier> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose> 
    </xsl:template>
    
    <xsl:template match="dwc:catalogNumber">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <identifier xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></identifier> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose> 
    </xsl:template>
    
    <xsl:template match="dwc:recordedBy">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <creator xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></creator> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>  
    </xsl:template>
    
    <xsl:template match="dwc:eventDate">
        <xsl:choose>
            <xsl:when test="normalize-space(.)"> <!-- Tests to see if normalized element is null.  If it is, does not output element to source file. -->
                <date xmlns="http://purl.org/dc/elements/1.1/"><xsl:apply-templates/></date> <xsl:text> </xsl:text><xsl:comment> Source node is <xsl:value-of select="name()"/> </xsl:comment>
            </xsl:when>
            <xsl:otherwise> <!-- This is for when the element is null. -->
                <xsl:comment> Node <xsl:value-of select="name()"/> had no content.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>        
      
    </xsl:template>
    
    <!-- Exception Handling - If an element was not matched above this section should output a message and the element as a comment to the ouptput file. -->
    <!-- For Dublin Core elements. -->
    <xsl:template match="dc:*">
        <xsl:choose>
            <xsl:when test="dc:type | dc:language">
            </xsl:when>
            <xsl:otherwise>
                <xsl:comment> Node <xsl:value-of select="name()"/> with content "<xsl:apply-templates/>" was not matched.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
    
    <!-- For Darwin Core elements. -->
    <xsl:template match="dwc:*">
        <xsl:choose>
            <xsl:when test="dwc:basisOfRecord | dwc:scientificName | dwc:specificEpithet | dwc:family | dwc:genus | dwc:vernacularName | dwc:continent | dwc:country | dwc:countryCode | dwc:stateProvince | dwc:county | dwc:locality | dwc:habitat | dwc:occurrenceRemarks | dwc:occurrenceID | dwc:institutionCode | dwc:collectionCode | dwc:catalogNumber | dwc:recordedBy | dwc:eventDate">
            </xsl:when>
            <xsl:otherwise>
                <xsl:comment> Node <xsl:value-of select="name()"/> with content "<xsl:apply-templates/>" was not matched.</xsl:comment>
            </xsl:otherwise>
        </xsl:choose>
            
    </xsl:template>
    <!-- End of Exception Handling section. -->
    
</xsl:stylesheet>