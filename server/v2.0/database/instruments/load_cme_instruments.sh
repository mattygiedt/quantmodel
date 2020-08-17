#! /bin/sh

mysql -u root -ppassword dev < sql/es.sql
mysql -u root -ppassword dev < sql/cl.sql
mysql -u root -ppassword dev < sql/gc.sql
