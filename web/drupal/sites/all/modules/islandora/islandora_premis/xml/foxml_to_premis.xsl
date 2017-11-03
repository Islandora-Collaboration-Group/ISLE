<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xmlns:foxml="info:fedora/fedora-system:def/foxml#"
  xmlns:audit="info:fedora/fedora-system:def/audit#"
  xmlns:fedora="info:fedora/fedora-system:def/relations-external#"
  xmlns:fedora-model="info:fedora/fedora-system:def/model#"
  xmlns:islandora="http://islandora.ca/ontology/relsext#"
  xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
  xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
  xmlns:fits="http://hul.harvard.edu/ois/xml/ns/fits/fits_output"
  xmlns:php="http://php.net/xsl"
    exclude-result-prefixes="fedora fedora-model foxml audit islandora rdf php">
  <xsl:output method="xml" encoding="utf-8" indent="yes"/>
  <!-- Define a global parameter containing the Islandora object's PID. -->
  <xsl:param name="pid" select="foxml:digitalObject/@PID"/>
  <!-- Global parameters exported from the XSLT processor. -->
  <xsl:param name="premis_agent_name_organization"/>
  <xsl:param name="premis_agent_identifier_organization"/>
  <xsl:param name="premis_agent_identifier_type"/>
  <xsl:param name="premis_agent_type_organization"/>
  <xsl:param name="premis_object_creating_application_name"/>
  <!-- Note: the version number is current at time of deriving PREMIS, not at time of creation of audit log entry. -->
  <xsl:param name="fedora_commons_version"/>
  <xsl:preserve-space elements="*"/>
  <xsl:template match="foxml:digitalObject">
    <premis xmlns="info:lc/xmlns/premis-v2" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/" xsi:schemaLocation="info:lc/xmlns/premis-v2 http://www.loc.gov/standards/premis/v2/premis.xsd" version="2.2">
      <xsl:comment>PREMIS data for Islandora object <xsl:value-of select="$pid"/>. Contains PREMIS object entries for each
        datastream in an Islandora object, and event entries documenting all fixity checks
        performed on versions of those datastreams by the Islandora Checksum Checker module.
        Note that a datastream version that has never had a fixity check performed on it will
        not be linked to any fixity check events.</xsl:comment>
      <xsl:comment>Some things to note:</xsl:comment>
      <xsl:comment>'Internal' eventIdentifierType values in this PREMIS document are comprised of Fedora
        datasteam ID plus ':' plus Fedora Audit Record ID.</xsl:comment>
      <xsl:comment>This PREMIS document does not contain any linkingEventIdentifier elements. eventIdentifierValue
        values can be linked to objects using the naming convention described in the previous comment.</xsl:comment>
      <xsl:comment>Datastreams in the Inline XML control group (X) (e.g., DC and RELS-EXT) do not have a contentLocation
        element in the FOXML, so they do not have a corresponding contentLocationValue element in PREMIS.</xsl:comment>
      <xsl:comment>creatingApplicationName will be 'Islandora'. See https://jira.duraspace.org/browse/ISLANDORA-2016 for
        more info. dateCreatedByApplication uses the Fedora datastream version CREATED value. These properties
        are applied to all datastreams, even OBJ, and all datastream versions.</xsl:comment>
      <xsl:comment>The eventOutcome element is "coded" (as recommended in the PREMIS Data Dictionary) by the
        Islandora Checksum Checker module in this PREMIS document as follows: "[checksum type] checksum validated.",
        "Invalid [checksum type] detected.", or "Checksums not enabled."</xsl:comment>
      <!-- Objects first. -->
      <xsl:for-each select="foxml:datastream">
        <xsl:variable name="datastream_id" select="@ID"/>
        <xsl:for-each select="foxml:datastreamVersion">
          <object xsi:type="file">
            <objectIdentifier>
              <objectIdentifierType>Fedora Commons datastreamVersion ID</objectIdentifierType>
              <objectIdentifierValue>
                <xsl:value-of select="@ID"/>
              </objectIdentifierValue>
            </objectIdentifier>
            <objectCharacteristics>
              <compositionLevel>0</compositionLevel>
              <creatingApplication>
                <creatingApplicationName>
                  <xsl:value-of select="$premis_object_creating_application_name"/>
                </creatingApplicationName>
                <dateCreatedByApplication> 
                  <xsl:value-of select="@CREATED"/>
                </dateCreatedByApplication> 
              </creatingApplication>
              <fixity>
                <messageDigestAlgorithm>
                  <xsl:value-of select="foxml:contentDigest/@TYPE"/>
                </messageDigestAlgorithm>
                <messageDigest>
                  <xsl:value-of select="foxml:contentDigest/@DIGEST"/>
                </messageDigest>
              </fixity>
              <xsl:if test="string-length(@SIZE)">
                <size>
                  <xsl:value-of select="@SIZE"/>
                </size>
              </xsl:if>
              <format>
                <formatDesignation>
                  <formatName>
                    <xsl:value-of select="@MIMETYPE"/>
                  </formatName>
                </formatDesignation>
              </format>
              <!-- Call back to PHP for the objectCharacteristicsExtension content (may be none). -->
              <xsl:variable name="object-characteristics-extension" select="php:function('islandora_premis_get_object_characteristics_extension', string($pid), string(../@ID))"/>
              <xsl:if test="$object-characteristics-extension">
                <objectCharacteristicsExtension>
                  <xsl:copy-of select="$object-characteristics-extension/fits:fits"/>
                </objectCharacteristicsExtension>
              </xsl:if>
            </objectCharacteristics>
            <storage>
              <contentLocation>
                <contentLocationType>Fedora Commons contentLocation REF value</contentLocationType>
                <contentLocationValue>
                  <xsl:value-of select="foxml:contentLocation/@REF"/>
                </contentLocationValue>
              </contentLocation>
            </storage>
          </object>
        </xsl:for-each>
      </xsl:for-each>
      <!-- Then their events. -->
      <xsl:for-each select="foxml:datastream">
        <xsl:variable name="datastream_id" select="@ID"/>
        <xsl:for-each select="foxml:datastreamVersion">
          <xsl:variable name="datastream_version_id" select="@ID"/>
          <xsl:variable name="event_content_location" select="concat($pid, '+', $datastream_id, '+', $datastream_version_id)"/>
          <!-- There should only be one audit:auditTrail but this for-each loop accounts for multiple. -->
          <xsl:for-each select="/foxml:digitalObject/foxml:datastream[@ID='AUDIT']/foxml:datastreamVersion/foxml:xmlContent/audit:auditTrail">
            <xsl:for-each select="audit:record">
              <!-- We're only interested in audit:records that document a PREMIS fixityEvent. -->
              <xsl:variable name="justification" select="audit:justification"/>
              <xsl:if test="contains($justification, concat('PREMIS:file=', $event_content_location))">
                <xsl:variable name="responsibility" select="audit:responsibility"/>
                <xsl:variable name="date" select="audit:date"/>
                <event>
                  <eventIdentifier>
                    <eventIdentifierType>Internal</eventIdentifierType>
                    <eventIdentifierValue>
                      <xsl:value-of select="concat($datastream_id, ':', @ID)"/>
                    </eventIdentifierValue>
                  </eventIdentifier>
                  <eventType>fixity check</eventType>
                  <eventDateTime>
                    <xsl:value-of select="$date"/>
                  </eventDateTime>
                  <eventOutcomeInformation>
                    <eventOutcome>
                      <xsl:value-of select="substring-after(audit:justification, 'PREMIS:eventOutcome=')"/>
                    </eventOutcome>
                  </eventOutcomeInformation>
                  <linkingAgentIdentifier>
                    <linkingAgentIdentifierType>
                      <xsl:value-of select="$premis_agent_identifier_type"/>
                    </linkingAgentIdentifierType>
                    <linkingAgentIdentifierValue>
                      <xsl:value-of select="$premis_agent_identifier_organization"/>
                    </linkingAgentIdentifierValue>
                    <linkingAgentRole>Implementer</linkingAgentRole>
                  </linkingAgentIdentifier>
                  <linkingAgentIdentifier>
                    <linkingAgentIdentifierType>URI</linkingAgentIdentifierType>
                    <linkingAgentIdentifierValue>http://www.fedora-commons.org/</linkingAgentIdentifierValue>
                    <linkingAgentRole>Validator</linkingAgentRole>
                  </linkingAgentIdentifier>
                </event>
              </xsl:if>
            </xsl:for-each>
          </xsl:for-each>
        </xsl:for-each>
      </xsl:for-each>
      <!-- Then agents. -->
      <agent>
        <agentIdentifier>
          <agentIdentifierType>
            <xsl:value-of select="$premis_agent_identifier_type"/>
          </agentIdentifierType>
          <agentIdentifierValue>
            <xsl:value-of select="$premis_agent_identifier_organization"/>
          </agentIdentifierValue>
        </agentIdentifier>
        <agentName>
          <xsl:value-of select="$premis_agent_name_organization"/>
        </agentName>
        <agentType>
          <xsl:value-of select="$premis_agent_type_organization"/>
        </agentType>
      </agent>
      <agent>
        <agentIdentifier>
          <agentIdentifierType>URI</agentIdentifierType>
          <agentIdentifierValue>http://www.fedora-commons.org/</agentIdentifierValue>
        </agentIdentifier>
        <agentName>Fedora Repository <xsl:value-of select="$fedora_commons_version"/></agentName>
        <agentType>software</agentType>
      </agent>
      <!-- rights metadata -->
      <rights>
        <rightsExtension>
          <dc:rights>
            <xsl:value-of select="/foxml:digitalObject/foxml:datastream/foxml:datastreamVersion[last()]/foxml:xmlContent/oai_dc:dc/dc:rights"/>
          </dc:rights>
        </rightsExtension>
      </rights>
    </premis>
  </xsl:template>
</xsl:stylesheet>
