#COMPILER=g++
COMPILER=/home/mgiedt/quantmodel/libs/gcc/4.6.0/bin/bin/g++
CFLAGS=-O3 -pipe -mtune=core2 -march=core2 -flto -Wall -Werror -pthread -DNDEBUG
#CFLAGS=-ggdb3 -Wall -pthread -DNDEBUG
CINCLUDE=-I./ -I../../core/cpp -I/usr/local/include -I/usr/include/mysql -I/usr/local/include/mysql++ 
CLIBS=-llog4cpp -lnsl -lzmq -lgmp -lz -lprotobuf
CORE=../../core/cpp/quantmodel.pb.o ../../core/cpp/xmlParser.o 
COBJS=DealingEngineParameters.o ActiveSet.o CreateOrder.o UpdateOrder.o CancelOrder.o ReleaseOrder.o CreateRelease.o CancelRelease.o ReplaceRelease.o ReleaseRejected.o CancelRejected.o ReleaseRestated.o ReleaseCanceled.o ReleaseReplaced.o CreateExecution.o ExecutionCanceled.o QueryData.o DealingStatus.o FixEngineLogin.o FixEngineLogout.o MarketDataSubscribe.o MarketDataUnsubscribe.o LogoutFixEngine.o LoginFixEngine.o CreateDraftOrder.o ValidateRelease.o ReloadStaticData.o CreateAuditEvent.o ExecuteSignal.o ExecuteDraftOrder.o

all:	dealing_engine dealing_engine_tester

dealing_engine_tester: DealingEngineTester.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} DealingEngineTester.cpp ${CORE} -o DealingEngineTester ${CLIBS}

dealing_engine: DealingEngine.cpp ${COBJS}
	${COMPILER} ${CFLAGS} -fwhole-program ${CINCLUDE} DealingEngine.cpp ${CORE} ${COBJS} -o DealingEngine ${CLIBS} 

DealingEngineParameters.o: DealingEngineParameters.cpp DealingEngineParameters.hpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c DealingEngineParameters.cpp 

ActiveSet.o: ActiveSet.cpp ActiveSet.hpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c ActiveSet.cpp 

CreateOrder.o: RequestHandler.hpp CreateOrder.cpp 
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CreateOrder.cpp

UpdateOrder.o: RequestHandler.hpp UpdateOrder.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c UpdateOrder.cpp

CancelOrder.o: RequestHandler.hpp CancelOrder.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CancelOrder.cpp

ReleaseOrder.o: RequestHandler.hpp ReleaseOrder.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c ReleaseOrder.cpp

CreateRelease.o: RequestHandler.hpp CreateRelease.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CreateRelease.cpp

CancelRelease.o: RequestHandler.hpp CancelRelease.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CancelRelease.cpp

ReplaceRelease.o: RequestHandler.hpp ReplaceRelease.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c ReplaceRelease.cpp

ReleaseRejected.o: RequestHandler.hpp ReleaseRejected.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c ReleaseRejected.cpp

CancelRejected.o: RequestHandler.hpp CancelRejected.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CancelRejected.cpp

ReleaseRestated.o: RequestHandler.hpp ReleaseRestated.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c ReleaseRestated.cpp

ReleaseCanceled.o: RequestHandler.hpp ReleaseCanceled.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c ReleaseCanceled.cpp

ReleaseReplaced.o: RequestHandler.hpp ReleaseReplaced.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c ReleaseReplaced.cpp

CreateExecution.o: RequestHandler.hpp CreateExecution.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CreateExecution.cpp

ExecutionCanceled.o: RequestHandler.hpp ExecutionCanceled.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c ExecutionCanceled.cpp

QueryData.o: RequestHandler.hpp QueryData.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c QueryData.cpp

FlattenPositions.o: RequestHandler.hpp FlattenPositions.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c FlattenPositions.cpp

DealingStatus.o: RequestHandler.hpp DealingStatus.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c DealingStatus.cpp

FixEngineLogin.o: RequestHandler.hpp FixEngineLogin.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c FixEngineLogin.cpp

FixEngineLogout.o: RequestHandler.hpp FixEngineLogout.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c FixEngineLogout.cpp

MarketDataSubscribe.o: RequestHandler.hpp MarketDataSubscribe.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c MarketDataSubscribe.cpp

MarketDataUnsubscribe.o: RequestHandler.hpp MarketDataUnsubscribe.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c MarketDataUnsubscribe.cpp

LogoutFixEngine.o: RequestHandler.hpp LogoutFixEngine.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c LogoutFixEngine.cpp

LoginFixEngine.o: RequestHandler.hpp LoginFixEngine.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c LoginFixEngine.cpp

CreateDraftOrder.o: RequestHandler.hpp CreateDraftOrder.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CreateDraftOrder.cpp

ValidateRelease.o: RequestHandler.hpp ValidateRelease.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c ValidateRelease.cpp

ReloadStaticData.o: RequestHandler.hpp ReloadStaticData.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c ReloadStaticData.cpp

CreateAuditEvent.o: RequestHandler.hpp CreateAuditEvent.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c CreateAuditEvent.cpp

ExecuteSignal.o: RequestHandler.hpp ExecuteSignal.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c ExecuteSignal.cpp

ExecuteDraftOrder.o: RequestHandler.hpp ExecuteDraftOrder.cpp
	${COMPILER} ${CFLAGS} ${CINCLUDE} -c ExecuteDraftOrder.cpp

deploy_dev:
	cp DealingEngine ../../../../../release/dev/dealing_engine/bin
	cp DealingEngineTester ../../../../../release/dev/dealing_engine/bin

deploy_uat:
	cp DealingEngine ../../../../../release/uat/dealing_engine/bin

clean:
	rm -rf *.o DealingEngine DealingEngineTester
