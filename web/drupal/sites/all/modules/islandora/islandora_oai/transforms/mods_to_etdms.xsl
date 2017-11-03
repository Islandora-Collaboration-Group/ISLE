<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns="http://www.ndltd.org/standards/metadata/etdms/1.0/"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns:mods="http://www.loc.gov/mods/v3" exclude-result-prefixes="mods srw_dc"
xmlns:srw_dc="info:srw/schema/1/dc-schema"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns:oai_etdms="http://www.ndltd.org/standards/metadata/etdms/1.0/">

<!--
This stylesheet transforms MODS version 3.2 records and collections of records to simple Dublin Core (DC) records,
based on the Library of Congress' MODS to simple DC mapping <http://www.loc.gov/standards/mods/mods-dcsimple.html>
The stylesheet will transform a collection of MODS 3.2 records into simple Dublin Core (DC)
as expressed by the SRU DC schema <http://www.loc.gov/standards/sru/dc-schema.xsd>

The stylesheet will transform a single MODS 3.2 record into simple Dublin Core (DC)
as expressed by the OAI DC schema <http://www.openarchives.org/OAI/2.0/oai_dc.xsd>
Because MODS is more granular than DC, transforming a given MODS element or subelement to a DC element frequently results in less precise tagging,
and local customizations of the stylesheet may be necessary to achieve desired results.

This stylesheet makes the following decisions in its interpretation of the MODS to simple DC mapping:
When the roleTerm value associated with a name is creator, then name maps to dc:creator
When there is no roleTerm value associated with name, or the roleTerm value associated with name is a value other than creator, then name maps to dc:contributor
Start and end dates are presented as span dates in dc:date and in dc:coverage
When the first subelement in a subject wrapper is topic, subject subelements are strung together in dc:subject with hyphens separating them
Some subject subelements, i.e., geographic, temporal, hierarchicalGeographic, and cartographics, are also parsed into dc:coverage
The subject subelement geographicCode is dropped in the transform

Revision 1.2 2008-03-12 <chick@diglib.org>
Modified to return only on identifier element, that one selected is
based on the Aquifer MODS documents
Revision 1.1 2007-05-18 <tmee@loc.gov>
Added modsCollection conversion to DC SRU
Updated introductory documentation
Version 1.0 2007-05-04 Tracy Meehleib <tmee@loc.gov>

-->

<xsl:output method="xml" omit-xml-declaration="yes" indent="yes"/>
<xsl:strip-space elements="*"/>

<xsl:template match="/">
<xsl:choose>
<xsl:when test="//mods:modsCollection">
<srw_dc:dcCollection xsi:schemaLocation="info:srw/schema/1/dc-schema http://www.loc.gov/standards/sru/dc-schema.xsd">
<xsl:apply-templates/>
<xsl:for-each select="mods:modsCollection/mods:mods">
<srw_dc:dc xsi:schemaLocation="info:srw/schema/1/dc-schema http://www.loc.gov/standards/sru/dc-schema.xsd">
<xsl:apply-templates/>
</srw_dc:dc>
</xsl:for-each>
</srw_dc:dcCollection>
</xsl:when>
<xsl:otherwise>
<xsl:for-each select="mods:mods">
    <oai_etdms:thesis xmlns="http://www.ndltd.org/standards/metadata/etdms/1.0/" xmlns:oai_etdms="http://www.ndltd.org/standards/metadata/etdms/1.0/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.ndltd.org/standards/metadata/etdms/1.0/ http://www.ndltd.org/standards/metadata/etdms/1-0/etdms.xsd">
        <xsl:call-template name="title"/>
        <xsl:call-template name="creator"/>
        <xsl:call-template name="subject"/>
        <xsl:call-template name="description"/>
        <xsl:call-template name="publisher"/>
        <xsl:call-template name="contributor"/>
        <xsl:call-template name="date"/>
        <xsl:call-template name="type"/>
        <xsl:call-template name="format"/>
        <xsl:call-template name="identifier"/>
        <xsl:call-template name="language"/>
        <xsl:call-template name="rights"/>
        <xsl:call-template name="degree"/>
        <xsl:apply-templates/>
