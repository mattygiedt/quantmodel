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
    public sealed class CreateAccount : ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( CreateAccount ) );

        private readonly string name,
            market_account_id,
            custody_account_id,
            notional_val,
            profit_target,
            stop_loss,
            signal_weight,
            is_active;

        public CreateAccount(
            string name,
            string market_account_id,
            string custody_account_id,
            string notional_val,
            string profit_target,
            string stop_loss,
            string signal_weight,
            string is_active )
        {
            this.name = name;
            this.market_account_id = market_account_id;
            this.custody_account_id = custody_account_id;
            this.notional_val = notional_val;
            this.profit_target = profit_target;
            this.stop_loss = stop_loss;
            this.signal_weight = signal_weight;
            this.is_active = is_active;
        }

        public override Google.ProtocolBuffers.IMessage ToMessage( string session_id )
        {
            string added_datetime = com.quantmodel.common.network.message.ClientMessage.getTimestamp();

            return QuantModelProcedures.INSERT_ACCOUNT(
                session_id,
                name,
                market_account_id,
                custody_account_id,
                notional_val,
                profit_target,
                stop_loss,
                signal_weight,
                is_active,
                added_datetime,
                "CLIENT" );
        }
    }
}

                                        