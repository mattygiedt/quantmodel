using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Collections;
using System.Globalization;
using System.Configuration;

namespace com.quantmodel.common.data
{
    public abstract class DatabaseObject
    {
        private static readonly DateTime EPOCH =
            new DateTime( 1970, 1, 1, 0, 0, 0, DateTimeKind.Utc );

        protected Hashtable fields = new Hashtable();
        protected string id = "";

        public string Id { get { return id; } }
        public string this [ object key ]
        {
            get
            {
                string str = fields.ContainsKey( key ) ?
                    (string)fields[ key ] : "";
                return str;
            }
        }

        public void SetAttribute( string key, string value )
        {
            fields[ key ] = value;
        }

        public double ToDouble( string key )
        {
            double d = 0.0D;
            try { d = Convert.ToDouble( this[ key ] ); }
            catch( Exception ) { d = 0.0D; }
            return d;
        }

        public double ToLong( string key )
        {
            long l = 0L;
            try { l = Convert.ToInt64( this[ key ] ); }
            catch( Exception ) { l = 0L; }
            return l;
        }

        public int ToInt32( string key )
        {
            int i = 0;
            try { i = Convert.ToInt32( this[ key ] ); }
            catch( Exception ) { i = 0; }
            return i;
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            foreach( DictionaryEntry de in fields )
            {
                sb.Append( de.Key )
                  .Append( " --> " )
                  .Append( de.Value )
                  .Append( "\n" );
            }

            return sb.ToString();
        }

        public void ToXML( XmlElement node )
        {
            foreach( DictionaryEntry de in fields )
            {
                node.SetAttribute(
                    (string)de.Key,
                    (string)de.Value );
            }
        }

        public string ToLocalTime( string key )
        {
            //
            //  Convert from 1303304682.369954 UTC into Local Time
            //

            try
            {
                TimeSpan ts = TimeSpan.FromSeconds( Convert.ToDouble( this[ key ] ) );
                DateTime dateValue = EPOCH + ts;

                return dateValue.ToLocalTime().ToString(
                    (string)ConfigurationManager.AppSettings[ "datetime_format" ] );
            }
            catch( Exception )
            {
                return this[ key ];
            }
        }

        public string OrderType( string name )
        {
            return Convert.ToInt32( Enum.Parse( typeof( EnumData.OrderType ), name ) ).ToString();
        }

        public string OrderType( )
        {
            return Enum.GetName( typeof( EnumData.OrderType ), ToInt32( ORDER_TYPE ) );
        }

        public string Tif( )
        {
            return Enum.GetName( typeof( EnumData.TimeInForce ), ToInt32( TIF ) );
        }

        public static string Tif( string tif )
        {
            return Enum.GetName( typeof( EnumData.TimeInForce ), Convert.ToInt32( tif ) );
        }

        public string SideCode( )
        {
            return Enum.GetName( typeof( EnumData.SideCode ), ToInt32( SIDE_CODE ) );
        }

        public static string SideCode( string side_code )
        {
            return Enum.GetName( typeof( EnumData.SideCode ), Convert.ToInt32( side_code ) );
        }

        public string OrderStatus( )
        {
            return Enum.GetName( typeof( EnumData.OrderStatus ), ToInt32( ORDER_STATUS ) );
        }

        public string ReleaseType( )
        {
            return Enum.GetName( typeof( EnumData.ReleaseType ), ToInt32( RELEASE_TYPE ) );
        }

        public string ReleaseStatus( )
        {
            return Enum.GetName( typeof( EnumData.ReleaseStatus ), ToInt32( RELEASE_STATUS ) );
        }

        public string ExecutionStatus( )
        {
            return Enum.GetName( typeof( EnumData.ExecutionStatus ), ToInt32( EXECUTION_STATUS ) );
        }

