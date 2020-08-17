using System;
using System.IO;
using System.Collections;
using System.Globalization;

using quantmodel;
using com.quantmodel.common;

namespace com.quantmodel.common.network.message
{
    public sealed class CreateTradeBlotter : ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( CreateTradeBlotter ) );
        
		private readonly DateTime EPOCH = new DateTime( 1970, 1, 1, 0, 0, 0, DateTimeKind.Utc );
		
        private double begDate;
		private double endDate;
		
        public CreateTradeBlotter( DateTime date )
        {
            begDate = ( date.ToUniversalTime() - EPOCH ).TotalSeconds;
			endDate = ( date.ToUniversalTime().AddDays( 1 ) - EPOCH ).TotalSeconds;
        }

        public override Google.ProtocolBuffers.IMessage ToMessage( string session_id  )
        {
            string request_id = Guid.NewGuid().ToString();
            string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();
            
            string sql = "select execution_id, instrument_id, counterparty_execution_id, side_code, execution_prc, execution_qty, added_datetime from executions where account_id = 10 and added_datetime > " + begDate + " and added_datetime < " + endDate + ";";

            return
                ClientAdapterRequest.CreateBuilder()
                    .SetRequestId( request_id )
                    .SetSessionId( session_id )
                    .SetTimestamp( timestamp )
                    .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )
                    .SetDatabaseRequest(
                       DatabaseEngineRequest.CreateBuilder()
                           .SetRequestId( request_id )
                           .SetTimestamp( timestamp )
                           .AddQuery(
                               DatabaseEngineRequest.Types.Query.CreateBuilder()
                                   .SetQueryId( "GET_TRADE_BLOTTER_DATA" )
                                   .SetQueryStr( sql )
                                   .SetType( DatabaseEngineRequest.Types.QueryType.SELECT ) ) )
                   .Build();
        }
    }
}

