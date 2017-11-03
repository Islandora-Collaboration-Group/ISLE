#!/bin/bash
clear
echo 'testing';
#/opt/libreoffice3.4/program/soffice --headless --nologo --invisible --nofirststartwizard -convert-to pdf:$1 -outdir $2
cd /Applications/LibreOffice.app/Contents/program
./soffice --headless --nologo --invisible --nofirststartwizard -convert-to pdf $1 -outdir $2