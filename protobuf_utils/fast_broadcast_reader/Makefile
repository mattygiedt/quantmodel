COMPILER=g++
CFLAGS=-O3 -mtune=core2 -march=core2 -Wall -Werror -pthread -DNDEBUG
#CFLAGS=-O3 -Wall -pthread -DNDEBUG
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include
CLIBS=-llog4cpp -lnsl -lz -lprotobuf -lgmpxx
COBJS=../../core/cpp/quantmodel.pb.o ../../core/cpp/xmlParser.o 

all: fast_broadcast_reader

fast_broadcast_reader: FastBroadcastReader.cpp
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} FastBroadcastReader.cpp ${COBJS} -o FastBroadcastReader ${CLIBS} 

clean:
	rm -rf *.o FastBroadcastReader
