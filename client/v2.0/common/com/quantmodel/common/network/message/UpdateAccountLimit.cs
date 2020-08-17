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
    public sealed class UpdateAccountLimit : ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( UpdateAccountLimit ) );

        private readonly string account_id,
            symbol,
            security_exchange,
            fix_session_id,
            max_open_long_qty,
            max_open_short_qty,
            max_release_qty,
            is_active;

        public UpdateAccountLimit( string account_id, string symbol, string security_exchange, string fix_session_id,
            string max_open_long_qty, string max_open_short_qty, string max_release_qty, string is_active )
        {
            this.account_id = account_id;
            this.symbol = symbol;
            this.security_exchange = security_exchange;
            this.fix_session_id = fix_session_id;
            this.max_open_long_qty = max_open_long_qty;
            this.max_open_short_qty = max_open_short_qty;
            this.max_release_qty = max_release_qty;
            this.is_active = is_active;
        }

        public override Google.ProtocolBuffers.IMessage ToMessage( string session_id )
        {
            string updated_datetime = com.quantmodel.common.network.message.ClientMessage.getTimestamp();

            return QuantModelProcedures.UPDATE_ACCOUNT_LIMIT(
                session_id, account_id, symbol, security_exchange, fix_session_id,
                max_open_long_qty, max_open_short_qty, max_release_qty,
                is_active, updated_datetime, "CLIENT" );
        }
    }
}