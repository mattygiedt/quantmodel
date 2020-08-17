COMPILER=g++
CFLAGS=-O3 -mtune=core2 -march=core2 -Wall -pthread -DNDEBUG
#CFLAGS=-O3 -Wall -pthread -DNDEBUG
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include
CLIBS=-llog4cpp -lnsl -lz -lprotobuf -lgmpxx
COBJS=../../core/cpp/quantmodel.pb.o ../../core/cpp/xmlParser.o 

all: protobuf_converter_v1 protobuf_converter_v2

protobuf_converter_v1: ProtobufConverter_v1.cpp
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} ProtobufConverter_v1.cpp ${COBJS} -o ProtobufConverter_v1 ${CLIBS} 

protobuf_converter_v2: ProtobufConverter_v2.cpp
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} ProtobufConverter_v2.cpp ${COBJS} -o ProtobufConverter_v2 ${CLIBS} 

clean:
	rm -rf *.o ProtobufConverter_v1 ProtobufConverter_v2
