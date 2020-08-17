using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Collections;

using quantmodel;
using com.quantmodel.common.events;

namespace com.quantmodel.common.data
{
    public sealed class ActiveSet
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( ActiveSet ) );

        private readonly Hashtable orders = new Hashtable();
        private readonly Hashtable accounts = new Hashtable();
        private readonly Hashtable releases = new Hashtable();
        private readonly Hashtable executions = new Hashtable();
        private readonly Hashtable instruments = new Hashtable();
        private readonly Hashtable investment_systems = new Hashtable();

        private readonly EventDispatcher dispatcher;

        public ActiveSet( EventDispatcher dispatcher )
        {
            this.dispatcher = dispatcher;
            this.dispatcher.OnBroadcast +=
                new EventHandler<BroadcastEventArgs>( FireBroadcast );
        }

        public void ClearStaticData()
        {
            accounts.Clear();
            instruments.Clear();
            investment_systems.Clear();
        }

        //
        //  Static Data
        //

        public bool HasAccount( string id )
        {
            return accounts.ContainsKey( id );
        }

        public Account GetAccount( string id )
        {
            return (Account)accounts[ id ];
        }

        public ArrayList Accounts
        {
            get
            {
                ArrayList list = new ArrayList();
                foreach( DictionaryEntry account in accounts ) list.Add( account.Value );
                list.Sort();
                return list;
            }
        }

        public bool HasInstrument( string id )
        {
            return instruments.ContainsKey( id );
        }

        public Instrument GetInstrument( string id )
        {
            return (Instrument)instruments[ id ];
        }

        public ArrayList Instruments
        {
            get
            {
                ArrayList list = new ArrayList();
                foreach( DictionaryEntry instrument in instruments ) list.Add( instrument.Value );
                list.Sort();
                return list;
            }
        }

        public bool HasInvestmentSystem( string id )
        {
            return "CLIENT".Equals( id ) || investment_systems.ContainsKey( id );
        }

        public InvestmentSystem GetInvestmentSystem( string id )
        {
            return (InvestmentSystem)investment_systems[ id ];
        }

        public ArrayList InvestmentSystems
        {
            get
            {
                ArrayList list = new ArrayList();
                foreach( DictionaryEntry investment_system in investment_systems ) list.Add( investment_system.Value );
                list.Sort();
                return list;
            }
        }

        //
        //  Dealing Objects
        //

        public bool HasOrder( object id )
        {
            return orders.ContainsKey( id );
        }

        public Order GetOrder( object id )
        {
            return (Order)orders[ id ];
        }

        public ArrayList Orders
        {
            get
            {
                ArrayList list = new ArrayList();
                foreach( DictionaryEntry order in orders ) list.Add( order.Value );
                return list;
            }
        }

        public bool HasRelease( object id )
        {
            return releases.ContainsKey( id );
        }

        public Release GetRelease( object id )
        {
            return (Release)releases[ id ];
        }

        public ArrayList Releases
        {
            get
            {
                ArrayList list = new ArrayList();
                foreach( DictionaryEntry release in releases ) list.Add( release.Value );
                return list;
            }
        }

        public bool HasExecution( object id )
        {
            return executions.ContainsKey( id );
        }

        public Execution GetExecution( object id )
        {
            return (Execution)executions[ id ];
        }

        public ArrayList Executions
        {
            get
            {
                ArrayList list = new ArrayList();
                foreach( DictionaryEntry execution in executions ) list.Add( execution.Value );
                return list;
            }
        }

        public void Update( Instrument instrument )
        {
            if( instruments.ContainsKey( instrument.Id ) )
            {
                instruments.Remove( instrument.Id );
                instruments.Add( instrument.Id, instrument );
            }
            else
            {
                instruments.Add( instrument.Id, instrument );
            }
        }

        public void Update( Account account )
        {
            if( accounts.ContainsKey( account.Id ) )
            {
                accounts.Remove( account.Id );
                accounts.Add( account.Id, account );
            }
            else
            {
                accounts.Add( account.Id, account );
            }
        }

        public void Update( InvestmentSystem investment_system )
        {
            if( investment_systems.ContainsKey( investment_system.Id ) )
            {
                investment_systems.Remove( investment_system.Id );
                investment_systems.Add( investment_system.Id, investment_system );
            }
            else
            {
                investment_systems.Add( investment_system.Id, investment_system );
            }
        }

        public void Update( Order order )
        {
            if( orders.ContainsKey( order.Id ) )
            {
                Order oldOrder = (Order)orders[ order.Id ];
                orders.Remove( order.Id );
                orders.Add( order.Id, order );
                dispatcher.OrderChangedCallback(
                    new DealingEventArgs( order, oldOrder ) );
            }
            else if( HasInvestmentSystem( order[ DatabaseObject.INVESTMENT_SYSTEM_ID ] ) )
            {
                orders.Add( order.Id, order );
                dispatcher.OrderAddedCallback(
                    new DealingEventArgs( order ) );
            }
        }

        public void Update( Release release )
        {
            if( releases.ContainsKey( release.Id ) )
            {
                Release oldRelease = (Release)releases[ release.Id ];
                releases.Remove( release.Id );
                releases.Add( release.Id, release );
                dispatcher.ReleaseChangedCallback(
                    new DealingEventArgs( release, oldRelease ) );
            }
            else if( HasOrder( release[ DatabaseObject.ORDER_ID ] ) )
            {
                releases.Add( release.Id, release );
                dispatcher.ReleaseAddedCallback(
                    new DealingEventArgs( release ) );
            }
        }

        public void Update( Execution execution )
        {
            if( executions.ContainsKey( execution.Id ) )
            {
                Execution oldExecution = (Execution)executions[ execution.Id ];
                executions.Remove( execution.Id );
                executions.Add( execution.Id, execution );
                dispatcher.ExecutionChangedCallback(
                    new DealingEventArgs( execution, oldExecution ) );
            }
            else if( HasOrder( execution[ DatabaseObject.ORDER_ID ] ) )
            {
                executions.Add( execution.Id, execution );
                dispatcher.ExecutionAddedCallback(
                    new DealingEventArgs( execution ) );
            }
        }

        private void FireBroadcast( object sender, BroadcastEventArgs e )
		{
            ClientAdapterBroadcast broadcast = e.Data;

            if( broadcast.Type == ClientAdapterBroadcast.Types.BroadcastType.DEALING )
            {
                DealingEngineBroadcast dealingBroadcast = broadcast.DealingBroadcast;

                if( dealingBroadcast.Type == DealingEngineBroadcast.Types.BroadcastType.ROLLOVER )
                {
                    orders.Clear();
                    releases.Clear();
                    executions.Clear();

                    dispatcher.RolloverCallback( new DealingEventArgs( "ROLLOVER" ) );
                }

                foreach( OrderMessage order in dealingBroadcast.OrderList )
                {
                    if( order.HasOrderData )
                    {
                        Order o = new Order( order.OrderData );

                        foreach( AllocationData data in order.AllocationDataList )
                        {
                            o.AddAllocation( new Allocation( data ) );
                        }

                        Update( o );
                    }

                    foreach( ReleaseData data in order.ReleaseDataList )
                    {
                        Update( new Release( data ) );
                    }

                    foreach( ExecutionData data in order.ExecutionDataList )
                    {
                        Update( new Execution( data ) );
                    }
                }
            }
            else if( broadcast.Type == ClientAdapterBroadcast.Types.BroadcastType.MARKETDATA )
            {
                MarketDataBroadcast marketDataBroadcast = broadcast.MarketdataBroadcast;

                if( marketDataBroadcast.Type == MarketDataBroadcast.Types.BroadcastType.PRICE )
                {
                    foreach( MarketDataPriceData priceData in marketDataBroadcast.PriceDataList )
                    {
                        if( HasInstrument( priceData.InstrumentId ) )
                        {
                            Instrument instrument = (Instrument)instruments[ priceData.InstrumentId ];

                            instrument.SetAttribute(
                                DatabaseObject.LAST_PRC,
                                Convert.ToString( priceData.LastPrc ) );

                            dispatcher.InstrumentChangedCallback(
                                new MarketDataEventArgs( instrument ) );
                        }
                    }
                }
            }
	    }
    }
}
