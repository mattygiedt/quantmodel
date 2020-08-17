#!/bin/sh

# http://steve-parker.org/sh/exitcodes.shtml
check_errs()
{
  # Function. Parameter 1 is the return code
  # Para. 2 is text to display on failure.
  if [ "${1}" -ne "0" ]; then
    echo "ERROR # ${1} : ${2}"
    # as a bonus, make our script exit with the right error code.
    exit ${1}
  fi
}

cd database/procs
./generate.sh


cd ../../core/cpp
make clean
make
check_errs $? "failed to make bdsq library"


cd ../../database_engine/cpp
make clean
make
check_errs $? "failed to make database engine"


cd ../../persistence_engine/cpp
make clean
make
check_errs $? "failed to make persistence engine"


cd ../../dealing_engine/cpp
make clean
make
check_errs $? "failed to make dealing engine"


cd ../../fix_engine/cpp
make clean
make
check_errs $? "failed to make fix engine"


cd ../../fast_engine/cpp
make clean
make
check_errs $? "failed to make fast engine"


#cd ../../fix_executor/cpp
#make clean
#make
#check_errs $? "failed to make fix executor"


cd ../../marketdata_engine/cpp
make clean
make
check_errs $? "failed to make market data engine"


#cd ../../lastprice_engine/cpp
#make clean
#make
#check_errs $? "failed to make last price engine"


cd ../../client_adapter/cpp
make clean
make
check_errs $? "failed to make client adapter"

cd ../../task_engine/java
ant 
check_errs $? "failed to make task engine"
