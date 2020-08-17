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
    public sealed class GetUserInvestmentSystems : ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( GetUserInvestmentSystems ) );

        private readonly string user_id;

        public GetUserInvestmentSystems( string user_id )
        {
            this.user_id = user_id;
        }

        public override Google.ProtocolBuffers.IMessage ToMessage( string session_id  )
        {
            return QuantModelProcedures.GET_INVESTMENT_SYSTEMS_BY_USER( session_id, user_id );
        }
    }
}