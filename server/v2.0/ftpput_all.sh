#!/bin/sh

cd database_engine/cpp
./ftp_put.sh

cd ../../persistence_engine/cpp
./ftp_put.sh

cd ../../dealing_engine/cpp
./ftp_put.sh

cd ../../fix_engine/cpp
./ftp_put.sh

cd ../../fast_engine/cpp
./ftp_put.sh

#cd ../../fix_executor/cpp
#make clean
#make
#check_errs $? "failed to make fix executor"

cd ../../marketdata_engine/cpp
./ftp_put.sh

#cd ../../lastprice_engine/cpp
#make clean
#make
#check_errs $? "failed to make last price engine"

cd ../../client_adapter/cpp
./ftp_put.sh

cd ../../task_engine/java
./ftp_put.sh
