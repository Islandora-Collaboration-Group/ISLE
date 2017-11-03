@echo off

if x==%1x goto howto
java -cp marc4j.jar;marcxml.jar; %1 %2 %3 %4 %5
goto end

:howto
echo Usage marcxml usage:
echo marcxml classname [parameters]
echo. 
echo Where classname can be one of the following:
echo.
echo   To convert MARC to MARC21slim xml:
echo      gov.loc.marcxml.MARC2MARC21slim
echo.
echo   To convert MARC21slim xml to MARC:
echo      gov.loc.marcxml.MARC21slim2MARC
echo.
echo   To convert MARC to MARC21BIG xml:
echo      gov.loc.marcxml.MARC2MARCBIG
echo.
echo   To convert MARC to MODS xml
echo      gov.loc.marcxml.MARC2MODS
echo.
echo   To use any stylesheet when converting from MARC records
echo      gov.loc.marcxml.GenericMarcXmlWriter
echo.
echo   To convert MARC records to UNICODE
echo      gov.loc.marc.UnicodeConverter
echo.
echo   To validate MARC records
echo      gov.loc.marc.Validator
echo.
                                                         
:end