        public static readonly string ACCOUNT_ID = "account_id";
        public static readonly string ADDED_BY = "added_by";
        public static readonly string ADDED_DATETIME = "added_datetime";
        public static readonly string ALLOCATION_ID = "allocation_id";
        public static readonly string ALLOCATION_QTY = "allocation_qty";
        public static readonly string APPLICATION_ID = "application_id";
        public static readonly string AVERAGE_PRC = "average_prc";
        public static readonly string BASE_CCY = "base_ccy";
        public static readonly string CLIENT_ORDER_ID = "client_order_id";
        public static readonly string CLOSE_EXECUTION_ID = "close_execution_id";
        public static readonly string CLOSE_PRC = "close_prc";
        public static readonly string CLOSED_PRC = "closed_prc";
        public static readonly string CLOSED_VAL = "closed_val";
        public static readonly string COUNTERPARTY_EXECUTION_ID = "counterparty_execution_id";
        public static readonly string COUNTERPARTY_ID = "counterparty_id";
        public static readonly string COUNTERPARTY_ORDER_ID = "counterparty_order_id";
        public static readonly string CUSIP = "cusip";
        public static readonly string CUSTODY_ACCOUNT_ID = "custody_account_id";
        public static readonly string DEFAULT_WEIGHT = "default_weight";
        public static readonly string ENTRY_PRC = "entry_prc";
        public static readonly string ENTRY_VAL = "entry_val";
        public static readonly string EXECUTED_QTY = "executed_qty";
        public static readonly string EXECUTED_VAL = "executed_val";
        public static readonly string EXECUTING_COUNTERPARTY_ID = "executing_counterparty_id";
        public static readonly string EXECUTION_CCY = "execution_ccy";
        public static readonly string EXECUTION_ID = "execution_id";
        public static readonly string EXECUTION_PRC = "execution_prc";
        public static readonly string EXECUTION_QTY = "execution_qty";
        public static readonly string EXECUTION_STATUS = "execution_status";
        public static readonly string EXECUTION_VAL = "execution_val";
        public static readonly string EXTERNAL_ORDER_ID = "external_order_id";
        public static readonly string FIRSTNAME = "firstname";
        public static readonly string FIX_ENGINE_ID = "fix_engine_id";
        public static readonly string GIVEUP_ACCOUNT_ID = "giveup_account_id";
        public static readonly string GIVEUP_FIRM_ID = "giveup_firm_id";
        public static readonly string INSTRUMENT_ID = "instrument_id";
        public static readonly string INSTRUMENT_ID_TYPE_ID = "instrument_id_type_id";
        public static readonly string INSTRUMENT_TYPE = "instrument_type";
        public static readonly string INVESTMENT_SYSTEM_ID = "investment_system_id";
        public static readonly string IS_ACTIVE = "is_active";
        public static readonly string IS_ENABLED = "is_enabled";
        public static readonly string IS_OPEN = "is_open";
        public static readonly string IS_SUPPRESSED = "is_suppressed";
        public static readonly string ISIN = "isin";
        public static readonly string LASTNAME = "lastname";
        public static readonly string LAST_PRC = "last_prc";
        public static readonly string LEAVES_QTY = "leaves_qty";
        public static readonly string LEG_TYPE = "leg_type";
        public static readonly string LIMIT_PRC = "limit_prc";
        public static readonly string LOT_ID = "lot_id";
        public static readonly string LOT_QTY = "lot_qty";
        public static readonly string MARKET_ACCOUNT_ID = "market_account_id";
        public static readonly string MATURITY_MONTH_YEAR = "maturity_month_year";
        public static readonly string MAX_OPEN_LONG_QTY = "max_open_long_qty";
        public static readonly string MAX_OPEN_SHORT_QTY = "max_open_short_qty";
        public static readonly string MAX_RELEASE_QTY = "max_release_qty";
        public static readonly string MULTIPLIER = "multiplier";
        public static readonly string NAME = "name";
        public static readonly string NOTIONAL_VAL = "notional_val";
        public static readonly string OPEN_EXECUTION_ID = "open_execution_id";
        public static readonly string OPEN_POS = "open_pos";
        public static readonly string OPEN_PRC = "open_prc";
        public static readonly string OPEN_QTY = "open_qty";
        public static readonly string ORDER_ID = "order_id";
        public static readonly string ORDER_QTY = "order_qty";
        public static readonly string ORDER_STATUS = "order_status";
        public static readonly string ORDER_TYPE = "order_type";
        public static readonly string PARENT_ORDER_ID = "parent_order_id";
        public static readonly string PASSWORD = "password";
        public static readonly string POSITION_OVERNIGHT_ID = "position_overnight_id";
        public static readonly string POSITION_TYPE = "position_type";
        public static readonly string PROFIT_TARGET = "profit_target";
        public static readonly string RELEASE_ID = "release_id";
        public static readonly string RELEASE_QTY = "release_qty";
        public static readonly string RELEASE_STATUS = "release_status";
        public static readonly string RELEASE_TYPE = "release_type";
        public static readonly string RELEASED_QTY = "released_qty";
        public static readonly string RELEASED_VAL = "released_val";
        public static readonly string RIC = "ric";
        public static readonly string SECURITY_EXCHANGE = "security_exchange";
        public static readonly string SECURITY_ID = "security_id";
        public static readonly string SECURITY_TYPE = "security_type";
        public static readonly string SEDOL = "sedol";
        public static readonly string SESSION_ID = "session_id";
        public static readonly string SIDE_CODE = "side_code";
        public static readonly string SIGNAL_ID = "signal_id";
        public static readonly string SIGNAL_WEIGHT = "signal_weight";
        public static readonly string STOP_LOSS = "stop_loss";
        public static readonly string STOP_PRC = "stop_prc";
        public static readonly string STRATEGY_ID = "strategy_id";
        public static readonly string SYMBOL = "symbol";
        public static readonly string TIF = "tif";
        public static readonly string UPDATED_BY = "updated_by";
        public static readonly string UPDATED_DATETIME = "updated_datetime";
        public static readonly string USER_ID = "user_id";
        public static readonly string USERNAME = "username";
        public static readonly string VALUE = "value";
        public static readonly string WEIGHT = "weight";
	}
}
