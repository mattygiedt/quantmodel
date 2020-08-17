using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Collections;

namespace com.quantmodel.common.data
{
    public class ProfitLossData
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( ProfitLossData ) );

        public string AccountId { get { return account_id; } }
        public string InstrumentId { get { return instrument_id; } }
        public double LastPx { get { return last_px; } }
        public double OpenPos { get { return open_pos; } }
        public double ClosedPos { get { return closed_pos; } }
        public double TotalPos { get { return open_pos + closed_pos; } }
        public double OpenPL { get { return open_pl; } }
        public double ClosedPL { get { return closed_pl; } }
        public double TotalPL { get { return OpenPL + ClosedPL; } }

        public double AvgOpenPrc {
            get
            {
                if( open_pos == 0 )
                {
                    return 0.0D;
                }
                else
                {
                    return open_val / Math.Abs( open_pos );
                }
            }
        }

        public double LongPos { get { return open_pos > 0 ? open_pos : 0.0D; } }
        public double ShortPos { get { return open_pos < 0 ? open_pos : 0.0D; } }

        private double last_px;
        private double open_pos;
        private double open_val;
        private double closed_pos;
        private double closed_val;
        private double open_pl;
        private double closed_pl;
        private double multiplier;
        private string account_id;
        private string instrument_id;

        public ProfitLossData( string account_id )
        {
            open_pos =
            open_val =
            closed_pos =
            closed_val =
            open_pl =
            closed_pl =
            last_px =
            multiplier = 0.0D;

            instrument_id = "";
            this.account_id = account_id;
        }

        public ProfitLossData( string account_id, XmlNode node, double last_prc, double mul )
        {
            this.account_id = account_id;
            instrument_id = node.Attributes[ "instrument_id" ].Value;
            multiplier = mul;
            last_px = last_prc;

            open_pos = Convert.ToDouble( node.Attributes[ "open_pos" ].Value );
            open_val = Math.Abs( Convert.ToDouble( node.Attributes[ "open_val" ].Value ) );

            closed_pos = Convert.ToDouble( node.Attributes[ "closed_pos" ].Value );
            closed_val = Convert.ToDouble( node.Attributes[ "closed_val" ].Value );

            if( open_pos == 0 )
            {
                open_pl = 0.0D;
            }
            else if( open_pos > 0 )
            {
                open_pl = ( ( last_px * open_pos ) - open_val ) * multiplier;
            }
            else
            {
                open_pl = ( open_val + ( last_px * open_pos ) ) * multiplier;
            }

            closed_pl = closed_val * multiplier;

            if( log.IsDebugEnabled )
            {
                log.Debug( " plData: " + node.OuterXml );
                log.Debug( "  open_pos: " + open_pos );
                log.Debug( "  open_val: " + open_val );
                log.Debug( "  closed_pos: " + closed_pos );
                log.Debug( "  closed_val: " + closed_val );
                log.Debug( "  last_px: " + last_px );
                log.Debug( "  multipier: " + multiplier );
                log.Debug( "  closed_pl: " + closed_pl );

                if( open_pos == 0 )
                {
                    log.Debug( "  open_pl: " + open_pl );
                }
                else if( open_pos > 0 )
                {
                    log.Debug( "  open_pl: ( ( " + last_prc + " * " + open_pos + " ) - " + open_val + " ) * " + multiplier );
                    log.Debug( "  open_pl: " + open_pl );
                }
                else
                {
                    log.Debug( "  open_pl: ( " + open_val + " + ( " + last_prc + " * " + open_pos + " ) ) * " + multiplier );
                    log.Debug( "  open_pl: " + open_pl );
                }
            }
        }

        public void Update( double last_prc )
        {
            last_px = last_prc;

            if( open_pos == 0 )
            {
                //
                //  Don't do anything.
                //
            }
            else if( open_pos > 0 )
            {
                open_pl = ( ( last_px * open_pos ) - open_val ) * multiplier;
            }
            else
            {
                open_pl = ( open_val + ( last_px * open_pos ) ) * multiplier;
            }
        }

        public void Aggregate( ProfitLossData data )
        {
            open_pl = open_pl + data.OpenPL;
            //open_pos = open_pos + Math.Abs( data.OpenPos );
            open_pos = open_pos + data.OpenPos;
            closed_pl = closed_pl + data.ClosedPL;
            closed_pos = closed_pos + data.ClosedPos;
        }
	}
}
