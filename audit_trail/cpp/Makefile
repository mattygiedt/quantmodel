#COMPILER=g++
COMPILER=/home/mgiedt/quantmodel/libs/gcc/4.6.0/bin/bin/g++
CFLAGS=-O3 -mtune=core2 -march=core2 -fwhole-program -flto -Wall -Werror -pthread -DNDEBUG
#CFLAGS=-ggdb3 -Wall -pthread -DNDEBUG -fPIC
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include 
CLIBS=-llog4cpp -lnsl -lzmq -lgmp -lz -lprotobuf
COBJS=../../core/cpp/quantmodel.pb.o ../../core/cpp/xmlParser.o

all: audit_event_parser

audit_event_parser: AuditEventParser.cpp 
	${COMPILER} ${CFLAGS} ${CINCLUDE} AuditEventParser.cpp ${COBJS} -o AuditEventParser ${CLIBS}

clean:
	rm -rf *.o *.log AuditEventParser
