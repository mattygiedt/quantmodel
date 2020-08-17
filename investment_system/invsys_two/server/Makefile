COMPILER=/home/mgiedt/quantmodel/libs/gcc/4.6.0/bin/bin/g++
DBGFLAGS=-ggdb3 -Wall -Werror -pthread -DNDEBUG
CFLAGS=-O3 -m64 -fwhole-program -flto -Wall -Werror -pthread -DNDEBUG
CINCLUDE=-I./ -I/usr/local/include
CLIBS=-llog4cpp -lnsl -lzmq -lz -lprotobuf 
QMAPI=-L./ libqmapi.a
CORE=../../../core/cpp/quantmodel.pb.o ../../../core/cpp/xmlParser.o
COBJS= 

all: investment_system 

investment_system: InvestmentSystemTwo.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} InvestmentSystemTwo.cpp -o InvestmentSystemTwo ${CLIBS} ${QMAPI}

clean:
	rm -rf *.o InvestmentSystemTwo
