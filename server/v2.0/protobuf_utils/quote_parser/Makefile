COMPILER=g++
CFLAGS=-O3 -mtune=core2 -march=core2 -Wall -Werror -pthread -DNDEBUG
#CFLAGS=-O3 -Wall -pthread -DNDEBUG
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include
CLIBS=-llog4cpp -lnsl -lz -lprotobuf -lgmpxx
COBJS=../../core/cpp/quantmodel.pb.o ../../core/cpp/xmlParser.o 

all: protobuf_parser

protobuf_parser: ProtobufParser.cpp
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} ProtobufParser.cpp ${COBJS} -o ProtobufParser ${CLIBS} 

clean:
	rm -rf *.o ProtobufParser
