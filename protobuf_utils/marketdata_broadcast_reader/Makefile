COMPILER=g++
CFLAGS=-O3 -mtune=core2 -march=core2 -Wall -Werror -pthread -DNDEBUG
#CFLAGS=-O3 -Wall -pthread -DNDEBUG
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include
CLIBS=-llog4cpp -lnsl -lz -lprotobuf -lgmpxx
COBJS=../../core/cpp/quantmodel.pb.o ../../core/cpp/xmlParser.o 

all: marketdata_broadcast_reader

marketdata_broadcast_reader: MarketDataBroadcastReader.cpp
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} MarketDataBroadcastReader.cpp ${COBJS} -o MarketDataBroadcastReader ${CLIBS} 

clean:
	rm -rf *.o MarketDataBroadcastReader
