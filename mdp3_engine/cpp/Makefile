COMPILER=g++
CFLAGS=-ggdb3 -Wall -pthread -DNDEBUG
#COMPILER=/home/mgiedt/quantmodel/libs/gcc/4.6.0/bin/bin/g++
#CFLAGS=-std=c++0x -O3 -pipe -mtune=core2 -march=core2 -flto -Wall -pthread -DNDEBUG
#QUICKFAST=/home/mgiedt/quantmodel/libs/quickfast/quickfast_1_3
BOOST=/home/mgiedt/quantmodel/libs/boost/boost_1_45_0
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include -I${BOOST}/include
COLLECTOR_CLIBS=
ENGINE_CLIBS=
READER_CLIBS=
CORE_COBJS=../../core/cpp/xmlParser.o 
ENGINE_COBJS=

all: template_parser message_reader ${ENGINE_COBJS}

template_parser: MDP3TemplateParser.cpp ${ENGINE_COBJS}
	${COMPILER} ${CFLAGS} -fwhole-program -I./ -I${CINCLUDE} MDP3TemplateParser.cpp ${CORE_COBJS} ${ENGINE_COBJS} -o MDP3TemplateParser ${ENGINE_CLIBS}

message_reader: MDP3MessageReader.cpp ${ENGINE_COBJS}
	${COMPILER} ${CFLAGS} -fwhole-program -I./ -I${CINCLUDE} MDP3MessageReader.cpp ${CORE_COBJS} ${ENGINE_COBJS} -o MDP3MessageReader ${ENGINE_CLIBS}

CMEMarketDataProtobuf.o: CMEMarketDataProtobuf.proto
	protoc -I=. --proto_path=. --descriptor_set_out=CMEMarketDataProtobuf.protobin --cpp_out=. CMEMarketDataProtobuf.proto
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CMEMarketDataProtobuf.pb.cc -o CMEMarketDataProtobuf.o

clean:
	rm -rf *.o MDP3TemplateParser MDP3MessageReader
