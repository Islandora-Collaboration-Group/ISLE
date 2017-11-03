<?xml version="1.0" encoding="UTF-8"?> 
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:mods="http://www.loc.gov/mods/v3" exclude-result-prefixes="mods pubmed"
	xmlns:pubmed="http://www.ncbi.nlm.nih.gov"
	xmlns:date="http://exslt.org/dates-and-times" extension-element-prefixes="date">
	<xsl:import href="exslt/date/functions/date/date.date.template.xsl"/>
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<xsl:strip-space elements="*"/>
	<xsl:variable name="upper" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>
	<xsl:variable name="lower" select="'abcdefghijklmnopqrstuvwxyz'"/>
	<xsl:template match="*[namespace-uri()='' and local-name() != 'PubmedArticleSet']">
		<xsl:element name="{name()}" namespace="http://www.ncbi.nlm.nih.gov">
			<xsl:copy-of select="@*"/>
			<xsl:apply-templates/>
		</xsl:element>
	</xsl:template>
	<xsl:template name="PubMedDate">
		<xsl:if test="Year">
			<Year xmlns="http://www.loc.gov/mods/v3">
				<xsl:value-of select="Year"/>
			</Year>
		</xsl:if>
		<xsl:if test="Month">
			<Month xmlns="http://www.loc.gov/mods/v3">
				<xsl:value-of select="Month"/>
			</Month>
		</xsl:if>
		<xsl:if test="Day">
			<Day xmlns="http://www.loc.gov/mods/v3">
				<xsl:value-of select="Day"/>
			</Day>
		</xsl:if>
		<xsl:if test="MedlineDate">
			<MedlineDate xmlns="http://www.loc.gov/mods/v3">
				<xsl:value-of select="MedlineDate"/>
			</MedlineDate>
		</xsl:if>
	</xsl:template>
	<xsl:template match="//PubmedArticle">
		<mods xmlns="http://www.loc.gov/mods/v3" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:pubmed="http://www.ncbi.nlm.nih.gov">
			<!-- Variables -->
			<xsl:variable name="Article" select="MedlineCitation/Article"/>
			<xsl:variable name="Journal" select="$Article/Journal"/>
			<xsl:variable name="JournalIssue" select="$Journal/JournalIssue"/>
			<xsl:variable name="PubDate" select="$JournalIssue/PubDate"/>
			<xsl:variable name="Abstract" select="$Article/Abstract"/>
			<xsl:variable name="Grant" select="$Article/GrantList/Grant"/>
			<xsl:variable name="Author" select="$Article/AuthorList/Author"/>
			<xsl:variable name="Investigator" select="$Article/InvestigatorList/Investigator"/>
			<!-- Title -->
			<titleInfo>
				<title>
					<xsl:value-of select="$Article/ArticleTitle"/>
				</title>
			</titleInfo>
			<!-- Abstract -->
			<xsl:for-each select="$Abstract/AbstractText">
				<abstract>
					<xsl:attribute name="displayLabel">
						<xsl:value-of select="normalize-space(@Label)"/>
					</xsl:attribute>
					<xsl:value-of select="normalize-space(text())"/>
					
				</abstract>
			</xsl:for-each>
			
			<!-- Copyright Information -->
			<xsl:if test="$Abstract/CopyrightInformation">
				<accessCondition type="useAndReproduction">
					<xsl:value-of select="$Abstract/CopyrightInformation"/>
				</accessCondition>
			</xsl:if>
			<!-- Author -->
			<xsl:for-each select="$Author[not(CollectiveName)]">
				<name type="personal">
					<xsl:if test="LastName">
						<namePart type="family">
							<xsl:value-of select="LastName"/>
						</namePart>
					</xsl:if>
					<xsl:if test="ForeName">
						<namePart type="given">
							<xsl:value-of select="ForeName"/>
						</namePart>
					</xsl:if>
					<role>
						<roleTerm authority="marcrelator" type="text">author</roleTerm>
					</role>
				</name>
			</xsl:for-each>
			<!-- Group Author -->
			<xsl:for-each select="$Author/CollectiveName">
				<name type="corporate">
					<namePart>
						<xsl:value-of select="self::node()"/>
					</namePart>
					<role>
						<roleTerm authority="marcrelator" type="text">author</roleTerm>
					</role>
				</name>
			</xsl:for-each>
			<!-- Author Affiliation -->
			<xsl:for-each select="$Article/Affiliation">
				<name type="corporate">
					<affiliation>
						<xsl:value-of select="text()"/>
					</affiliation>
					<role>
						<roleTerm authority="marcrelator" type="text">author</roleTerm>
					</role>
				</name>
			</xsl:for-each>
			<originInfo>
				<!-- Electronic Publication Date @todo Could not find a reference to this -->
				<xsl:for-each select="$Article/ArticleDate[@DateType='Electronic' and last()]">
					<dateCaptured>
						<xsl:call-template name="PubMedDate"/>
					</dateCaptured>
				</xsl:for-each>
				<!-- Publication Date-Year/Date-Month/Date-Day -->
				<xsl:for-each select="$JournalIssue/PubDate[last()]">
					<dateIssued>
						<xsl:call-template name="PubMedDate"/>
					</dateIssued>
					
				</xsl:for-each>
				<!-- Season -->
				<dateOther type="season">
					<xsl:choose>
						<xsl:when test="$JournalIssue/PubDate[last()]/Season = 'Autumn'">
							<xsl:text>Fall</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$JournalIssue/PubDate[last()]/Season"/>
						</xsl:otherwise>
					</xsl:choose>
				</dateOther>
				<!-- Publisher -->
				<xsl:if test="$Journal/PublisherName">
					<publisher><xsl:value-of select="$Journal/PublisherName"/></publisher>
				</xsl:if>
			</originInfo>
			<!-- Grant Number/Funding Agency -->
			<xsl:for-each select="$Grant">
				<name type="corporate">
					<namePart>
						<xsl:value-of select="GrantID"/>
					</namePart>
					<affiliation>
						<xsl:value-of select="Agency"/>
					</affiliation>
					<role>
						<roleTerm authority="marcrelator" type="text">funder</roleTerm>
					</role>
				</name>
			</xsl:for-each>
			<!-- Investigator Name -->
			<xsl:for-each select="$Investigator">
				<name type="personal">
					<xsl:if test="LastName">
						<namePart type="family">
							<xsl:value-of select="LastName"/>
						</namePart>
					</xsl:if>
					<xsl:if test="ForeName">
						<namePart type="given">
							<xsl:value-of select="ForeName"/>
						</namePart>
					</xsl:if>
					<!-- Investigator Affiliation -->
					<xsl:if test="Affiliation">
						<affiliation>
							<xsl:value-of select="Affiliation"/>
						</affiliation>
					</xsl:if>
					<role>
						<roleTerm>investigator</roleTerm>
					</role>
				</name>
			</xsl:for-each>
			<!-- ISSN -->
			<xsl:if test="$Journal/ISSN[normalize-space(text())]">
				<identifier type="issn">
					<xsl:value-of select="$Journal/ISSN"/>
				</identifier>
			</xsl:if>
			<part>
				<!-- Issue -->
				<xsl:if test="$JournalIssue/Issue">
					<detail type="issue">
						<number>
							<xsl:value-of select="$JournalIssue/Issue"/>
						</number>
					</detail>
				</xsl:if>
				<!-- Volume -->
				<xsl:if test="$JournalIssue/Volume[normalize-space(text())]">
					<detail type="volume">
						<number>
							<xsl:value-of select="$JournalIssue/Volume"/>
						</number>
					</detail>
				</xsl:if>
				<!-- Pages -->
				<xsl:if test="$Article/Pagination/MedlinePgn[normalize-space(text())]">
					<extent unit="page">
						<list>
							<xsl:value-of select="$Article/Pagination/MedlinePgn"/>
						</list>
					</extent>
				</xsl:if>
			</part>
			<!-- Language -->
			<xsl:if test="$Article/Language[normalize-space(text())]">
				<language>
					<languageTerm>
						<xsl:value-of select="$Article/Language"/>
					</languageTerm>
				</language>
			</xsl:if>
			<!-- Manuscript Identifier -->
			<xsl:if test="PubmedData/ArticleIdList/ArticleId[@IdType='mid' and normalize-space(text())]">
				<identifier type="mid">
					<xsl:value-of select="PubmedData/ArticleIdList/ArticleId[@IdType='mid']"/>
				</identifier>
			</xsl:if>
			<!-- DOI -->
			<xsl:if test="PubmedData/ArticleIdList/ArticleId[@IdType='doi' and normalize-space(text())]">
				<identifier type="doi">
					<xsl:value-of select="PubmedData/ArticleIdList/ArticleId[@IdType='doi']"/>
				</identifier>
			</xsl:if>
			<!-- PMCID -->
			<xsl:if test="PubmedData/ArticleIdList/ArticleId[@IdType='pmc' and normalize-space(text())]">
				<identifier type="pmc">
					<xsl:value-of select="PubmedData/ArticleIdList/ArticleId[@IdType='pmc']"/>
				</identifier>
			</xsl:if>
			<!-- Accession -->
			<xsl:if test="PubmedData/ArticleIdList/ArticleId[@IdType='pubmed' and normalize-space(text())]">
				<identifier type="accession">
					<xsl:value-of select="PubmedData/ArticleIdList/ArticleId[@IdType='pubmed']"/>
				</identifier>
			</xsl:if>
			<xsl:if test="$Article/Journal/Title[normalize-space(text())] or MedlineCitation/MedlineJournalInfo/MedlineTA">
				<relatedItem type="host">
					<xsl:if test="$Article/Journal/Title[normalize-space(text())]">
						<titleInfo>
							<!-- Full Journal Title -->
							<title>
								<xsl:value-of select="$Article/Journal/Title"/>
							</title>
						</titleInfo>
					</xsl:if>
					<xsl:if test="MedlineCitation/MedlineJournalInfo/MedlineTA">
						<titleInfo type="abbreviated">
							<!-- Journal Title Abbreviation -->
							<title>
								<xsl:value-of select="MedlineCitation/MedlineJournalInfo/MedlineTA"/>
							</title>
						</titleInfo>
					</xsl:if>
				</relatedItem>
			</xsl:if>
			<!-- MeSH -->
			<xsl:for-each select="MedlineCitation/MeshHeadingList/MeshHeading[normalize-space(*/text())]">
				<subject authority="mesh">
					<titleInfo>
						<title>
							<xsl:value-of select="normalize-space(DescriptorName)"/>
						</title>
					</titleInfo>
					<xsl:for-each select="QualifierName[normalize-space(text())]">
						<topic>
							<xsl:value-of select="normalize-space(text())"/>
						</topic>	
					</xsl:for-each>		
				</subject>
			</xsl:for-each>
			<xsl:for-each select="MedlineCitation/ChemicalList/Chemical">
				<relatedItem type="references">
					<!-- CAS Registry Number -->
					<identifier type="cas">
						<xsl:value-of select="RegistryNumber"/>
					</identifier>
					<titleInfo>
						<!-- Chemical -->
						<title>
							<xsl:value-of select="NameOfSubstance"/>
						</title>
					</titleInfo>
				</relatedItem>
			</xsl:for-each>
			<!-- Publication Type / Type of Work -->
			<genre>journal article</genre>
			<xsl:for-each select="$Article/PublicationTypeList/PublicationType[normalize-space(text())]">
				<note type="type of work">
					<xsl:value-of select="translate(text(), $upper, $lower)"/>
				</note>
			</xsl:for-each>
			<!-- Comments/Corrections -->
			<xsl:for-each select="MedlineCitation/CommentsCorrectionsList">
				<extension displayLabel="CommentsAndCorrections">
					<xsl:element name="CommentsCorrectionsList" namespace="http://www.ncbi.nlm.nih.gov">
						<xsl:copy-of select="@*"/>
						<xsl:apply-templates/>
					</xsl:element>
				</extension>
			</xsl:for-each>
			<recordInfo>
				<recordOrigin>PubMed</recordOrigin>
				<recordCreationDate/>
			</recordInfo>
			<!-- Clinical Trial Number -->
			<xsl:for-each select="$Article/DataBankList[@CompleteYN='Y']/DataBank[DataBankName='ClinicalTrials.gov']/AccessionNumberList/AccessionNumber">
				<identifier type="clinical trials">
					<xsl:value-of select="text()"/>
				</identifier>
			</xsl:for-each>
			<!-- Date - Store original values for dates -->
			<xsl:for-each select="$JournalIssue/PubDate">
				<note type='date issued'>
					<xsl:for-each select="*">
						<xsl:value-of select="text()"/>
						<xsl:text> </xsl:text>
					</xsl:for-each>
				</note>
			</xsl:for-each>
			<xsl:for-each select="$Article/ArticleDate[@DateType='Electronic']">
				<note type='date captured'>
					<xsl:for-each select="*">
						<xsl:value-of select="text()"/>
						<xsl:text> </xsl:text>
					</xsl:for-each>
				</note>
			</xsl:for-each>
			<!-- Always empty -->
			<note type='record creation date'/>
			<!-- Status -->
			<note type='status'>imported</note>
		</mods>
	</xsl:template>
</xsl:stylesheet>
