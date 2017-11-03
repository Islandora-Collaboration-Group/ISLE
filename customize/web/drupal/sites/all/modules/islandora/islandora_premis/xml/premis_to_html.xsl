<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:premis="info:lc/xmlns/premis-v2" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" version="1.0" exclude-result-prefixes="xsi premis">
  <xsl:output method="xml" omit-xml-declaration="yes" encoding="UTF-8" indent="yes"/>
  <xsl:strip-space elements="*"/>
  <xsl:template match="/premis:premis">
    <!-- PREMIS OBJECT CONTAINER -->
    <fieldset class="form-wrapper">
      <legend>
        <span class="fieldset-legend">PREMIS Object</span>
      </legend>
      <div class="fieldset-wrapper">
        <table class="sticky-header" style="position: fixed; top: 65px; left: 152.5px; visibility: hidden; width: 600px;">
          <thead style="display: block;">
            <tr>
              <th style="width: 279px; display: table-cell;">Field</th>
              <th style="width: 279px; display: table-cell;">Value</th>
            </tr>
          </thead>
        </table>
        <table class="islandora_premis_table sticky-enabled tableheader-processed sticky-table">
          <thead>
            <tr>
              <th>Field</th>
              <th>Value</th>
            </tr>
          </thead>
          <tbody>
            <xsl:apply-templates select="premis:object[@xsi:type='file']"/>
          </tbody>
        </table>
      </div>
    </fieldset>
    <!-- PREMIS EVENT CONTAINER -->
    <fieldset class="form-wrapper">
      <legend>
        <span class="fieldset-legend">PREMIS Events</span>
      </legend>
      <div class="fieldset-wrapper">
        <table class="sticky-header" style="position: fixed; top: 65px; left: 152.5px; visibility: hidden; width: 600px;">
          <thead style="display: block;">
            <tr>
              <th style="width: 279px; display: table-cell;">Field</th>
              <th style="width: 279px; display: table-cell;">Value</th>
            </tr>
          </thead>
        </table>
        <table class="islandora_premis_table sticky-enabled tableheader-processed sticky-table">
          <thead>
            <tr>
              <th>Field</th>
              <th>Value</th>
            </tr>
          </thead>
          <tbody>
            <xsl:apply-templates select="premis:event"/>
          </tbody>
        </table>
      </div>
    </fieldset>
    <!-- PREMIS AGENT CONTAINER -->
    <fieldset class="form-wrapper">
      <legend>
        <span class="fieldset-legend">PREMIS Agent</span>
      </legend>
      <div class="fieldset-wrapper">
        <table class="sticky-header" style="position: fixed; top: 65px; left: 152.5px; visibility: hidden; width: 600px;">
          <thead style="display: block;">
            <tr>
              <th style="width: 279px; display: table-cell;">Field</th>
              <th style="width: 279px; display: table-cell;">Value</th>
            </tr>
          </thead>
        </table>
        <table class="islandora_premis_table sticky-enabled tableheader-processed sticky-table">
          <thead>
            <tr>
              <th>Field</th>
              <th>Value</th>
            </tr>
          </thead>
          <tbody>
            <xsl:apply-templates select="premis:agent"/>
          </tbody>
        </table>
      </div>
    </fieldset>
    <!-- PREMIS RIGHTS CONTAINER -->
    <fieldset class="form-wrapper">
      <legend>
        <span class="fieldset-legend">PREMIS Rights</span>
      </legend>
      <div class="fieldset-wrapper">
        <table class="sticky-header" style="position: fixed; top: 65px; left: 152.5px; visibility: hidden; width: 600px;">
          <thead style="display: block;">
            <tr>
              <th style="width: 279px; display: table-cell;">Field</th>
              <th style="width: 279px; display: table-cell;">Value</th>
            </tr>
          </thead>
        </table>
        <table class="islandora_premis_table sticky-enabled tableheader-processed sticky-table">
          <thead>
            <tr>
              <th>Field</th>
              <th>Value</th>
            </tr>
          </thead>
          <tbody>
            <xsl:apply-templates select="premis:rights"/>
          </tbody>
        </table>
      </div>
    </fieldset>
  </xsl:template>
  <xsl:template match="premis:object[@xsi:type='file']">
    <tr class="odd">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:objectIdentifier/premis:objectIdentifierType)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:objectIdentifier/premis:objectIdentifierType"/>
      </td>
    </tr>
    <tr class="even">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:objectIdentifier/premis:objectIdentifierValue)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:objectIdentifier/premis:objectIdentifierValue"/>
      </td>
    </tr>
    <tr class="odd">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:objectCharacteristics/premis:compositionLevel)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:objectCharacteristics/premis:compositionLevel"/>
      </td>
    </tr>
    <tr class="even">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:objectCharacteristics/premis:creatingApplication/premis:creatingApplicationName)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:objectCharacteristics/premis:creatingApplication/premis:creatingApplicationName"/>
      </td>
    </tr>
    <tr class="odd">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:objectCharacteristics/premis:creatingApplication/premis:dateCreatedByApplication)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:objectCharacteristics/premis:creatingApplication/premis:dateCreatedByApplication"/>
      </td>
    </tr>
    <tr class="even">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:objectCharacteristics/premis:fixity/premis:messageDigestAlgorithm)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:objectCharacteristics/premis:fixity/premis:messageDigestAlgorithm"/>
      </td>
    </tr>
    <tr class="odd">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:objectCharacteristics/premis:fixity/premis:messageDigest)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:objectCharacteristics/premis:fixity/premis:messageDigest"/>
      </td>
    </tr>
    <tr class="even">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:objectCharacteristics/premis:size)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:objectCharacteristics/premis:size"/>
      </td>
    </tr>
    <tr class="odd">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:objectCharacteristics/premis:format/premis:formatDesignation/premis:formatName)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:objectCharacteristics/premis:format/premis:formatDesignation/premis:formatName"/>
      </td>
    </tr>
    <tr class="even">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:storage/premis:contentLocation/premis:contentLocationType)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:storage/premis:contentLocation/premis:contentLocationType"/>
      </td>
    </tr>
    <tr>
      <td class="islandora_premis_table_labels">
        <xsl:text disable-output-escaping="yes">&amp;nbsp;</xsl:text>
      </td>
      <td class="islandora_premis_table_values">
      </td>
    </tr>
    <tr class="even">
      <td class="islandora_premis_table_labels"/>
      <td class="islandora_premis_table_values"/>
    </tr>
  </xsl:template>
  <xsl:template match="premis:event">
    <tr class="odd">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:eventIdentifier/premis:eventIdentifierType)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:eventIdentifier/premis:eventIdentifierType"/>
      </td>
    </tr>
    <tr class="even">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:eventIdentifier/premis:eventIdentifierValue)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:eventIdentifier/premis:eventIdentifierValue"/>
      </td>
    </tr>
    <tr class="odd">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:eventType)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:eventType"/>
      </td>
    </tr>
    <tr class="even">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:eventDateTime)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:eventDateTime"/>
      </td>
    </tr>
    <tr class="odd">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:eventOutcomeInformation/premis:eventOutcome)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:eventOutcomeInformation/premis:eventOutcome"/>
      </td>
    </tr>
    <tr>
      <td class="islandora_premis_table_labels">
        <xsl:text disable-output-escaping="yes">&amp;nbsp;</xsl:text>
      </td>
      <td class="islandora_premis_table_values">
                      </td>
    </tr>
    <tr class="even">
      <td class="islandora_premis_table_labels"/>
      <td class="islandora_premis_table_values"/>
    </tr>
  </xsl:template>
  <xsl:template match="premis:agent">
    <tr class="odd">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:agentIdentifier/premis:agentIdentifierType)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:agentIdentifier/premis:agentIdentifierType"/>
      </td>
    </tr>
    <tr class="even">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:agentIdentifier/premis:agentIdentifierValue)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:agentIdentifier/premis:agentIdentifierValue"/>
      </td>
    </tr>
    <tr class="odd">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:agentName)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:agentName"/>
      </td>
    </tr>
    <tr class="even">
      <td class="islandora_premis_table_labels">
        <xsl:value-of select="name(premis:agentType)"/>
      </td>
      <td class="islandora_premis_table_values">
        <xsl:value-of select="premis:agentType"/>
      </td>
    </tr>
    <tr>
      <td class="islandora_premis_table_labels">
        <xsl:text disable-output-escaping="yes">&amp;nbsp;</xsl:text>
      </td>
      <td class="islandora_premis_table_values">
                      </td>
    </tr>
  </xsl:template>
  
  <xsl:template match="premis:rights">
    <tr class="odd">
      <td class="islandora_premis_table_labels"><xsl:value-of select="name(premis:rightsExtension)"/></td>
      <td class="islandora_premis_table_values"><xsl:value-of select="premis:rightsExtension"/></td>
    </tr>
      </xsl:template>
</xsl:stylesheet>
