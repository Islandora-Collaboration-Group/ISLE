<?xml version="1.0" encoding="UTF-8"?>
<!-- We need all lower level namespaces to be declared here for exclude-result-prefixes attributes
     to be effective -->
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:zs="http://www.loc.gov/zing/srw/"
  xmlns:foxml="info:fedora/fedora-system:def/foxml#"
  xmlns:rel="info:fedora/fedora-system:def/relations-external#"
  xmlns:fedora-model="info:fedora/fedora-system:def/model#"
  xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
  xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
  xmlns:fedora="info:fedora/fedora-system:def/relations-external#"
  xmlns:dwc="http://rs.tdwg.org/dwc/xsd/simpledarwincore/"
  xmlns:uvalibdesc="http://dl.lib.virginia.edu/bin/dtd/descmeta/descmeta.dtd"
  xmlns:uvalibadmin="http://dl.lib.virginia.edu/bin/admin/admin.dtd/"
  xmlns:res="http://www.w3.org/2001/sw/DataAccess/rf1/result"
  xmlns:eaccpf="urn:isbn:1-931666-33-4"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:tei="http://www.tei-c.org/ns/1.0"
  xmlns:mods="http://www.loc.gov/mods/v3"
  xmlns:exts="xalan://dk.defxws.fedoragsearch.server.GenericOperationsImpl"
  xmlns:islandora-exts="xalan://ca.upei.roblib.DataStreamForXSLT"
            exclude-result-prefixes="exts"
  xmlns:encoder="xalan://java.net.URLEncoder"
  xmlns:java="http://xml.apache.org/xalan/java"
  xmlns:sparql="http://www.w3.org/2001/sw/DataAccess/rf1/result"
  xmlns:xalan="http://xml.apache.org/xalan">

  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <!-- gsearch magik @TODO: see if any of the explicit variables can be replaced by these -->
  <xsl:param name="REPOSITORYNAME" select="repositoryName"/>
  <xsl:param name="FEDORASOAP" select="repositoryName"/>
  <xsl:param name="FEDORAUSER" select="repositoryName"/>
  <xsl:param name="FEDORAPASS" select="repositoryName"/>
  <xsl:param name="TRUSTSTOREPATH" select="repositoryName"/>
  <xsl:param name="TRUSTSTOREPASS" select="repositoryName"/>

  <!-- These values are accessible in included xslts -->
  <xsl:variable name="PROT">http</xsl:variable>
  <xsl:variable name="HOST">localhost</xsl:variable>
  <xsl:variable name="PORT">8080</xsl:variable>
  <xsl:variable name="PID" select="/foxml:digitalObject/@PID"/>
  <xsl:variable name="FEDORA" xmlns:java_string="xalan://java.lang.String" select="substring($FEDORASOAP, 1, java_string:lastIndexOf(java_string:new(string($FEDORASOAP)), '/'))"/>


  <!--
  This xslt stylesheet generates the IndexDocument consisting of IndexFields
    from a FOXML record. The IndexFields are:
      - from the root element = PID
      - from foxml:property   = type, state, contentModel, ...
      - from oai_dc:dc        = title, creator, ...
    The IndexDocument element gets a PID attribute, which is mandatory,
    while the PID IndexField is optional.
  -->

  <!-- These includes are for transformations on individual datastreams;
     disable the ones you do not want to perform;
     the paths may need to be updated if the standard install was not followed
     TODO: look into a way to make these paths relative -->

     <!-- older gsearches (slurp_all_MODS_to_solr also contains an include that would need to be
     altered if you use these)-->

  <!-- The name of the 'config' directory has changed in newer versions of fedoragsearch.
  If your installation uses 'WEB-INF/classes/config' enable the import block below, and
  disable the import block using 'WEB-INF/classes/fgsconfigFinal'.
  -->

  <!--
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/DC_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/RELS-EXT_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/RELS-INT_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/FOXML_properties_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/datastream_info_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/slurp_all_MODS_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/slurp_all_ead_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/MODS_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/EACCPF_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/TEI_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/text_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/XML_to_one_solr_field.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/XML_text_nodes_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/MADS_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/WORKFLOW_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/slurp_all_chemicalML_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/library/traverse-graph.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/config/index/FgsIndex/islandora_transforms/hierarchy.xslt"/>
  -->

  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/DC_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/QDC_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/RELS-EXT_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/RELS-INT_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/FOXML_properties_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/datastream_info_to_solr.xslt"/>
  <!--<xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/MODS_to_solr.xslt"/>-->
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/slurp_all_MODS_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/slurp_all_ead_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/EACCPF_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/TEI_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/text_to_solr.xslt"/>
  <!--<xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/XML_to_one_solr_field.xslt"/>-->
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/XML_text_nodes_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/MADS_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/WORKFLOW_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/slurp_all_chemicalML_to_solr.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/library/traverse-graph.xslt"/>
  <xsl:include href="/usr/local/tomcat/webapps/fedoragsearch/WEB-INF/classes/fgsconfigFinal/index/FgsIndex/islandora_transforms/hierarchy.xslt"/>

  <!-- Decide which objects to modify the index of -->
  <xsl:template match="/">
    <update>
      <!-- The following allows only active and data oriented FedoraObjects to be indexed -->
      <xsl:if test="not(foxml:digitalObject/foxml:datastream[@ID='METHODMAP' or @ID='DS-COMPOSITE-MODEL'])">
        <xsl:choose>
          <xsl:when test="foxml:digitalObject/foxml:objectProperties/foxml:property[@NAME='info:fedora/fedora-system:def/model#state' and @VALUE='Active']">
            <add>
              <xsl:apply-templates select="/foxml:digitalObject" mode="indexFedoraObject">
                <xsl:with-param name="PID" select="$PID"/>
              </xsl:apply-templates>
            </add>
            <!-- Newspaper graph example.
            <xsl:variable name="graph">
              <xsl:call-template name="_traverse_graph">
                <xsl:with-param name="risearch" select="concat($FEDORA, '/risearch')"/>
                <xsl:with-param name="to_traverse_in">
                  <sparql:result>
                    <sparql:obj>
                      <xsl:attribute name="uri">info:fedora/<xsl:value-of select="$PID"/></xsl:attribute>
                    </sparql:obj>
                  </sparql:result>
                </xsl:with-param>
                <xsl:with-param name="query">
                  PREFIX fre: &lt;info:fedora/fedora-system:def/relations-external#&gt;
                  PREFIX fm: &lt;info:fedora/fedora-system:def/model#&gt;
                  PREFIX islandora: &lt;http://islandora.ca/ontology/relsext#&gt;
                  SELECT ?obj
                  FROM &lt;#ri&gt;
                  WHERE {
                    {
                      &lt;%PID_URI%&gt; fm:hasModel &lt;info:fedora/islandora:newspaperCModel&gt; {
                        ?issue fre:isMemberOf &lt;%PID_URI%&gt; .
                        ?obj islandora:isPageOf ?issue
                      }
                      UNION {
                        ?obj fre:isMemberOf &lt;%PID_URI%&gt;
                      }
                    }
                    UNION {
                      &lt;%PID_URI%&gt; fm:hasModel &lt;info:fedora/islandora:newspaperIssueCModel&gt; .
                      ?obj islandora:isPageOf &lt;%PID_URI%&gt;
                    }
                    ?obj fm:state fm:Active
                  }
                </xsl:with-param>
              </xsl:call-template>
            </xsl:variable>
            <add commitWithin="5000">
              <xsl:for-each select="xalan:nodeset($graph)//sparql:obj[@uri != concat('info:fedora/', $PID)]">
                <xsl:variable name="xml_url" select="concat(substring-before($FEDORA, '://'), '://', encoder:encode($FEDORAUSER), ':', encoder:encode($FEDORAPASS), '@', substring-after($FEDORA, '://') , '/objects/', substring-after(@uri, '/'), '/objectXML')"/>
                <xsl:variable name="object" select="document($xml_url)"/>
                <xsl:if test="$object">
                  <xsl:apply-templates select="$object/foxml:digitalObject" mode="indexFedoraObject">
                    <xsl:with-param name="PID" select="substring-after(@uri, '/')"/>
                  </xsl:apply-templates>
                </xsl:if>
              </xsl:for-each>
            </add>
           -->
          </xsl:when>
          <xsl:otherwise>
            <xsl:apply-templates select="/foxml:digitalObject" mode="unindexFedoraObject"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:if>
    </update>
  </xsl:template>

  <!-- Index an object -->
  <xsl:template match="/foxml:digitalObject" mode="indexFedoraObject">
    <xsl:param name="PID"/>

    <doc>
      <!-- put the object pid into a field -->
      <field name="PID">
        <xsl:value-of select="$PID"/>
      </field>

      <!-- These templates are in the islandora_transforms -->
      <xsl:apply-templates select="foxml:objectProperties/foxml:property"/>
      <xsl:apply-templates select="/foxml:digitalObject" mode="index_object_datastreams"/>

      <!-- THIS IS SPARTA!!!
        These lines call a matching template on every datastream id so that you only have to edit included files
        handles inline and managed datastreams
        The datastream level element is used for matching,
        making it imperative to use the $content parameter for xpaths in templates
        if they are to support managed datstreams -->

      <!-- TODO: would like to get rid of the need for the content param -->
      <xsl:for-each select="foxml:datastream">
        <xsl:choose>
          <xsl:when test="@CONTROL_GROUP='X'">
            <xsl:apply-templates select="foxml:datastreamVersion[last()]">
              <xsl:with-param name="content" select="foxml:datastreamVersion[last()]/foxml:xmlContent"/>
            </xsl:apply-templates>
          </xsl:when>
          <xsl:when test="@CONTROL_GROUP='M' and foxml:datastreamVersion[last()][@MIMETYPE='text/xml' or @MIMETYPE='application/xml' or @MIMETYPE='application/rdf+xml' or @MIMETYPE='text/html' or @MIMETYPE='chemical/x-cml']">
            <!-- TODO: should do something about mime type filtering
              text/plain should use the getDatastreamText extension because document will only work for xml docs
              xml files should use the document function
              other mimetypes should not be being sent
              will this let us not use the content variable? -->
            <xsl:apply-templates select="foxml:datastreamVersion[last()]">
              <xsl:with-param name="content" select="document(concat($PROT, '://', encoder:encode($FEDORAUSER), ':', encoder:encode($FEDORAPASS), '@', $HOST, ':', $PORT, '/fedora/objects/', $PID, '/datastreams/', @ID, '/content'))"/>
            </xsl:apply-templates>
          </xsl:when>
          <!-- non-xml managed datastreams...
               Really, should probably only
               handle the mimetypes supported by the "getDatastreamText" call:
               https://github.com/fcrepo/gsearch/blob/master/FedoraGenericSearch/src/java/dk/defxws/fedoragsearch/server/TransformerToText.java#L185-L200
          -->
          <xsl:when test="@CONTROL_GROUP='M' and foxml:datastreamVersion[last() and not(starts-with(@MIMETYPE, 'image') or starts-with(@MIMETYPE, 'audio') or starts-with(@MIMETYPE, 'video') or @MIMETYPE = 'application/pdf')]">
            <!-- TODO: should do something about mime type filtering
              text/plain should use the getDatastreamText extension because document will only work for xml docs
              xml files should use the document function
              other mimetypes should not be being sent
              will this let us not use the content variable? -->
            <xsl:apply-templates select="foxml:datastreamVersion[last()]">
              <xsl:with-param name="content" select="java:ca.discoverygarden.gsearch_extensions.XMLStringUtils.escapeForXML(normalize-space(exts:getDatastreamText($PID, $REPOSITORYNAME, @ID, $FEDORASOAP, $FEDORAUSER, $FEDORAPASS, $TRUSTSTOREPATH, $TRUSTSTOREPASS)))"/>
            </xsl:apply-templates>
          </xsl:when>
        </xsl:choose>
      </xsl:for-each>

      <!-- this is an example of using template modes to have multiple ways of indexing the same stream -->
      <!--
      <xsl:apply-templates select="foxml:datastream[@ID='EAC-CPF']/foxml:datastreamVersion[last()]/foxml:xmlContent//eaccpf:eac-cpf">
        <xsl:with-param name="pid" select="$PID"/>
      </xsl:apply-templates>
      <xsl:apply-templates mode="fjm" select="foxml:datastream[@ID='EAC-CPF']/foxml:datastreamVersion[last()]/foxml:xmlContent//eaccpf:eac-cpf">
        <xsl:with-param name="pid" select="$PID"/>
        <xsl:with-param name="suffix">_s</xsl:with-param>
      </xsl:apply-templates>
      -->

      <!-- Index ancestors, as used in the islandora_collection_search module.
        Requires the "hierarchy.xslt" to be included (uncomment near the top of
        the file?).
        Also, note: When migrating objects between collections, it would be
        necessary to update all descendents to ensure their list of ancestors
        reflect the current state... We do this in the
        islandora_collection_search module when migrating, instead of
        reindexing all the descendents whenever indexing an object
        (updating a collection label would be fairly expensive if we blindly
        reindexed). -->
      <xsl:variable name="ancestors">
        <xsl:call-template name="get-ancestors">
          <xsl:with-param name="PID" select="$PID" />
        </xsl:call-template>
      </xsl:variable>
      <xsl:for-each select="xalan:nodeset($ancestors)//sparql:obj[@uri != concat('info:fedora/', $PID)]">
        <field name="ancestors_ms"><xsl:value-of select="substring-after(@uri, '/')"/></field>
      </xsl:for-each>

    </doc>
  </xsl:template>

  <!-- Delete the solr doc of an object -->
  <xsl:template match="/foxml:digitalObject" mode="unindexFedoraObject">
    <xsl:comment> name="PID" This is a hack, because the code requires that to be present </xsl:comment>
    <delete>
      <id>
        <xsl:value-of select="$PID"/>
      </id>
    </delete>
  </xsl:template>

  <!-- This prevents text from just being printed to the doc without field elements JUST TRY COMMENTING IT OUT -->
  <xsl:template match="text()"/>
  <xsl:template match="text()" mode="indexFedoraObject"/>
  <xsl:template match="text()" mode="unindexFedoraObject"/>
  <xsl:template match="text()" mode="index_object_datastreams"/>
</xsl:stylesheet>
