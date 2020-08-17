#COMPILER=g++
#CFLAGS=-ggdb3 -Wall -pthread -DNDEBUG
COMPILER=/home/mgiedt/quantmodel/libs/gcc/4.6.0/bin/bin/g++
CFLAGS=-O3 -pipe -fwhole-program -Wall -Werror -pthread -DNDEBUG 
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include -I/usr/include/mysql -I/usr/local/include/mysql++
CLIBS=-llog4cpp -lnsl -lzmq -lgmpxx -lgmp -lmysqlpp -lz -lprotobuf ../../core/cpp/libquantmodel.a
COBJS= 

all:	persistence_engine 

persistence_engine: PersistenceEngine.cpp ${COBJS}
	${COMPILER} ${CFLAGS} ${CINCLUDE} PersistenceEngine.cpp ${COBJS} -o PersistenceEngine ${CLIBS}

deploy_dev:
	cp PersistenceEngine ../../../../../release/dev/persistence_engine/bin

deploy_uat:
	cp PersistenceEngine ../../../../../release/uat/persistence_engine/bin

clean:
	rm -rf *.o PersistenceEngine
