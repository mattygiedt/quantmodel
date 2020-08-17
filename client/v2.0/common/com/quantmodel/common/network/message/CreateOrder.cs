using System;
using System.IO;
using System.Collections;

using quantmodel;
using com.quantmodel.common;

namespace com.quantmodel.common.network.message
{
    public sealed class CreateOrder : ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( CreateOrder ) );

        private readonly DraftOrderMessage draft_order_message;

        public CreateOrder( DraftOrderMessage draft_order )
        {
            draft_order_message = draft_order;
        }

        public CreateOrder( DraftOrderData order, DraftAllocationData allocation )
        {
            draft_order_message = DraftOrderMessage.CreateBuilder()
                .SetDraftOrderData( order )
                .AddDraftAllocationData( allocation )
            .Build();
        }

        public CreateOrder( DraftOrderData order, ArrayList allocations )
        {
            DraftOrderMessage.Builder builder = DraftOrderMessage.CreateBuilder();

            builder.SetDraftOrderData( order );

            foreach( DraftAllocationData allocation in allocations )
            {
                builder.AddDraftAllocationData( allocation );
            }

            draft_order_message = builder.Build();
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
                           .SetType( DealingEngineRequest.Types.RequestType.CREATE_ORDER )
                           .SetDraftOrder( draft_order_message ) )
                .Build();
        }
    }
}