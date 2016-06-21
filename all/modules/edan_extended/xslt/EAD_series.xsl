<!-- MAIN SERIES ARRANGEMENT -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:ead="urn:isbn:1-931666-22-9"
  xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns="http://www.w3.org/1999/xhtml" xmlns:fn="http://www.w3.org/2005/02/xpath-functions"
  xmlns:xdt="http://www.w3.org/2005/02/xpath-datatypes" xmlns:ns2="http://www.w3.org/1999/xlink">
      
  <!-- <xsl:output method="xml" indent="yes" encoding="UTF-8"/> -->
  <xsl:output
    method="xml"
    encoding="UTF-8"
    omit-xml-declaration="yes"
    indent="yes"
    media-type="text/xml"/>

  <xsl:strip-space elements="*"/>

  <!-- This template rule formats the main arrangement element -->
  <xsl:template match="/*">
    <html>
      <xsl:call-template name="head"/>
      <ul><xsl:call-template name="dsc"/></ul>
    </html>
  </xsl:template>

  <xsl:template name="head">
    <head>
      <meta http-equiv="content-type" content="text/html; charset=UTF-8" />
    </head>
  </xsl:template>

  <!-- This named template is a wrapper for the entire ead:archdesc/ead:dsc block -->
  <xsl:template name="dsc">
    <xsl:for-each select="ead:archdesc/ead:dsc">
      <xsl:apply-templates select="ead:c">
        <xsl:with-param name="indentLevel" select="0" />
      </xsl:apply-templates>
    </xsl:for-each>
  </xsl:template>

  <!-- Every component level will be treated the same. This allows for minimizing code-repetition, not creating out of memory problems, and faster execution -->
  <xsl:template match="ead:c">

    <!-- The indent level is passed in from the previous component in the hierarchy. That way, we get the correct visual nesting of folder headings based on the nesting of components -->
    <xsl:param name="indentLevel" />

    <!-- Check to see if this component is a series -->
    <xsl:variable name="series">
      <xsl:call-template name="checkForSeries">
        <xsl:with-param name="currentComponent" select="." />
      </xsl:call-template>
    </xsl:variable>

    <!-- The component will be displayed either as a series or a container -->
    <xsl:choose>
      <!-- If this component is a series, display it as a series -->
      <xsl:when test="$series='true'">
        <xsl:call-template name="display-series">
          <xsl:with-param name="currentComponent" select="." />
          <xsl:with-param name="currentIndentLevel" select="indentLevel" />
        </xsl:call-template>  
      </xsl:when>
      <!-- Otherwise, simply display the component as a container.  All non-series components (folder groupings, folders, items) get displayed the same way -->
      <xsl:otherwise>
        <xsl:comment>GOT HERE! (Single Series Details - template match="ead:c")</xsl:comment>
      </xsl:otherwise>
    </xsl:choose>

    <!-- This determines the indent level to pass on to children of this component -->
    <xsl:variable name="nextIndentLevel">
      <xsl:choose>
        <xsl:when test="$series='true'">
          <xsl:value-of select="0" />
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="number($indentLevel + 1)" />
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    
    <xsl:apply-templates select="ead:c">
      <xsl:with-param name="indentLevel" select="$nextIndentLevel" />
    </xsl:apply-templates>

  </xsl:template>


  <!-- Display Series template -->
  <xsl:template name="display-series">

    <xsl:param name="currentComponent" />
    <xsl:param name="currentIndentLevel" />

    

      <xsl:comment>Indent Level</xsl:comment>
      <xsl:comment><xsl:value-of select="$currentIndentLevel"/></xsl:comment>

      <xsl:choose>
        <!-- If at the file level, build-out an <li> element -->
        <xsl:when test="$currentComponent/@level!='file'">
          <xsl:if test="$currentComponent/ead:did/ead:unitid[string-length(text()|*)!=0]">
            <li>       
              <xsl:value-of select="$currentComponent/ead:did/ead:unitid"/>
              <xsl:text>: </xsl:text>
              <xsl:value-of select="$currentComponent/ead:did/ead:unittitle"/>
              <!-- <xsl:attribute name="data-unittitle"><xsl:value-of select="$currentComponent/ead:did/ead:unittitle" /></xsl:attribute> -->
            </li>
          </xsl:if>
          <!-- Call the subseries-list template -->
          <!-- <xsl:call-template name="subseries-list" /> -->
        </xsl:when>
        <!-- If at the series or subseries level, build-out an <h3> element -->
        <xsl:otherwise>
          <!-- <xsl:comment>GOT HERE 2</xsl:comment> -->
          <ul>
            <li>
              <!-- Remove http://edan.si.edu/slideshow/slideshowViewer.htm?damspath= -->
              <xsl:for-each select="$currentComponent/ead:dao">
                <xsl:variable name="damspath">
                    <xsl:call-template name="replace-string">
                      <xsl:with-param name="text" select="$currentComponent/ead:dao/@ns2:href"/>
                      <xsl:with-param name="replace" select="'http://edan.si.edu/slideshow/slideshowViewer.htm?damspath='" />
                      <xsl:with-param name="with" select="''"/>
                    </xsl:call-template>
                </xsl:variable>
                <xsl:variable name="damspathCount"><xsl:number/></xsl:variable>
                <!-- Add the data attribute to the <li> element -->
                <xsl:attribute name="data-damspath-{$damspathCount}"><xsl:value-of select="$damspath" /></xsl:attribute>
                <xsl:attribute name="data-box"><xsl:value-of select="$currentComponent/ead:did/ead:container[@type='Box']" /></xsl:attribute>
                <xsl:attribute name="data-box-id"><xsl:value-of select="$currentComponent/ead:did/ead:container[@type='Box']/@id" /></xsl:attribute>
                <xsl:attribute name="data-folder"><xsl:value-of select="$currentComponent/ead:did/ead:container[@type='Folder']" /></xsl:attribute>
                <xsl:attribute name="data-folder-title"><xsl:value-of select="$currentComponent/ead:did/ead:unittitle" /></xsl:attribute>
                <xsl:attribute name="data-folder-date"><xsl:value-of select="$currentComponent/ead:did/ead:unitdate" /></xsl:attribute>
                <xsl:attribute name="data-series-title"><xsl:value-of select="$currentComponent/../ead:did/ead:unittitle" /></xsl:attribute>
                <xsl:attribute name="data-series-date"><xsl:value-of select="$currentComponent/../ead:did/ead:unitdate" /></xsl:attribute>
                <xsl:attribute name="data-folder-parent-id"><xsl:value-of select="$currentComponent/ead:did/ead:container[@type='Folder']/@parent" /></xsl:attribute>
                <xsl:attribute name="data-ref-id"><xsl:value-of select="$currentComponent/../@id" /></xsl:attribute>
                <xsl:attribute name="data-parent-ref-id"><xsl:value-of select="$currentComponent/@id" /></xsl:attribute>
                <xsl:attribute name="data-titleproper"><xsl:value-of select="//ead:eadheader/ead:filedesc/ead:titlestmt/ead:titleproper" /></xsl:attribute>
              </xsl:for-each>

              <xsl:for-each select="$currentComponent/ead:did">
                <xsl:call-template name="unittitle-stuff"/>
              </xsl:for-each>
              <!-- Adding text here, otherwise, there isn't any space between the title and the physical description -->
              <xsl:text> </xsl:text>
              <xsl:apply-templates select="$currentComponent/ead:did/ead:physdesc"/>
            </li>
          </ul>
        </xsl:otherwise>
      </xsl:choose>

  </xsl:template>

  <!-- ARRANGEMENT WITHIN A SERIES -->
  <!-- This formats an arrangement list embedded within a series -->
