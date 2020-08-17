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
    public sealed class Login : ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( Login ) );

        private readonly string username;
        private readonly string password;
        private readonly string application;

        public Login( string username, string password )
        {
            this.username = username;
            this.password = password;
            this.application = "CLIENT";
        }

        public Login( string username, string password, string application )
        {
            this.username = username;
            this.password = password;
            this.application = application;
        }

        public override Google.ProtocolBuffers.IMessage ToMessage( string session_id  )
        {
            return QuantModelProcedures.USER_LOGIN( session_id, username, password, application, session_id );
        }
    }
}