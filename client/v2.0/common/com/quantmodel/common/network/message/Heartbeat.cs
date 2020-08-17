using System;
using System.IO;
using System.Windows;
using System.Security;
using System.Security.Permissions;
using System.Configuration;
using System.Windows.Forms;

using quantmodel;

namespace com.quantmodel.common.network.message
{
    public sealed class Heartbeat : ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( Heartbeat ) );



        public override Google.ProtocolBuffers.IMessage ToMessage( string session_id  )
        {
            return
                ClientAdapterRequest.CreateBuilder()
                    .SetRequestId( Guid.NewGuid().ToString() )
                    .SetSessionId( session_id )
                    .SetTimestamp( getTimestamp() )
                    .SetType( ClientAdapterRequest.Types.RequestType.HEARTBEAT )
                .Build();
        }
    }
}