</oai_etdms:thesis>
</xsl:for-each>
</xsl:otherwise>
</xsl:choose>
</xsl:template>

<xsl:template name="title">
    <xsl:for-each select="//mods:titleInfo">
<title>
<xsl:value-of select="mods:nonSort"/>
    <xsl:if test="normalize-space(mods:nonSort)">
<xsl:if test="mods:nonSort">
<xsl:text> </xsl:text>
</xsl:if>
    </xsl:if>
<xsl:value-of select="mods:title"/>
<xsl:if test="mods:subTitle">
    <xsl:if test="normalize-space(mods:subTitle)">
<xsl:text>: </xsl:text>
<xsl:value-of select="mods:subTitle"/>
    </xsl:if>
</xsl:if>
<xsl:if test="mods:partNumber">
    <xsl:if test="normalize-space(mods:partNumber)">
<xsl:text>. </xsl:text>
<xsl:value-of select="mods:partNumber"/>
    </xsl:if>
</xsl:if>
<xsl:if test="mods:partName">
    <xsl:if test="normalize-space(mods:partName)">
<xsl:text>. </xsl:text>
<xsl:value-of select="mods:partName"/>
    </xsl:if>
</xsl:if>
</title>
    </xsl:for-each>
</xsl:template>

<xsl:template name="creator">
    <xsl:for-each select="mods:name">
<xsl:if test="mods:role/mods:roleTerm[@type='text']='author' or mods:role/mods:roleTerm[@type='code']='aut' ">
<creator>
<xsl:call-template name="name"/>
</creator>
</xsl:if>
    </xsl:for-each>
</xsl:template>

<xsl:template name="subject">
    <xsl:for-each select="mods:subject[mods:topic | mods:name | mods:occupation | mods:geographic | mods:hierarchicalGeographic | mods:cartographics | mods:temporal] ">
<xsl:for-each select="mods:topic">
    <xsl:if test="normalize-space(.)">
    <subject>
<xsl:value-of select="."/>
    </subject>
    </xsl:if>
</xsl:for-each>
        <xsl:for-each select="mods:occupation">
    <xsl:if test="normalize-space(mods:occupation)">
        <subject>
    <xsl:if test="normalize-space(.)">
<xsl:value-of select="."/>
<xsl:if test="position()!=last()">--</xsl:if>
    </xsl:if>
        </subject>
    </xsl:if>
        </xsl:for-each>
    
<xsl:for-each select="mods:name">
    <xsl:if test="normalize-space(.)">
        <subject>
<xsl:call-template name="name"/>
        </subject>
    </xsl:if>
</xsl:for-each>


<xsl:for-each select="mods:titleInfo/mods:title">
    <xsl:if test="normalize-space(.)">
<subject>
<xsl:value-of select="mods:titleInfo/mods:title"/>
</subject>
    </xsl:if>
</xsl:for-each>
<xsl:for-each select="mods:geographic">
    <xsl:if test="normalize-space(.)">
        <coverage>
        <xsl:value-of select="."/>
        </coverage>
    </xsl:if>
</xsl:for-each>
<xsl:for-each select="mods:hierarchicalGeographic">
    <xsl:if test="normalize-space(.)">
        <coverage>
<xsl:for-each
select="mods:continent|mods:country|mods:provence|mods:region|mods:state|mods:territory|mods:county|mods:city|mods:island|mods:area">
<xsl:value-of select="."/>
<xsl:if test="position()!=last()"></xsl:if>
</xsl:for-each>
        </coverage>
    </xsl:if>
</xsl:for-each>
<xsl:for-each select="mods:cartographics/*">
    <xsl:if test="normalize-space(.)">
<coverage>
<xsl:value-of select="."/>
</coverage>
        </xsl:if>
