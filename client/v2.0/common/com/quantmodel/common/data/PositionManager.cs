using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Collections;

using quantmodel;
using com.quantmodel.common.events;

namespace com.quantmodel.common.data
{
    public sealed class Fill
    {
        public enum SideCode
        {
           BUY,
           SELL
        }

        private double price, quantity;
        private SideCode side;// = SideCode.BUY;

        public double Price { get { return price; } }
        public double Quantity { get { return quantity; } }
        public SideCode Side { get { return side; } }

        public Fill( string price, string quantity, string side )
        {
            this.price = System.Convert.ToDouble( price );
            this.quantity = System.Convert.ToDouble( quantity );
            this.side = System.Convert.ToInt32( side ) == 1 ? SideCode.BUY : SideCode.SELL;
        }
    }

    public sealed class InstrumentPosition
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( InstrumentPosition ) );

        public enum PositionExposure
        {
           LONG,
           SHORT,
           FLAT
        }

        private string investment_system_id;
        public string InvestmentSystemId { get { return investment_system_id; } }

        private string instrument_id;
        public string InstrumentId { get { return instrument_id; } }

        private PositionExposure exposure;// = PositionExposure.FLAT;
        private double open_pos, open_prc, closed_pos, closed_val, buy_qty, buy_val, buy_avg, sell_qty, sell_val, sell_avg, open_pl, closed_pl, total_pl, notional_pl;

        public PositionExposure Exposure { get { return exposure; } }
        public double OpenPosition { get { return open_pos; } }
        public double OpenPrice { get { return open_prc; } }
        public double ClosedPosition { get { return closed_pos; } }
        public double ClosedValue { get { return closed_val; } }
        public double LongPosition { get { if( exposure == PositionExposure.LONG ) return open_pos; else return 0.0; } }
        public double ShortPosition { get { if( exposure == PositionExposure.SHORT ) return -1*open_pos; else return 0.0; } }

        public double OpenPL { get { return open_pl; } }
        public double ClosedPL { get { return closed_pl; } }
        public double TotalPL { get { return total_pl; } }
        public double NotionalPL { get { return notional_pl; } }

        private readonly ArrayList fills = new ArrayList();

        public InstrumentPosition( string investment_system_id, string instrument_id )
        {
            this.investment_system_id = investment_system_id;
            this.instrument_id = instrument_id;
            Reset();
        }

        public void AddFill( Fill fill )
        {
            fills.Add( fill );
            Update( fill );
        }

        public void PairOff()
        {
            Reset();
            foreach( Fill fill in fills )
            {
                Update( fill );
            }
        }

        public void ProfitLoss( Account account, Instrument instrument )
        {
            double notional = Convert.ToDouble( account[ DatabaseObject.NOTIONAL_VAL ] );
            double last_prc = Convert.ToDouble( instrument[ DatabaseObject.LAST_PRC ] );
            double multiplier = Convert.ToDouble( instrument[ DatabaseObject.MULTIPLIER ] );

            //if( log.IsDebugEnabled )
            //{
            //    log.Debug( "PRE_PROFIT_LOSS" );
            //    log.Debug( " notional: " + notional );
            //    log.Debug( " last_prc: " + last_prc );
            //    log.Debug( " multiplier: " + multiplier );
            //    log.Debug( " exposure: " + exposure );
            //    log.Debug( " open_prc: " + open_prc );
            //    log.Debug( " open_pos: " + open_pos );
            //    log.Debug( " open_pl: " + open_pl );
            //    log.Debug( " closed_pl: " + closed_pl );
            //    log.Debug( " total_pl: " + total_pl );
            //    log.Debug( " notional_pl: " + notional_pl );
            //}

            if( exposure == PositionExposure.FLAT )
            {
                open_pl = 0.0;
            }
            else if( exposure == PositionExposure.LONG )
            {
                if( last_prc > 0.0 )
                {
                    open_pl = ( ( last_prc * open_pos ) - ( open_prc * open_pos ) ) * multiplier;
                }
                else
                {
                    open_pl = 0.0;
                }
            }
            else
            {
                if( last_prc > 0.0 )
                {
                    open_pl = ( ( open_prc * open_pos ) - ( last_prc * open_pos ) ) * multiplier;
                }
                else
                {
                    open_pl = 0.0;
                }
            }

            closed_pl = closed_val * multiplier;
            total_pl = open_pl + closed_pl;
            if( notional > 0 ) notional_pl = total_pl / notional;

            //if( log.IsDebugEnabled )
            //{
            //    log.Debug( "POST_PROFIT_LOSS" );
            //    log.Debug( " notional: " + notional );
            //    log.Debug( " last_prc: " + last_prc );
            //    log.Debug( " multiplier: " + multiplier );
            //    log.Debug( " exposure: " + exposure );
            //    log.Debug( " open_prc: " + open_prc );
            //    log.Debug( " open_pos: " + open_pos );
            //    log.Debug( " open_pl: " + open_pl );
            //    log.Debug( " closed_pl: " + closed_pl );
            //    log.Debug( " total_pl: " + total_pl );
            //    log.Debug( " notional_pl: " + notional_pl );
            //}
        }

        private void Reset()
        {
            exposure = PositionExposure.FLAT;
            open_pos = open_prc = closed_pos = closed_val = buy_qty =
            buy_val = sell_qty = sell_val = buy_avg = sell_avg =
            open_pl = closed_pl = total_pl = notional_pl = 0.0;
        }

        private void Update( Fill fill )
        {
            //if( log.IsDebugEnabled )
            //{
            //    log.Info( "PRE_UPDATE" );
            //    log.Info( " buy_qty: " + buy_qty );
            //    log.Info( " buy_val: " + buy_val );
            //    log.Info( " sel_qty: " + sell_qty );
            //    log.Info( " sel_val: " + sell_val );
            //    log.Info( " open_pos: " + open_pos );
            //    log.Info( " open_prc: " + open_prc );
            //    log.Info( " closed_pos: " + closed_pos );
            //    log.Info( " closed_val: " + closed_val );
            //}

            if( Fill.SideCode.BUY == fill.Side )
            {
                buy_qty += fill.Quantity;
                buy_val += fill.Quantity * fill.Price;
            }
            else
            {
                sell_qty += fill.Quantity;
                sell_val += fill.Quantity * fill.Price;
            }

            if( buy_qty == sell_qty )
            {
                exposure = PositionExposure.FLAT;
                closed_pos += buy_qty;
                closed_val += sell_val - buy_val;
                open_pos = open_prc = buy_qty = buy_val = sell_qty = sell_val = 0.0;
            }
            else if( buy_qty > sell_qty )
            {
                exposure = PositionExposure.LONG;

                if( buy_val > 0 && buy_qty > 0 ) buy_avg = buy_val / buy_qty;

                buy_qty -= sell_qty;
                buy_val = buy_qty * buy_avg;

                open_pos = buy_qty;
                open_prc = buy_avg;

                closed_pos += sell_qty;
                closed_val += sell_val - ( sell_qty * buy_avg );

                sell_qty = sell_val = 0.0;
            }
            else
            {
                exposure = PositionExposure.SHORT;

                if( sell_val > 0 && sell_qty > 0 ) sell_avg = sell_val / sell_qty;

                sell_qty -= buy_qty;
                sell_val = sell_qty * sell_avg;

                open_pos = sell_qty;
                open_prc = sell_avg;

                closed_pos += buy_qty;
                closed_val += ( buy_qty * sell_avg ) - buy_val;

                buy_qty = buy_val = 0.0;
            }

            //if( log.IsDebugEnabled )
            //{
            //    log.Info( "POST_UPDATE" );
            //    log.Info( " buy_qty: " + buy_qty );
            //    log.Info( " buy_val: " + buy_val );
            //    log.Info( " sel_qty: " + sell_qty );
            //    log.Info( " sel_val: " + sell_val );
            //    log.Info( " open_pos: " + open_pos );
            //    log.Info( " open_prc: " + open_prc );
            //    log.Info( " closed_pos: " + closed_pos );
            //    log.Info( " closed_val: " + closed_val );
            //}
        }
    }

    public sealed class InvestmentSystemPosition
    {
        private string account_id;
        public string AccountId { get { return account_id; } }

        private string investment_system_id;
        public string InvestmentSystemId { get { return investment_system_id; } }

        private readonly Hashtable instrument_positions = new Hashtable();

        public ArrayList InvestmentSystemPositions
        {
            get
            {
                ArrayList list = new ArrayList();
                foreach( DictionaryEntry position in instrument_positions ) list.Add( position.Value );
                return list;
            }
        }

        public InvestmentSystemPosition( string account_id, string investment_system_id )
        {
            this.account_id = account_id;
            this.investment_system_id = investment_system_id;
        }

        public InstrumentPosition CreateOrGet( string instrument_id )
        {
            if(! instrument_positions.ContainsKey( instrument_id ) )
            {
                instrument_positions.Add( instrument_id,
                    new InstrumentPosition( investment_system_id, instrument_id ) );
            }

            return (InstrumentPosition)instrument_positions[ instrument_id ];
        }
    }

    public sealed class AccountPosition
    {
        private string account_id;
        public string AccountId { get { return account_id; } }

        private readonly Hashtable investment_system_positions = new Hashtable();

        public ArrayList InstrumentPositions
        {
            get
            {
                ArrayList list = new ArrayList();

                foreach( InvestmentSystemPosition position in investment_system_positions.Values )
                    foreach( InstrumentPosition instrument_position in position.InvestmentSystemPositions )
                        list.Add( instrument_position );

                return list;
            }
        }

        public ArrayList AccountPositions
        {
            get
            {
                ArrayList list = new ArrayList();
                foreach( DictionaryEntry position in investment_system_positions ) list.Add( position.Value );
                return list;
            }
        }

        public AccountPosition( string account_id )
        {
            this.account_id = account_id;
        }

        public InvestmentSystemPosition CreateOrGet( string investment_system_id )
        {
            if(! investment_system_positions.ContainsKey( investment_system_id ) )
            {
                investment_system_positions.Add( investment_system_id,
                    new InvestmentSystemPosition( account_id, investment_system_id ) );
            }

            return (InvestmentSystemPosition)investment_system_positions[ investment_system_id ];
        }
    }

    public sealed class PositionManager
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( PositionManager ) );

        private readonly Hashtable account_positions = new Hashtable();
        private readonly EventDispatcher dispatcher;

        public ArrayList AccountPositions
        {
            get
            {
                ArrayList list = new ArrayList();
                foreach( DictionaryEntry position in account_positions ) list.Add( position.Value );
                return list;
            }
        }

        public PositionManager( EventDispatcher dispatcher )
        {
            this.dispatcher = dispatcher;

            dispatcher.Rollover += new EventHandler<DealingEventArgs>( FireRollover );
            dispatcher.ExecutionAdded += new EventHandler<DealingEventArgs>( FireExecutionAdded );
            dispatcher.InstrumentChanged += new EventHandler<MarketDataEventArgs>( FireInstrumentChanged );
        }

        private void FireInstrumentChanged( object sender, MarketDataEventArgs e )
		{
		    Instrument instrument = (Instrument)e.Data;

            //
            //  Find all account positions that have this instrument and fire
            //  position changed.
            //

            foreach( AccountPosition account_position in AccountPositions )
            {
                foreach( InstrumentPosition instrument_position in account_position.InstrumentPositions )
                {
                    if( instrument_position.InstrumentId.Equals( instrument.Id ) )
                    {
                        dispatcher.PositionChangedCallback(
                            new PositionEventArgs( account_position ) );

                        break;
                    }
                }
            }
		}

        private void FireExecutionAdded( object o, DealingEventArgs e )
        {
            Execution execution = (Execution)e.Data;
            Add( execution );
        }

        private void FireRollover( object o, DealingEventArgs e )
        {
            account_positions.Clear();
        }

        public AccountPosition CreateOrGet( string account_id )
        {
            if(! account_positions.ContainsKey( account_id ) )
            {
                account_positions.Add( account_id, new AccountPosition( account_id ) );
            }

            return (AccountPosition)account_positions[ account_id ];
        }

        public void Add( Execution execution )
        {
            if( execution.IsSpreadExecution )
            {
                return;
            }

            AccountPosition accountPosition;
            InvestmentSystemPosition investmentSystemPosition;
            InstrumentPosition instrumentPosition;

            accountPosition = CreateOrGet(
                execution[ DatabaseObject.ACCOUNT_ID ] );

            investmentSystemPosition =
                accountPosition.CreateOrGet(
                    execution[ DatabaseObject.INVESTMENT_SYSTEM_ID ] );

            instrumentPosition =
                investmentSystemPosition.CreateOrGet(
                    execution[ DatabaseObject.INSTRUMENT_ID ] );

            Fill fill = new Fill(
                execution[ DatabaseObject.EXECUTION_PRC ],
                execution[ DatabaseObject.EXECUTION_QTY ],
                execution[ DatabaseObject.SIDE_CODE ] );

            if( log.IsDebugEnabled )
            {
                log.Debug( "Intraday fill: pos " + execution[ DatabaseObject.EXECUTION_QTY ] + ", prc " +
                    execution[ DatabaseObject.EXECUTION_PRC ] + ", type " + execution[ DatabaseObject.SIDE_CODE ] );
            }

            instrumentPosition.AddFill( fill );

            dispatcher.PositionChangedCallback(
                new PositionEventArgs( accountPosition ) );

            if( (string)System.Configuration.ConfigurationManager.AppSettings[ "play_sound_on_fill" ] == "true" )
            {
                //
                //  Play a sound!!!
                //

                System.Media.SystemSounds.Asterisk.Play();
            }
        }

        public void Add( OvernightPosition position )
        {
            AccountPosition accountPosition;
            InvestmentSystemPosition investmentSystemPosition;
            InstrumentPosition instrumentPosition;

            accountPosition = CreateOrGet(
                position[ DatabaseObject.ACCOUNT_ID ] );

            investmentSystemPosition =
                accountPosition.CreateOrGet(
                    position[ DatabaseObject.INVESTMENT_SYSTEM_ID ] );

            instrumentPosition =
                investmentSystemPosition.CreateOrGet(
                    position[ DatabaseObject.INSTRUMENT_ID ] );

            Fill fill = new Fill(
                position[ DatabaseObject.OPEN_PRC ],
                position[ DatabaseObject.OPEN_POS ],
                position[ DatabaseObject.POSITION_TYPE ] );

            if( log.IsDebugEnabled )
            {
                log.Debug( "Overnight fill: pos " + position[ DatabaseObject.OPEN_POS ] + ", prc " +
                    position[ DatabaseObject.OPEN_PRC ] + ", type " + position[ DatabaseObject.POSITION_TYPE ] );
            }

            instrumentPosition.AddFill( fill );

            dispatcher.PositionChangedCallback(
                new PositionEventArgs( accountPosition ) );
        }
    }
}
