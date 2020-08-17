#COMPILER=g++
COMPILER=/home/mgiedt/quantmodel/libs/gcc/4.6.0/bin/bin/g++
CFLAGS=-O3 -pipe -mtune=core2 -march=core2 -fwhole-program -flto -Wall -pthread -DNDEBUG
#CFLAGS=-ggdb3 -Wall -pthread -DNDEBUG -fPIC
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include -I/usr/include/mysql -I/usr/local/include/mysql++
CLIBS=-llog4cpp -lnsl -lzmq -lgmp -lz -lprotobuf -lmysqlpp
COBJS=../../core/cpp/quantmodel.pb.o ../../core/cpp/xmlParser.o

all: last_price_engine

last_price_engine: LastPriceEngine.cpp 
	${COMPILER} ${CFLAGS} ${CINCLUDE} LastPriceEngine.cpp ${COBJS} -o LastPriceEngine ${CLIBS}

deploy_dev:
	cp LastPriceEngine ../../../../../release/dev/lastprice_engine/bin

clean:
	rm -rf *.o *.log LastPriceEngine
