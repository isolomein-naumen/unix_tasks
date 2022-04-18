#!/bin/bash
echo -n 1 > Afile
dd if=/dev/zero bs=2 count=4999 >> Afile
echo -n 1 >> Afile
dd if=/dev/zero bs=16 count=261519 >> Afile
echo -n 1 >> Afile
