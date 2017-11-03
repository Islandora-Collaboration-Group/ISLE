<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:variable name="projections">
        <!-- begin USGS values http://erg.usgs.gov/isb/pubs/MapProjections/projections.html -->
        <xsl:text>Globe</xsl:text>
        <xsl:text>Mercator</xsl:text>
        <xsl:text>Transverse Mercator</xsl:text>
        <xsl:text>Oblique Mercator</xsl:text>
        <xsl:text>Space Oblique Mercator</xsl:text>
        <xsl:text>Miller Cylindrical</xsl:text>
        <xsl:text>Robinson</xsl:text>
        <xsl:text>Sinusoidal Equal</xsl:text>
        <xsl:text>Area</xsl:text>
        <xsl:text>Orthographic</xsl:text>
        <xsl:text>Stereographic</xsl:text>
        <xsl:text>Gnomonic</xsl:text>
        <xsl:text>Azimuthal Equalidistant</xsl:text>
        <xsl:text>Lambert Azimuthal Equal Area</xsl:text>
        <xsl:text>Albers Equal Area Conic</xsl:text>
        <xsl:text>Lambert Conformal Conic</xsl:text>
        <xsl:text>Equidistant Conic</xsl:text>
        <xsl:text>Polyonic</xsl:text>
        <xsl:text>Biplolar Oblique Conic Conformal</xsl:text>
        <!-- begin CSDGM values http://fgdc.er.usgs.gov/metadata/csdgm/04.html .  Normalized values have removed the underscore character -->
        <xsl:text>Albers_Conical_Equal_Area</xsl:text>
        <xsl:text>Albers Conical Equal Area</xsl:text>
        <xsl:text>Azimuthal_Equidistant</xsl:text>
        <xsl:text>Azimuthal Equidistant</xsl:text>
        <xsl:text>Equidistant_Conic</xsl:text>
        <xsl:text>Equidistant Conic</xsl:text>
        <xsl:text>Equirectangular</xsl:text>
        <xsl:text>General_Vertical_Near-sided_Perspective</xsl:text>
        <xsl:text>General Vertical Near-sided Perspective</xsl:text>
        <xsl:text>Gnomonic</xsl:text>
        <xsl:text>Lambert_Azimuthal_Equal_Area </xsl:text>
        <xsl:text>Lambert Azimuthal Equal Area </xsl:text>
        <xsl:text>Lambert_Conformal_Conic</xsl:text>
        <xsl:text>Lambert Conformal Conic</xsl:text>
        <xsl:text>Mercator</xsl:text>
        <xsl:text>Modified_Stereographic_for_Alaska</xsl:text>
        <xsl:text>Modified Stereographic for Alaska</xsl:text>
        <xsl:text>Miller_Cylindrical</xsl:text>
        <xsl:text>Miller Cylindrical</xsl:text>
        <xsl:text>Oblique_Mercator</xsl:text>
        <xsl:text>Oblique Mercator</xsl:text>
        <xsl:text>Orthographic</xsl:text>
        <xsl:text>Polar_Stereographic</xsl:text>
        <xsl:text>Polar Stereographic</xsl:text>
        <xsl:text>Polyconic</xsl:text>
        <xsl:text>Robinson</xsl:text>
        <xsl:text>Sinusoidal</xsl:text>
        <xsl:text>Space_Oblique_Mercator_(Landsat)</xsl:text>
        <xsl:text>Space Oblique Mercator (Landsat)</xsl:text>
        <xsl:text>Stereographic</xsl:text>
        <xsl:text>Transverse_Mercator</xsl:text>
        <xsl:text>Transverse Mercator</xsl:text>
        <xsl:text>van_der_Grinten</xsl:text>
        <xsl:text>van der Grinten</xsl:text>
        <xsl:text>Map_Projection_Parameters</xsl:text>
        <xsl:text>Map Projection Parameters</xsl:text>
    </xsl:variable>
</xsl:stylesheet>
