<?xml version="1.0"?>
<xsl:stylesheet version="1.0" 
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:str="http://exslt.org/strings"
                extension-element-prefixes="str">

<xsl:template match="a">
   <xsl:apply-templates/>
</xsl:template>
<xsl:template match="*">
   <xsl:value-of select="." /> -
   <xsl:value-of select="str:tokenize(string(.), ' ')" />
   <xsl:value-of select="str:tokenize(string(.), '')" />
   <xsl:for-each select="str:tokenize(string(.), ' ')" >
      <xsl:value-of select="." />
   </xsl:for-each>
   <xsl:apply-templates select = "*" />
</xsl:template>

</xsl:stylesheet>