</xsl:for-each>
<xsl:if test="mods:temporal">
    <xsl:if test="normalize-space(mods:temporal)">
<coverage>
<xsl:for-each select="mods:temporal">
    <xsl:if test="normalize-space(mods:temporal)">
<xsl:value-of select="."/>
<xsl:if test="position()!=last()">-</xsl:if>
    </xsl:if>
</xsl:for-each>
</coverage>
    </xsl:if>
</xsl:if>
<xsl:if test="*[1][local-name()='topic'] and *[local-name()!='topic']">
    <xsl:if test="normalize-space(.)">
<!--<subject>
<xsl:for-each select="*[local-name()!='cartographics' and local-name()!='geographicCode' and local-name()!='hierarchicalGeographic'] ">
    <xsl:if test="normalize-space(.)">
<xsl:value-of select="."/>
<xsl:if test="position()!=last()">*-</xsl:if>
    </xsl:if>
</xsl:for-each>
</subject>-->
    </xsl:if>
</xsl:if>
    </xsl:for-each>
    <xsl:for-each select="mods:classification">
        <subject>
            <xsl:value-of select="."/>
        </subject>
    </xsl:for-each>
</xsl:template>

<xsl:template name="description">
    <xsl:for-each select="//mods:abstract | //mods:tableOfContents | //mods:note">
    <xsl:if test="normalize-space(.)">
<description>
<xsl:value-of select="."/>
</description>
    </xsl:if>
    </xsl:for-each>
</xsl:template>

<xsl:template name="date">
    <xsl:for-each select="mods:originInfo">
<xsl:apply-templates select="*[@point='start']"/>
<xsl:for-each
select="mods:dateIssued[@point!='start' and @point!='end'] |mods:dateCreated[@point!='start' and @point!='end'] | mods:dateCaptured[@point!='start' and @point!='end'] | mods:dateOther[@point!='start' and @point!='end']">
<date>
<xsl:value-of select="."/>
</date>
</xsl:for-each>
    </xsl:for-each>
    <xsl:for-each select="//mods:dateIssued | //mods:dateCreated | //mods:dateCaptured">
        <xsl:if test="normalize-space(.)">
        <date>
            <xsl:choose>
                <xsl:when test="@point='start'">
                    <xsl:value-of select="."/>
                    <xsl:text> - </xsl:text>
                </xsl:when>
                <xsl:when test="@point='end'">
                    <xsl:value-of select="."/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="."/>
                </xsl:otherwise>
            </xsl:choose>
        </date>
        </xsl:if>
    </xsl:for-each>
    <xsl:for-each select="mods:dateIssued[@point='start'] | mods:dateCreated[@point='start'] | mods:dateCaptured[@point='start'] | mods:dateOther[@point='start'] ">
        <xsl:variable name="dateName" select="local-name()"/>
        <date>
            <xsl:value-of select="."/>-<xsl:value-of select="../*[local-name()=$dateName][@point='end']"/>
        </date>
    </xsl:for-each>
</xsl:template>

<xsl:template name="publisher">
    <xsl:for-each select="mods:originInfo">
<xsl:for-each select="mods:publisher">
<publisher>
<xsl:value-of select="."/>
</publisher>
</xsl:for-each>
    </xsl:for-each>
</xsl:template>

<xsl:template name="contributor">
    <xsl:for-each select="//mods:name">
    <xsl:if test="mods:role/mods:roleTerm/text()!='author'">
        <xsl:if test="normalize-space(mods:namePart[@type='given'])">
        <contributor>
            <xsl:attribute name="role">
                <xsl:value-of select="mods:role/mods:roleTerm/text()"/>
            </xsl:attribute>
    <xsl:value-of select="concat(mods:namePart[@type='given'], ' ', mods:namePart[@type='family'])"/>
        </contributor>
        </xsl:if>
    </xsl:if>
    </xsl:for-each>
</xsl:template>

<xsl:template name="type">
    <xsl:for-each select="//mods:genre">