<!--   <xsl:template name="subseries-list">
    <xsl:for-each select="ead:arrangement/ead:p">
      <xsl:if test="text()[not(self::list)]">
        <p><xsl:apply-templates select="."/></p>
      </xsl:if>
    </xsl:for-each>
    <xsl:if test="ead:arrangement/ead:p/ead:list[string-length(text()|*)!=0]">
      <ul>
        <xsl:for-each select="ead:arrangement/ead:p/ead:list">
          <xsl:for-each select="ead:item">
            <li><a><xsl:attribute name="href">#series_<xsl:number/></xsl:attribute><xsl:apply-templates select="."/></a></li>
          </xsl:for-each>
        </xsl:for-each>
      </ul>
    </xsl:if>
  </xsl:template> -->

  <!-- A "function" for determining whether the current component is a series or not -->
  <xsl:template name="checkForSeries">
    <xsl:param name="currentComponent" />
    <xsl:choose>
      <xsl:when test="$currentComponent/@level!='' or $currentComponent/@otherlevel!=''"> 
        <xsl:text>true</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>false</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Unit Title Stuff template -->
  <xsl:template name="unittitle-stuff">
    <xsl:choose>
      <!--When unitdate is a child of unittitle.-->
      <xsl:when test="ead:unittitle/ead:unitdate">
        <xsl:for-each select="ead:unittitle">
          <!--Inserts the text of unittitle and any children other than unitdate.-->
          <xsl:apply-templates select="text()|*[not(self::unitdate)]"/>
            <!--Tests to see if the unitdate has content and adds it and separating space if it does.-->
            <xsl:if test="string-length(./ead:unitdate)!=0">
              <xsl:text> </xsl:text>
              <xsl:apply-templates select="./ead:unitdate"/>
            </xsl:if>
        </xsl:for-each>
      </xsl:when>
      <!--When unitdate is a child of did.-->
      <xsl:otherwise>
        <xsl:apply-templates select="ead:unittitle"/>
        <xsl:if test="string-length(ead:unitdate)!=0">
          <xsl:text>, </xsl:text>
          <xsl:apply-templates select="ead:unitdate"/>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!--This template replaces string(s) within a string -->
  <xsl:template name="replace-string">
    <xsl:param name="text"/>
    <xsl:param name="replace"/>
    <xsl:param name="with"/>
    <xsl:choose>
      <xsl:when test="contains($text,$replace)">
        <xsl:value-of select="substring-before($text,$replace)"/>
        <xsl:value-of select="$with"/>
        <xsl:call-template name="replace-string">
          <xsl:with-param name="text" select="substring-after($text,$replace)"/>
          <xsl:with-param name="replace" select="$replace"/>
          <xsl:with-param name="with" select="$with"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$text"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Blockquote template -->
  <xsl:template match="blockquote">
    <blockquote>
      <p><xsl:apply-templates/></p>
    </blockquote>
  </xsl:template>

  <!-- The following templates format the display of various RENDER attributes.-->
  <xsl:template match="*/title">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="*/emph">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="*[@render='bold']">
    <b>
      <xsl:apply-templates />
    </b>
  </xsl:template>

  <xsl:template match="*[@render='italic']">
    <i>
      <xsl:apply-templates />
    </i>
  </xsl:template>

  <xsl:template match="*[@render='underline']">
    <u>
      <xsl:apply-templates />
    </u>
  </xsl:template>

  <xsl:template match="*[@render='sub']">
    <sub>
      <xsl:apply-templates />
    </sub>
  </xsl:template>

  <xsl:template match="*[@render='super']">
    <super>
      <xsl:apply-templates />
    </super>
  </xsl:template>

  <xsl:template match="*[@render='quoted']">
    <xsl:text>"</xsl:text>
    <xsl:apply-templates />
    <xsl:text>"</xsl:text>
  </xsl:template>

  <xsl:template match="*[@render='boldquoted']">
    <b>
      <xsl:text>"</xsl:text>
      <xsl:apply-templates />
      <xsl:text>"</xsl:text>
    </b>
  </xsl:template>

  <xsl:template match="*[@render='boldunderline']">
    <b>
      <u>
        <xsl:apply-templates />
      </u>
    </b>
  </xsl:template>

  <xsl:template match="*[@render='bolditalic']">
    <b>
      <i>
        <xsl:apply-templates />
      </i>
    </b>
  </xsl:template>

  <xsl:template match="*[@render='boldsmcaps']">
    <b>
      <xsl:apply-templates />
    </b>
  </xsl:template>

  <xsl:template match="*[@render='smcaps']">
    <xsl:apply-templates />
  </xsl:template>

</xsl:stylesheet>