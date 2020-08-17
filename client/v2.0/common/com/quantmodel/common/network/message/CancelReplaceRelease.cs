using System;
using System.IO;
using System.Windows;
using System.Security;
using System.Security.Permissions;
using System.Configuration;
using System.Windows.Forms;

using quantmodel;
using com.quantmodel.common;

namespace com.quantmodel.common.network.message
{
    public sealed class CancelReplaceRelease : ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( CancelReplaceRelease ) );

        private readonly string release_id;
        private readonly string stop_prc;
        private readonly string limit_prc;

        public CancelReplaceRelease( string release_id, string stop_prc, string limit_prc  )
        {
            this.release_id = release_id;
            this.stop_prc = stop_prc;
            this.limit_prc = limit_prc;
        }

        public override Google.ProtocolBuffers.IMessage ToMessage( string session_id )
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
                           .SetType( DealingEngineRequest.Types.RequestType.REPLACE_RELEASE )
                           .SetOrder( OrderMessage.CreateBuilder()
                                .AddReleaseData( ReleaseData.CreateBuilder()
                                    .SetReleaseId( release_id )
                                    .SetStopPrc( stop_prc )
                                    .SetLimitPrc( limit_prc ) ) ) )
                .Build();
        }
    }
}