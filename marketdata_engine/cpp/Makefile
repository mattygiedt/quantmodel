#COMPILER=g++
COMPILER=/home/mgiedt/quantmodel/libs/gcc/4.6.0/bin/bin/g++
#CFLAGS=-O3 -pipe -mtune=core2 -march=core2 -fwhole-program -flto -Wall -pthread -DNDEBUG
CFLAGS=-ggdb3 -Wall -pthread -DNDEBUG -fPIC
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include 
CLIBS=-llog4cpp -lnsl -lzmq -lgmp -lz -lprotobuf
COBJS=../../core/cpp/quantmodel.pb.o ../../core/cpp/xmlParser.o

all: marketdata_engine marketdata_collector

marketdata_engine: MarketDataEngine.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} MarketDataEngine.cpp ${COBJS} -o MarketDataEngine ${CLIBS}

marketdata_collector: MarketDataCollector.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} MarketDataCollector.cpp ${COBJS} -o MarketDataCollector ${CLIBS}

deploy_dev:
	cp MarketDataEngine ../../../../../release/dev/marketdata_engine/bin
	cp MarketDataCollector ../../../../../release/dev/marketdata_collector/bin

deploy_uat:
	cp MarketDataEngine ../../../../../release/uat/marketdata_engine/bin
	cp MarketDataCollector ../../../../../release/uat/marketdata_collector/bin

clean:
	rm -rf *.o *.log MarketDataEngine MarketDataCollector
