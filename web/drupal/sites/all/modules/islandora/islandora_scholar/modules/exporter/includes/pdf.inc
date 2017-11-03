<?php

/**
 * @file
 * An implementation of Nicola Asuni's TCPDF php library.
 *
 *  Description : Example 001 for TCPDF class
 *               Default Header and Footer
 *
 * Author: Nicola Asuni
 *
 * (c) Copyright:
 *               Nicola Asuni
 *               Tecnick.com s.r.l.
 *               Via Della Pace, 11
 *               09044 Quartucciu (CA)
 *               ITALY
 *               www.tecnick.com
 *               info@tecnick.com
 */

/**
 * Creates an PDF document using TCPDF.
 *
 * @param string $html_input
 *   The HTML to be converted into a PDF.
 *
 * @param string $output_filename
 *   The location to save the generated PDF.
 */
function citation_exporter_create_pdf($html_input, $output_filename) {
  module_load_include('php', 'citation_exporter', '/lib/tcpdf/tcpdf');

  // Create new PDF document.
  $pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, TRUE, 'UTF-8', FALSE, FALSE);

  // Set default monospaced font.
  $pdf->SetDefaultMonospacedFont(PDF_FONT_MONOSPACED);

  // Set margins.
  $pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
  $pdf->SetHeaderMargin(PDF_MARGIN_HEADER);
  $pdf->SetFooterMargin(PDF_MARGIN_FOOTER);

  // Set auto page breaks.
  $pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);

  // Set image scale factor.
  $pdf->setImageScale(PDF_IMAGE_SCALE_RATIO);

  // Set default font subsetting mode.
  $pdf->setFontSubsetting(TRUE);

  // Set font.
  // Dejavusans is a UTF-8 Unicode font, if you only need to
  // Print standard ASCII chars, you can use core fonts like
  // Helvetica or times to reduce file size.
  $pdf->SetFont('dejavusans', '', 14, '', TRUE);

  // Add a page.
  // This method has several options, check the source code documentation for
  // more information.
  $pdf->AddPage();

  // Print text using writeHTMLCell().
  $pdf->writeHTMLCell($w = 0, $h = 0, $x = '', $y = '', $html_input, $border = 0, $ln = 1, $fill = 0, $reseth = TRUE, $align = '', $autopadding = TRUE);

  // Close and output PDF document. This method has several options, check the
  // source code documentation for more information.
  $pdf->Output($output_filename, 'F');
}
