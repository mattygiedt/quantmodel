COMPILER=/home/mgiedt/quantmodel/libs/gcc/4.6.0/bin/bin/g++
DBGFLAGS=-ggdb3 -Wall -Werror -pthread -DNDEBUG
CFLAGS=-O3 -m64 -fwhole-program -flto -Wall -Werror -pthread -DNDEBUG
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include
CLIBS=-llog4cpp -lnsl -lzmq -lz -lprotobuf
CORE=../../core/cpp/quantmodel.pb.o ../../core/cpp/xmlParser.o
COBJS=

all: limit_order_book_viewer limit_order_book_proxy

limit_order_book_viewer: LimitOrderBookViewer.cpp ${COBJS}
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} LimitOrderBookViewer.cpp ${CORE} ${COBJS} -o LimitOrderBookViewer ${CLIBS}
	${COMPILER} ${DBGFLAGS} -fwhole-program ${CINCLUDE} LimitOrderBookViewer.cpp ${CORE} ${COBJS} -o LimitOrderBookViewerDebug ${CLIBS}

limit_order_book_proxy: LimitOrderBookProxy.cpp ${COBJS}
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} LimitOrderBookProxy.cpp ${CORE} ${COBJS} -o LimitOrderBookProxy ${CLIBS}

clean:
	rm -rf *.o LimitOrderBookViewer LimitOrderBookProxy
