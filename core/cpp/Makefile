#COMPILER = g++
COMPILER=/home/mgiedt/quantmodel/libs/gcc/4.6.0/bin/bin/g++
CFLAGS=-O3 -pipe -flto -Wall -DNDEBUG -fPIC
#CFLAGS=-ggdb3 -Wall -pthread -DNDEBUG -fPIC
CINCLUDE = -I./ -I/usr/local/include 
CLIBS = -llog4cpp -lnsl -lzmq -lgmp -lz
LIBFILES = quantmodel.pb.cpp xmlParser.cpp
LIBOFILES =  $(LIBFILES:%.cpp=%.o)

all: protobuf libquantmodel

protobuf: quantmodel.proto
	protoc -I=. --proto_path=. --descriptor_set_out=quantmodel.protobin --cpp_out=. --java_out=. quantmodel.proto
	mv quantmodel.pb.cc quantmodel.pb.cpp
	cp quantmodel/Quantmodel.java ../../task_engine/java/src/com/quantmodel/engine
	cp quantmodel.protobin ../../../../client/v2.0/protobuf

libquantmodel: $(LIBOFILES)
	rm -f $@.a
	ar cq $@.a $(LIBOFILES)
	ranlib $@.a

quantmodel.pb.o: quantmodel.pb.cpp quantmodel.pb.h
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c quantmodel.pb.cpp -o quantmodel.pb.o ${CLIBS}

xmlParser.o: xmlParser.cpp xmlParser.hpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c xmlParser.cpp -o xmlParser.o ${CLIBS}

.PHONY: clean

clean:
	rm -f *.o *.a
