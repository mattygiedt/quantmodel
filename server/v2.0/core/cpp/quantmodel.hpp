#ifndef QUANTMODEL_H
#define QUANTMODEL_H

/* QuantModel C++ Library Header */

#define VERSION_ID  "1.0.0.0"

#define MAX_DIGITS  32
#define GMP_FORMAT  "%.6Ff"
#define INT_FORMAT  "%.0Ff"

#define FORCE_CACHE_LINE_ALIGNMENT __attribute__ ((aligned (64)))

#define likely(x)   __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)

#include <google/protobuf/descriptor.h>

#include "quantmodel.pb.h"
#include "QuantModelColumns.hpp"
#include "QuantModelProcedures.hpp"
#include "xmlParser.hpp"
#include "ZMQMessage.hpp"
#include "TimeStamp.hpp"
#include "MemcachedConnection.hpp"

#include "DataObject.hpp"
//#include "User.hpp"
#include "SystemParameter.hpp"
#include "Account.hpp"
#include "AccountLimit.hpp"
#include "Instrument.hpp"
#include "Allocation.hpp"
#include "Order.hpp"
#include "Release.hpp"
#include "Execution.hpp"
#include "Strategy.hpp"
#include "InvestmentSystem.hpp"
#include "AccountStrategy.hpp"
#include "PositionIntraday.hpp"
#include "PositionOvernight.hpp"
#include "PositionManager.hpp"

#endif // QUANTMODEL_H