<xsl:choose>
<xsl:when test="@authority='dct'">
<type>
<xsl:value-of select="."/>
</type>
<xsl:for-each select="mods:typeOfResource">
<type>
<xsl:value-of select="."/>
</type>
</xsl:for-each>
</xsl:when>
<xsl:otherwise>
<type>
<xsl:value-of select="."/>
</type>
</xsl:otherwise>
</xsl:choose>
    </xsl:for-each>
<xsl:for-each select="mods:typeOfResource">
<xsl:if test="@collection='yes'">
<type>Collection</type>
</xsl:if>
<xsl:if test=". ='software' and ../mods:genre='database'">
<type>DataSet</type>
</xsl:if>
<xsl:if test=".='software' and ../mods:genre='online system or service'">
<type>Service</type>
</xsl:if>
<xsl:if test=".='software'">
<type>Software</type>
</xsl:if>
<xsl:if test=".='cartographic material'">
<type>Image</type>
</xsl:if>
<xsl:if test=".='multimedia'">
<type>InteractiveResource</type>
</xsl:if>
<xsl:if test=".='moving image'">
<type>MovingImage</type>
</xsl:if>
<xsl:if test=".='three-dimensional object'">
<type>PhysicalObject</type>
</xsl:if>
<xsl:if test="starts-with(.,'sound recording')">
<type>Sound</type>
</xsl:if>
<xsl:if test=".='still image'">
<type>StillImage</type>
</xsl:if>
<xsl:if test=". ='text'">
<type>Text</type>
</xsl:if>
<xsl:if test=".='notated music'">
<type>Text</type>
</xsl:if>
</xsl:for-each>
</xsl:template>

<xsl:template name="format">
    <xsl:for-each select="mods:physicalDescription">
<xsl:if test="mods:extent">
    <xsl:if test="normalize-space(mods:extent)">
<format>
<xsl:value-of select="mods:extent"/>
</format>
    </xsl:if>
</xsl:if>
<xsl:if test="mods:form">
    <xsl:if test="mods:form[@authority!='local']">
    <xsl:if test="normalize-space(mods:form)">
        <format>
<xsl:value-of select="mods:form"/>
        </format>
    </xsl:if>
    </xsl:if>
</xsl:if>
<xsl:if test="mods:internetMediaType">
    <xsl:if test="normalize-space(mods:internetMediaType)">
        <format>
<xsl:value-of select="mods:internetMediaType"/>
        </format>
    </xsl:if>
</xsl:if>
    </xsl:for-each>
<xsl:for-each select="mods:mimeType">
    <xsl:if test="normalize-space(mods:mimeType)">
        <format>
<xsl:value-of select="."/>
        </format>
    </xsl:if>
</xsl:for-each>
</xsl:template>

<xsl:template name="identifier">
    <xsl:for-each select="mods:identifier">
<xsl:variable name="type" select="translate(@type,'ABCDEFGHIJKLMNOPQRSTUVWXYZ','abcdefghijklmnopqrstuvwxyz')"/>
<xsl:if test="contains ('isbn issn uri doi lccn uri pdf lac', $type)">
    <xsl:if test="normalize-space(.)">
<identifier>
<xsl:value-of select="."/>
</identifier>
    </xsl:if>
</xsl:if>
    </xsl:for-each>
    <xsl:for-each select="mods:identifier">
        <xsl:if test="normalize-space(mods:identifier[@type='pid'])">
    <identifier>
        <xsl:value-of select="mods:identifier[@type='pid']"/>
    </identifier>
        </xsl:if>
    </xsl:for-each>
    <xsl:for-each select="mods:location">
        <xsl:if test="normalize-space(./mods:url)">
            <identifier>
                <xsl:value-of select="./mods:url"/>
            </identifier>
        </xsl:if>
    </xsl:for-each>
</xsl:template>
<xsl:template name="language">
    <xsl:for-each select="mods:language">
<language>
<xsl:value-of select="normalize-space(.)"/>
</language>
    </xsl:for-each>
