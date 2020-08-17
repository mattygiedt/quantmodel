using System;
using System.IO;
using System.Windows;
using System.Security;
using System.Security.Permissions;
using System.Configuration;
using System.Windows.Forms;

namespace com.quantmodel.common.network.message
{
    public abstract class ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( ClientMessage ) );

        private static readonly DateTime EPOCH =
            new DateTime( 1970, 1, 1, 0, 0, 0, DateTimeKind.Utc );

        public abstract Google.ProtocolBuffers.IMessage ToMessage( string session_id );

        public static string getTimestamp( )
        {
            TimeSpan ts = ( DateTime.UtcNow - EPOCH );
            return ts.TotalSeconds.ToString();
        }
    }
}
