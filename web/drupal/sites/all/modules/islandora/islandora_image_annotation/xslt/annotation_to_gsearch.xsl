<?xml version="1.0" encoding="UTF-8"?>

<!--
    Document   : annotation_to_gsearch.xsl
    Created on : November 13, 2012, 11:49 AM
    Author     : ppound
    Description:
        reads an OAC datastream and indexes specified fields in solr using gsearch.
        Put this file in the same dir as your gsearch xslt and then add an include to the existing gsearch xslt
        add this to your gsearch xslt using <xsl:include href="annotation_to_gsearch.xsl"/>
        then you will need to call the annotations template somewhere from within the gsearch xslt
-->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"   
    xmlns:foxml="info:fedora/fedora-system:def/foxml#" xmlns:dc="http://purl.org/dc/elements/1.1/"   
    xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
    xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#"
    xmlns:fedora="info:fedora/fedora-system:def/relations-external#"
    xmlns:rel="info:fedora/fedora-system:def/relations-external#"   
    xmlns:fedora-model="info:fedora/fedora-system:def/model#"   
    xmlns:oa="http://www.w3.org/ns/openannotation/core/"
    xmlns:cnt="http://www.w3.org/2008/content#" 
    xmlns:rels="http://vre.upei.ca/rels/" xmlns:mads="http://www.loc.gov/mads/v2"
    exclude-result-prefixes="oa cnt">
    
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
    
   
    <xsl:template name = "annotations">
       <!-- we will rely on a PARENT_pid field that is created in our main xslt.  We will need the parent pid in our results if you do not already have a PARENT_pid uncomment the next commented section-->
    <!--  get the parent pid
        <xsl:variable name="memberOf_PID">
            <xsl:value-of
                select="foxml:datastream[@ID='RELS-EXT']/foxml:datastreamVersion[last()]/foxml:xmlContent//rdf:Description/rel:isMemberOf/@rdf:resource | foxml:datastream[@ID='RELS-EXT']/foxml:datastreamVersion[last()]/foxml:xmlContent//rdf:description/rel:isMemberOf/@rdf:resource"/>
        </xsl:variable>
        
        <xsl:variable name="memberOfCollection_PID">
            <xsl:value-of
                select="foxml:datastream[@ID='RELS-EXT']/foxml:datastreamVersion[last()]/foxml:xmlContent//rdf:Description/rel:isMemberOfCollection/@rdf:resource | foxml:datastream[@ID='RELS-EXT']/foxml:datastreamVersion[last()]/foxml:xmlContent//rdf:description/rel:isMemberOfCollection/@rdf:resource"/>
        </xsl:variable>
        
        <xsl:variable name = "PARENT_PID">
            <xsl:choose>
                <xsl:when test="text() [normalize-space($memberOf_PID)] ">
                    <xsl:value-of select="$memberOf_PID"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select='$memberOfCollection_PID'/>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:variable>        
        
        <field>
            <xsl:attribute name="name">PARENT_pid</xsl:attribute>
            <xsl:value-of select="substring($PARENT_PID,13)"/>
        </field>
    -->
    
    <xsl:for-each
            select="foxml:datastream[@ID='RELS-EXT']/foxml:datastreamVersion[last()]/foxml:xmlContent//rdf:Description/fedora-model:hasModel"> 
           <!-- to generalize this we could query for the contentmodel objects title instead of hard
           coded values.  For the vdp project types are limited to annos and images so this will do for now-->
           <field>
                <xsl:attribute name="name">rdf_type_s</xsl:attribute>
                <xsl:choose>
                <xsl:when test="@rdf:resource='info:fedora/islandora:OACCModel'">Annotation</xsl:when>
                <xsl:when test="@rdf:resource='info:fedora/islandora:sp_basic_image'">Image</xsl:when>
                <xsl:otherwise>Other</xsl:otherwise>
                </xsl:choose>
            </field>           
        </xsl:for-each>
    <xsl:variable name = "HASBODY">
        <xsl:value-of select = "foxml:datastream[@ID='OAC']/foxml:datastreamVersion[last()]/foxml:xmlContent//rdf:Description/oa:hasBody[1]/@rdf:resource"/>
    </xsl:variable>
         
    <xsl:for-each
        select="foxml:datastream[@ID='OAC']/foxml:datastreamVersion[last()]/foxml:xmlContent//rdf:Description//dc:title">
        <field>
            <xsl:attribute name="name">anno_title_s</xsl:attribute>
            <xsl:value-of select="."/>
        </field>
    </xsl:for-each>
    
    <xsl:for-each
        select="foxml:datastream[@ID='OAC']/foxml:datastreamVersion[last()]/foxml:xmlContent//rdf:Description//dc:type">
        <field>
            <xsl:attribute name="name">anno_category_s</xsl:attribute>
            <xsl:value-of select="."/>
        </field>
    </xsl:for-each>
    <xsl:for-each
        select='foxml:datastream[@ID="OAC"]/foxml:datastreamVersion[last()]/foxml:xmlContent//rdf:Description[@rdf:about=$HASBODY]/cnt:chars'>
        <field>
            <xsl:attribute name="name">anno_body_t</xsl:attribute>
            <xsl:value-of select="."/>
        </field>
    </xsl:for-each>
</xsl:template>
</xsl:stylesheet>
