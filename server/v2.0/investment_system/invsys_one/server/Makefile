COMPILER=/home/mgiedt/quantmodel/libs/gcc/4.6.0/bin/bin/g++
DBGFLAGS=-ggdb3 -Wall -Werror -pthread -DNDEBUG
CFLAGS=-O3 -m64 -fwhole-program -flto -Wall -Werror -pthread -DNDEBUG
CINCLUDE=-I./ -I../../../core/cpp -I/usr/local/include
CLIBS=-llog4cpp -lnsl -lzmq -lgmpxx -lgmp -lmysqlpp -lz -lprotobuf
CORE=../../../core/cpp/quantmodel.pb.o ../../../core/cpp/xmlParser.o
COBJS=invsys_one.pb.o 

all: investment_system harness backtester invsys_one.pb.o

investment_system: InvestmentSystemOne.cpp ${COBJS}
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} InvestmentSystemOne.cpp ${CORE} ${COBJS} -o InvestmentSystemOne ${CLIBS}

debug_investment_system: InvestmentSystemOne.cpp ${COBJS}
	${COMPILER} ${DBGFLAGS} -fwhole-program ${CINCLUDE} InvestmentSystemOne.cpp ${CORE} ${COBJS} -o DebugInvestmentSystemOne ${CLIBS}

harness: InvestmentSystemOneHarness.cpp ${COBJS}
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} InvestmentSystemOneHarness.cpp ${CORE} ${COBJS} -o InvestmentSystemOneHarness ${CLIBS}

backtester: InvestmentSystemOneBacktester.cpp ${COBJS}
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} InvestmentSystemOneBacktester.cpp ${CORE} ${COBJS} -o InvestmentSystemOneBacktester ${CLIBS}

invsys_one.pb.o: invsys_one.proto
	protoc -I=. --descriptor_set_out=invsys_one.protobin --cpp_out=. invsys_one.proto
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c invsys_one.pb.cc -o invsys_one.pb.o ${CLIBS}
	cp invsys_one.protobin ../client/protobuf

clean:
	rm -rf *.o InvestmentSystemOne InvesmentSystemOneHarness InvestmentSystemOneBacktester
