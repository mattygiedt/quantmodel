#!/bin/sh

ftp -n 76.74.96.208 <<END_SCRIPT
quote USER bdsqftp
quote PASS 7735513289
bin
put task-engine-1.0.0.jar
quit
END_SCRIPT
exit 0
