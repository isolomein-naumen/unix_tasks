#!/bin/bash
./createAFile.sh
./task Afile Bfile
gzip -k Afile ; gzip -k Bfile
gzip -cd Bfile.gz | ./task Cfile
./task -b100 Afile Dfile
echo
echo Afile size: $((`stat --format %B Afile` * `stat --format %b Afile`))
echo Afile.gz size: $((`stat --format %B Afile.gz` * `stat --format %b Afile.gz`))
echo Bfile size: $((`stat --format %B Bfile` * `stat --format %b Bfile`))
echo Bfile.gz size: $((`stat --format %B Bfile.gz` * `stat --format %b Bfile.gz`))
echo Cfile size: $((`stat --format %B Cfile` * `stat --format %b Cfile`))
echo Dfile size: $((`stat --format %B Dfile` * `stat --format %b Dfile`))
