#COMPILER=g++
COMPILER=/home/mgiedt/quantmodel/libs/gcc/4.6.0/bin/bin/g++
CFLAGS=-O3 -pipe -fwhole-program -Wall -Werror -pthread -DNDEBUG
#CFLAGS=-ggdb3 -Wall -pthread -DNDEBUG
CINCLUDE = -I./ -I../../core/cpp -I/usr/local/include -I/usr/include/mysql -I/usr/local/include/mysql++ 
CLIBS = -llog4cpp -lnsl -lzmq -lgmp -lmysqlpp -lz -lprotobuf ../../core/cpp/libquantmodel.a
COBJS = 

all: database_engine

database_engine: DatabaseEngine.cpp ${COBJS}
	${COMPILER} ${CFLAGS} ${CINCLUDE} DatabaseEngine.cpp ${COBJS} -o DatabaseEngine ${CLIBS} 

deploy_dev:
	cp DatabaseEngine ../../../../../release/dev/database_engine/bin

deploy_uat:
	cp DatabaseEngine ../../../../../release/uat/database_engine/bin

clean:
	rm -rf *.o *.log DatabaseEngine 
