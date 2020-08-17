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
    public sealed class GetActiveInstrumentsBySymbol : ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( GetActiveInstrumentsBySymbol ) );

        private readonly string symbol;

        public GetActiveInstrumentsBySymbol( string symbol )
        {
            this.symbol = symbol;
        }

        public override Google.ProtocolBuffers.IMessage ToMessage( string session_id  )
        {
            return QuantModelProcedures.GET_ACTIVE_INSTRUMENTS_BY_SYMBOL( session_id, symbol );
        }
    }
}