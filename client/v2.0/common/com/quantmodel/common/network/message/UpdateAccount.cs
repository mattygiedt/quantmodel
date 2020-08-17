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
    public sealed class UpdateAccount : ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( UpdateAccount ) );

        private readonly string account_id,
            name,
            market_account_id,
            giveup_account_id,
            giveup_firm_id,
            custody_account_id,
            notional_val,
            profit_target,
            stop_loss,
            signal_weight,
            is_suppressed,
            is_active;

        public UpdateAccount( string account_id,
                              string name,
                              string market_account_id,
                              string custody_account_id,
                              string giveup_account_id,
                              string giveup_firm_id,
                              string notional_val,
                              string profit_target,
                              string stop_loss,
                              string signal_weight,
                              string is_suppressed,
                              string is_active )
        {
            this.account_id = account_id;
            this.name = name;
            this.market_account_id = market_account_id;
            this.custody_account_id = custody_account_id;
            this.giveup_account_id = giveup_account_id;
            this.giveup_firm_id = giveup_firm_id;
            this.notional_val = notional_val;
            this.profit_target = profit_target;
            this.stop_loss = stop_loss;
            this.signal_weight = signal_weight;
            this.is_suppressed = is_suppressed;
            this.is_active = is_active;
        }

        public override Google.ProtocolBuffers.IMessage ToMessage( string session_id )
        {
            string updated_datetime = com.quantmodel.common.network.message.ClientMessage.getTimestamp();

            return QuantModelProcedures.UPDATE_ACCOUNT(
                session_id,
                account_id,
                name,
                market_account_id,
                custody_account_id,
                giveup_account_id,
                giveup_firm_id,
                notional_val,
                profit_target,
                stop_loss,
                signal_weight,
                is_suppressed,
                is_active,
                updated_datetime,
                "CLIENT" );
        }
    }
}