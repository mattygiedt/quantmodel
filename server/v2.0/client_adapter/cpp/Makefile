#COMPILER=g++
COMPILER=/home/mgiedt/quantmodel/libs/gcc/4.6.0/bin/bin/g++
CFLAGS=-O3 -pipe -flto -Wall -Werror -pthread -DNDEBUG
#CFLAGS=-ggdb3 -Wall -pthread -DNDEBUG
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include
CLIBS=-llog4cpp -lnsl -lzmq -lz -lprotobuf
COBJS=../../core/cpp/xmlParser.o ../../core/cpp/quantmodel.pb.o

all: client_adapter client_proxy investment_system_proxy investment_system_adapter

client_proxy: ClientProxy.cpp
	${COMPILER} ${CFLAGS} -fwhole-program  ${CINCLUDE} ClientProxy.cpp ${COBJS} -o ClientProxy ${CLIBS}

client_adapter: ClientAdapter.cpp
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} ClientAdapter.cpp ${COBJS} -o ClientAdapter ${CLIBS} 

investment_system_proxy: InvestmentSystemProxy.cpp
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} InvestmentSystemProxy.cpp ${COBJS} -o InvestmentSystemProxy ${CLIBS}

investment_system_adapter: InvestmentSystemAdapter.cpp
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} InvestmentSystemAdapter.cpp ${COBJS} -o InvestmentSystemAdapter ${CLIBS}


deploy_dev:
	cp ClientAdapter ../../../../../release/dev/client_adapter/bin
	cp ClientProxy ../../../../../release/dev/client_proxy/bin

deploy_uat:
	cp ClientAdapter ../../../../../release/uat/client_adapter/bin
	cp ClientProxy ../../../../../release/uat/client_proxy/bin

clean:
	rm -rf *.o ClientProxy ClientAdapter InvestmentSystemProxy InvestmentSystemAdapter
