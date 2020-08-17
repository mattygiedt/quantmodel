COMPILER=g++
DBGFLAGS=-ggdb3 -Wall -Werror -pthread
CFLAGS=-O3 -m64 -fwhole-program -Wall -Werror -pthread -DNDEBUG
CINCLUDE=-I./ -I/usr/local/include
CLIBS=-lnsl -lzmq -lz -lprotobuf 
QMAPI=-L./ libqmapi.a

all: investment_system

investment_system: ExampleInvestmentSystem.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} ExampleInvestmentSystem.cpp -o ExampleInvestmentSystem ${CLIBS} ${QMAPI}

clean:
	rm -rf *.o ExampleInvestmentSystem
