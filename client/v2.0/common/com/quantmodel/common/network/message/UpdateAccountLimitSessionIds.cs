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
    public sealed class UpdateAccountLimitSessionIds : ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( UpdateAccountLimitSessionIds ) );

        private readonly string account_id, fix_session_id;

        public UpdateAccountLimitSessionIds( string account_id, string fix_session_id )
        {
            this.account_id = account_id;
            this.fix_session_id = fix_session_id;
        }

        public override Google.ProtocolBuffers.IMessage ToMessage( string session_id )
        {
            string updated_datetime = com.quantmodel.common.network.message.ClientMessage.getTimestamp();

            return QuantModelProcedures.UPDATE_ACCOUNT_LIMIT_SESSION_IDS(
                session_id,
                account_id,
                fix_session_id,
                updated_datetime,
                "CLIENT" );
        }
    }
}