</xsl:template>

<xsl:template match="mods:relatedItem[mods:titleInfo | mods:name | mods:identifier | mods:location]">
<xsl:choose>
<xsl:when test="@type='original'">
<source>
<xsl:for-each
select="mods:titleInfo/mods:title | mods:identifier | mods:location/mods:url">
<xsl:if test="normalize-space(.)!= ''">
<xsl:value-of select="."/>
<xsl:if test="position()!=last()">--</xsl:if>
</xsl:if>
</xsl:for-each>
</source>
</xsl:when>
<xsl:when test="@type='series'"/>
<xsl:otherwise>
<relation>
<xsl:for-each
select="mods:titleInfo/mods:title | mods:identifier | mods:location/mods:url">
<xsl:if test="normalize-space(.)!= ''">
<xsl:value-of select="."/>
<xsl:if test="position()!=last()">--</xsl:if>
</xsl:if>
</xsl:for-each>
</relation>
</xsl:otherwise>
</xsl:choose>
</xsl:template>

<xsl:template name="rights">
    <xsl:for-each select="mods:accessCondition">
<rights>
<xsl:value-of select="."/>
</rights>
    </xsl:for-each>
</xsl:template>

<xsl:template name="name">
<xsl:variable name="name">
    <xsl:for-each select="mods:roleTerm">
        <xsl:if test="mods:roleTerm = 'author'">
        </xsl:if>
    </xsl:for-each>
<xsl:for-each select="mods:namePart[not(@type)]">
<xsl:value-of select="."/>
<xsl:text> </xsl:text>
</xsl:for-each>
<xsl:value-of select="mods:namePart[@type='family']"/>
<xsl:if test="mods:namePart[@type='given']">
<xsl:text>, </xsl:text>
<xsl:value-of select="mods:namePart[@type='given']"/>
</xsl:if>
<xsl:if test="mods:namePart[@type='date']">
<xsl:text>, </xsl:text>
<xsl:value-of select="mods:namePart[@type='date']"/>
<xsl:text/>
</xsl:if>
<xsl:if test="mods:displayForm">
<xsl:text> (</xsl:text>
<xsl:value-of select="mods:displayForm"/>
<xsl:text>) </xsl:text>
</xsl:if>
<!--<xsl:for-each select="mods:role[mods:roleTerm[@type='text']!='creator']">
<xsl:text> (</xsl:text>
<xsl:value-of select="normalize-space(.)"/>
<xsl:text>) </xsl:text>
</xsl:for-each>-->
</xsl:variable>
<xsl:value-of select="normalize-space($name)"/>
</xsl:template>

<!--<xsl:template match="mods:temporal[@point='start'] ">
<xsl:value-of select="."/>-<xsl:value-of select="../mods:temporal[@point='end']"/>
</xsl:template>

<xsl:template match="mods:temporal[@point!='start' and @point!='end'] ">
<xsl:value-of select="."/>
</xsl:template>-->

<xsl:template name="degree">
    <xsl:for-each select="mods:extension">
    <degree>
        <name>
            <xsl:value-of select="*/*[local-name()='name']"/>
        </name>
        <level>
            <xsl:value-of select="*/*[local-name()='level']"/>
        </level>
        <discipline>
            <xsl:value-of select="*/*[local-name()='discipline']"/>
        </discipline>
        <grantor>
            <xsl:value-of select="*/*[local-name()='grantor']"/>
        </grantor>
    </degree>
    </xsl:for-each>
</xsl:template>

<!-- suppress all else:-->
<xsl:template match="*"/>

<!--
<extension xmlns:etd="http://www.ndltd.org/standards/metadata/etdms/1-0/etdms.xsd" >

	<etd:degree>
  	  	<etd:name>Doctor of Philosophy</etd:name>
  	  	<etd:level>Doctoral</etd:level>
  	  	<etd:discipline>Educational Administration</etd:discipline>
  	</etd:degree>
</extension>
-->

</xsl:stylesheet>
