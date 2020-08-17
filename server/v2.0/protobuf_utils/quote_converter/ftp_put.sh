#!/bin/sh

ftp -n 76.74.96.208 <<END_SCRIPT
quote USER bdsqftp
quote PASS 7735513289
bin
put ProtobufConverter_v1
put ProtobufConverter_v2
quit
END_SCRIPT
exit 0

