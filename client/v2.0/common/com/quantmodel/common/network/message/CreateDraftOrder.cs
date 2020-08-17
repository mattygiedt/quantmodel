using System;
using System.IO;
using System.Collections;

using quantmodel;
using com.quantmodel.common;

namespace com.quantmodel.common.network.message
{
    public sealed class CreateDraftOrder : ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( CreateDraftOrder ) );

        private readonly SignalData signal_data;

        public CreateDraftOrder( SignalData signal )
        {
            this.signal_data = signal;
        }

        public override Google.ProtocolBuffers.IMessage ToMessage( string session_id  )
        {
            string request_id = Guid.NewGuid().ToString();
            string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();

            return
                ClientAdapterRequest.CreateBuilder()
                    .SetRequestId( request_id )
                    .SetSessionId( session_id )
                    .SetTimestamp( timestamp )
                    .SetType( ClientAdapterRequest.Types.RequestType.DEALING )
                    .SetDealingRequest(
                       DealingEngineRequest.CreateBuilder()
                           .SetRequestId( request_id )
                           .SetTimestamp( timestamp )
                           .SetType( DealingEngineRequest.Types.RequestType.CREATE_DRAFT_ORDER )
                           .AddSignalData( signal_data ) )
                .Build();
        }
    }
}
