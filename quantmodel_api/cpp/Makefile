COMPILER=g++
CFLAGS=-O3 -mtune=core2 -march=core2 -Wall -Werror -pthread -DNDEBUG
#CFLAGS=-O3 -Wall -pthread -DNDEBUG
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include
CLIBS=-lnsl -lz -lprotobuf

all: protobuf xmlparser quantmodelapi

quantmodelapi: xmlparser protobuf
	rm -f libquantmodel.a
	ar cq libquantmodel.a xmlParser.o quantmodel.pb.o
	ranlib libquantmodel.a

xmlparser: xmlParser.cpp xmlParser.hpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c xmlParser.cpp -o xmlParser.o

protobuf: quantmodel.proto
	protoc -I=. --proto_path=. --descriptor_set_out=quantmodel.protobin --cpp_out=. quantmodel.proto
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c quantmodel.pb.cc -o quantmodel.pb.o

clean:
	rm -rf *.a *.o *.pb.cc *.pb.h
