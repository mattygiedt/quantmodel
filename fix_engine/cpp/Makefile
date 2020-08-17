COMPILER=g++
CFLAGS=-O3 -pipe -mtune=core2 -march=core2 -Wall -Werror -pthread -DNDEBUG
#CFLAGS=-ggdb3 -Wall -pthread -DNDEBUG
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include -I/usr/include/mysql -I/usr/local/include/mysql++ 
CLIBS=-llog4cpp -lnsl -lzmq -lgmp -lmysqlpp -lz -lprotobuf -lquickfix
CORE=../../core/cpp/quantmodel.pb.o ../../core/cpp/xmlParser.o
COBJS=TradingTechnologiesOrder.o TradingTechnologiesPrice.o GlobexOrder.o

all: fix_engine 

fix_engine: FixEngine.cpp ${COBJS}
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} FixEngine.cpp ${COBJS} ${CORE} -o FixEngine ${CLIBS}

TradingTechnologiesOrder.o: TradingTechnologiesOrder.cpp TradingTechnologiesOrder.hpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c TradingTechnologiesOrder.cpp

TradingTechnologiesPrice.o: TradingTechnologiesPrice.cpp TradingTechnologiesPrice.hpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c TradingTechnologiesPrice.cpp

GlobexOrder.o: GlobexOrder.cpp GlobexOrder.hpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c GlobexOrder.cpp

deploy_dev:
	cp FixEngine ../../../../../release/dev/fix_engine/bin

deploy_uat:
	cp FixEngine ../../../../../release/uat/fix_engine/bin

clean:
	rm -rf *.o FixEngine 
