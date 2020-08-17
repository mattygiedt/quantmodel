using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Collections;

namespace com.quantmodel.common.data
{
    public class EnumData
    {
        public enum SideCode {
            BUY = 1,
            SELL,
            BUY_MINUS,
            SELL_PLUS,
            SELL_SHORT,
            SELL_SHORT_EXEMPT,
            UNDISCLOSED,
            CROSS,
            CROSS_SHORT
        }

        public enum TimeInForce {
            DAY = 0,    //0: Day
            GTC = 1,    //1: Good Till Cancel (GTC)
            OPG = 2,    //2: At The Opening (OPG)
            IOC = 3,    //3: Immediate or Cancel (IOC)
            FOK = 4,    //4: Fill Or Kill (FOK)
            GTD = 6,    //6: Good Till Date (GTD)
            GIS = 8     //8: Good in Session (GIS)
        }

        public enum OrderStatus {
            SUPPRESSED = 0,
            NEW,
            PENDING_RELEASE,
            RELEASED,
            PARTIALLY_FILLED,
            FILLED,
            CANCELED,
            CLOSED,
            REJECTED
        }

        public enum OrderType {
            MARKET = 1,
            LIMIT,
            STOP,
            STOP_LIMIT,
            MARKET_ON_CLOSE,
            MARKET_LIMIT
        }

        public enum ReleaseType {
            MARKET = 1,
            LIMIT,
            STOP,
            STOP_LIMIT,
            MARKET_ON_CLOSE
        }

        public enum ReleaseStatus {
            PENDING_NEW = 0,
            NEW,
            PARTIALLY_FILLED,
            FILLED,
            DONE_FOR_DAY,
            CANCELED,
            REPLACED,
            PENDING_CANCEL,
            PENDING_REPLACE,
            STOPPED,
            REJECTED,
            SUSPENDED,
            CLOSED,
            COMPLIANCE_REJECT
        }

        public enum ExecutionStatus {
            NEW = 0,
            UNALLOCATED,
            ALLOCATED,
            CANCELED,
            CLOSED
        }
	}
}
