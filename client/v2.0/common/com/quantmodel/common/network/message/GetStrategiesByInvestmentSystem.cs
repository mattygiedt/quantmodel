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
    public sealed class GetStrategiesByInvestmentSystem : ClientMessage
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( GetStrategiesByInvestmentSystem ) );

        private readonly string investment_system_id;

        public GetStrategiesByInvestmentSystem( string investment_system_id )
        {
            this.investment_system_id = investment_system_id;
        }

        public override Google.ProtocolBuffers.IMessage ToMessage( string session_id  )
        {
            return QuantModelProcedures.GET_STRATEGIES_BY_INVESTMENT_SYSTEM( session_id, investment_system_id );
        }
    }
}