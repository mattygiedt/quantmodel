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
    public sealed class GetAccountLimitsByAccountId : ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( GetAccountLimitsByAccountId ) );

        private readonly string account_id;

        public GetAccountLimitsByAccountId( string account_id )
        {
            this.account_id = account_id;
        }

        public override Google.ProtocolBuffers.IMessage ToMessage( string session_id  )
        {
            return QuantModelProcedures.GET_ACCOUNT_LIMITS_BY_ACCOUNT_ID( session_id, account_id );
        }
    }
}