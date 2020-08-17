#COMPILER=g++
#CFLAGS=-ggdb3 -Wall -pthread -DNDEBUG
COMPILER=/home/mgiedt/quantmodel/libs/gcc/4.6.0/bin/bin/g++
CFLAGS=-std=c++0x -O3 -pipe -mtune=core2 -march=core2 -flto -Wall -pthread -DNDEBUG
QUICKFAST=/home/mgiedt/quantmodel/libs/quickfast/quickfast_1_3
BOOST=/home/mgiedt/quantmodel/libs/boost/boost_1_45_0
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include -I${QUICKFAST}/src
COLLECTOR_CLIBS=-lzmq -lprotobuf
ENGINE_CLIBS=-L${BOOST}/lib -L${QUICKFAST}/lib -llog4cpp -lgmp -lzmq -lprotobuf -lboost_thread -lboost_system -lboost_filesystem -lQuickFAST
READER_CLIBS=-L/home/mgiedt/quantmodel/libs/boost/boost_1_45_0/lib -L/home/mgiedt/quantmodel/libs/quickfast/quickfast_1_3/lib -lboost_thread -lboost_system -lboost_filesystem -lQuickFAST -lprotobuf
CORE_COBJS=../../core/cpp/quantmodel.pb.o ../../core/cpp/xmlParser.o 
ENGINE_COBJS=CMEMarketDataProtobuf.o CMEValueMessageBuilder.o CMESettlementConsumer.o CMEMessageConsumer.o CMEDataSource.o 

all: cme_fast_2_protobuf cme_settlement_engine cme_fast_engine cme_fast_data_collector cme_message_reader ${ENGINE_COBJS}

cme_message_reader: CMEFastMessageReader.cpp ${ENGINE_COBJS}
	${COMPILER} ${CFLAGS} -fwhole-program -I./ -I${CINCLUDE} CMEFastMessageReader.cpp ${CORE_COBJS} ${ENGINE_COBJS} -o CMEFastMessageReader ${ENGINE_CLIBS}

cme_fast_2_protobuf: CMEFast2Protobuf.cpp ${ENGINE_COBJS}
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} CMEFast2Protobuf.cpp ${CORE_COBJS} ${ENGINE_COBJS} -o CMEFast2Protobuf ${ENGINE_CLIBS}

cme_settlement_engine: CMESettlementEngine.cpp ${ENGINE_COBJS}
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} CMESettlementEngine.cpp ${CORE_COBJS} ${ENGINE_COBJS} -o CMESettlementEngine ${ENGINE_CLIBS}

cme_fast_engine: CMEFastEngine.cpp ${ENGINE_COBJS}
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} CMEFastEngine.cpp ${CORE_COBJS} ${ENGINE_COBJS} -o CMEFastEngine ${ENGINE_CLIBS} 

cme_fast_data_collector: CMEFastDataCollector.cpp ${ENGINE_COBJS}
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} CMEFastDataCollector.cpp ${CORE_COBJS} ${ENGINE_COBJS} -o CMEFastDataCollector ${ENGINE_CLIBS}

CMESettlementConsumer.o: CMESettlementConsumer.cpp CMESettlementConsumer.hpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CMESettlementConsumer.cpp -o CMESettlementConsumer.o

CMEMessageConsumer.o: CMEMessageConsumer.cpp CMEMessageConsumer.hpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CMEMessageConsumer.cpp -o CMEMessageConsumer.o

CMEDataSource.o: CMEDataSource.cpp CMEDataSource.hpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CMEDataSource.cpp -o CMEDataSource.o

CMERequestHandler.o: CMERequestHandler.cpp CMERequestHandler.hpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CMERequestHandler.cpp -o CMERequestHandler.o

CMEValueMessageBuilder.o: CMEValueMessageBuilder.cpp CMEValueMessageBuilder.hpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CMEValueMessageBuilder.cpp -o CMEValueMessageBuilder.o

CMEMarketDataProtobuf.o: CMEMarketDataProtobuf.proto
	protoc -I=. --proto_path=. --descriptor_set_out=CMEMarketDataProtobuf.protobin --cpp_out=. CMEMarketDataProtobuf.proto
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CMEMarketDataProtobuf.pb.cc -o CMEMarketDataProtobuf.o

clean:
	rm -rf *.o CMEFastEngine CMESettlementEngine CMEFast2Protobuf CMEFastDataCollector CMEFastMessageReader
