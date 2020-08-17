using System;
using System.IO;
using System.Text;
using System.Windows;
using System.Threading;
using System.Collections;
using System.Configuration;
using System.Windows.Forms;

using quantmodel;
using com.quantmodel.common.data;
using com.quantmodel.common.events;
using com.quantmodel.common.network;
using com.quantmodel.common.network.message;

namespace com.quantmodel.client
{
    public class ClientController : UserControl
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( ClientController ) );

        private delegate void RpcEventDelegate( object state );
        private delegate void HeartbeatDelegate( );

        public static readonly string session_id = Guid.NewGuid().ToString();

        private static readonly Mutex sendMutex = new Mutex();

        private readonly int timeout;
        private readonly System.IntPtr handle;
        private readonly Socket rpc;
        private readonly string rpc_addr, bcast_addr;
        private readonly System.Threading.Timer timer;
        private readonly EventDispatcher dispatcher;
        private readonly ActiveSet activeSet;
        private readonly PositionManager positionManager;

        private static readonly Context rpc_ctx = new Context( 1 );
        private static readonly Context broadcast_ctx = new Context( 1 );

        //
        //  Properties
        //

        private User userData;
        public User ApplicationUser
        {
            get { return userData; }
            set { userData = value; }
        }

        public ActiveSet Cache { get{ return activeSet; } }
        public EventDispatcher Dispatcher { get { return dispatcher; } }
        public PositionManager Position { get { return positionManager; } }

        public ClientController( string rpc_addr, string bcast_addr ) : base()
        {
            this.rpc_addr = rpc_addr;
            this.bcast_addr = bcast_addr;

            handle = this.Handle;
            dispatcher = new EventDispatcher();
            activeSet = new ActiveSet( dispatcher );
            positionManager = new PositionManager( dispatcher );

            //
            //  Set the socket addresses
            //

            rpc = rpc_ctx.Socket( SocketType.REQ );
            rpc.Connect( rpc_addr );

            //
            //  Create the RPC timer
            //

            timer = new System.Threading.Timer(
                this.TimeoutEvent, null,
                Timeout.Infinite,
                Timeout.Infinite );

            //
            //  Create the broadcast receiver thread
            //

            Thread broadcastThread = new Thread( new ThreadStart( BroadcastReceiver ) );
            broadcastThread.Name = "BroadcastReceiver";
            broadcastThread.IsBackground = true;
            broadcastThread.Start();

            //
            //  Create the heartbeat thread
            //

            Thread heartbeatThread = new Thread( new ThreadStart( HeartbeatSender ) );
            heartbeatThread.Name = "HeartbeatSender";
            heartbeatThread.IsBackground = true;
            heartbeatThread.Start();

            //
            //  Set the timeout
            //

            timeout = Convert.ToInt32( (string)ConfigurationManager.AppSettings[ "timeout" ] );
        }

        private void TimeoutEvent( object state )
        {
            if( this.InvokeRequired )
            {
                BeginInvoke( new RpcEventDelegate( TimeoutEvent ), state );
            }

            //
            //  Show error dialog
            //

            System.Windows.Forms.MessageBox.Show(
              "RPC Timeout to the QuantModel Server: " + rpc_addr,
              "Application Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );

            Environment.Exit( 1 );
        }

        public ClientAdapterResponse SendRpc( ClientMessage message )
        {
            return SendRpc( message, timeout );
        }

        public ClientAdapterResponse SendRpc( ClientMessage message, int timeout )
        {
            if( InvokeRequired )
            {
                log.Error( "SendRpc must be called on client thread." );
                return null;
            }
            else
            {
                sendMutex.WaitOne();

                //
                //  Set the RPC timer to fire in timeout seconds.
                //

                timer.Change( timeout, Timeout.Infinite );

                //
                //  Do the rpc -- Send / Recv
                //

                if( log.IsDebugEnabled )
                {
                    log.Debug( " <-s- " + message.ToMessage( session_id ).ToString() );
                }

                rpc.Send( message.ToMessage( session_id ).ToByteArray() );
                ClientAdapterResponse response = ClientAdapterResponse.ParseFrom( rpc.Recv( ) );
                timer.Change( Timeout.Infinite, Timeout.Infinite );

                sendMutex.ReleaseMutex();

                if( log.IsDebugEnabled )
                {
                    log.Debug( " -r-> " + response.ToString() );
                }

                return response;
            }
        }

        public void Close()
        {
            timer.Dispose();
        }

        private void SendHeartbeat()
        {
            SendRpc( new Heartbeat() );
        }

        private void HeartbeatSender()
        {
            log.Info( "Starting heartbeat sender" );

            try
            {
                while( true )
                {
                    System.Threading.Thread.Sleep( 60000 );

                    if( log.IsDebugEnabled )
                    {
                        log.Debug( "Sending client heartbeat." );
                    }

                    BeginInvoke( new HeartbeatDelegate( SendHeartbeat ) );
                }
            }
            catch( System.Exception e )
            {
                log.Error( "HeartbeatSender exiting:", e );
            }
        }

        private void BroadcastReceiver()
        {
            log.Info( "Starting broadcast receiver" );

            try
            {
                Socket broadcast = broadcast_ctx.Socket( SocketType.SUB );
                broadcast.Subscribe( "", Encoding.Unicode );
                broadcast.Connect( bcast_addr );

                while( true )
                {
                    ClientAdapterBroadcast message =
                        ClientAdapterBroadcast.ParseFrom( broadcast.Recv( ) );

                    if( log.IsDebugEnabled )
                    {
                        log.Debug( " -b-> " + message.ToString() );
                    }

                    dispatcher.OnBroadcastCallback( new BroadcastEventArgs( message ) );
                }
            }
            catch( System.Exception e )
            {
                log.Error( "BroadcastReceiver exiting:", e );
            }
        }

        public void LoadStaticData()
        {
            activeSet.ClearStaticData();
            ArrayList symbols = new ArrayList();
            ArrayList investmentSystems = new ArrayList();
            ClientAdapterResponse response;

            //
            //  Instruments
            //

            response = SendRpc( new GetInstrumentSymbols( ) );

            foreach( DatabaseEngineResponse.Types.ResultSet rs in response.DatabaseResponse.ResultSetList )
            {
                foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row row in rs.RowList )
                {
                    symbols.Add( row.ColumnList[ 0 ].Value );
                }
            }

            foreach( string symbol in symbols )
            {
                log.Info( "Loading instrument symbol: " + symbol );

                response = SendRpc( new GetActiveInstrumentsBySymbol( symbol ) );

                foreach( DatabaseEngineResponse.Types.ResultSet rs in response.DatabaseResponse.ResultSetList )
                {
                    foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row row in rs.RowList )
                    {
                        activeSet.Update( new Instrument( row ) );
                    }
                }
            }

            //
            //  Accounts
            //

            response = SendRpc( new GetUserAccounts( userData.Id ) );

            foreach( DatabaseEngineResponse.Types.ResultSet rs in response.DatabaseResponse.ResultSetList )
            {
                foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row row in rs.RowList )
                {
                    Account account = new Account( row );
                    activeSet.Update( new Account( row ) );

                    log.Info( "Loading account: " + account[ DatabaseObject.NAME ] );
                }
            }

            //
            //  InvestmentSystems
            //

            response = SendRpc( new GetUserInvestmentSystems( userData.Id ) );

            foreach( DatabaseEngineResponse.Types.ResultSet rs in response.DatabaseResponse.ResultSetList )
            {
                foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row row in rs.RowList )
                {
                    InvestmentSystem investmentSystem = new InvestmentSystem( row );
                    investmentSystems.Add( investmentSystem );

                    log.Info( "Loading investment system: " + investmentSystem[ DatabaseObject.NAME ] );
                }
            }

            //
            //  Get the strategies for each investment system
            //

            foreach( InvestmentSystem investmentSystem in investmentSystems )
            {
                response = SendRpc( new GetStrategiesByInvestmentSystem( investmentSystem.Id ) );

                foreach( DatabaseEngineResponse.Types.ResultSet rs in response.DatabaseResponse.ResultSetList )
                {
                    foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row row in rs.RowList )
                    {
                        Strategy strategy = new Strategy( row );
                        investmentSystem.AddStrategy( strategy );
                        activeSet.Update( investmentSystem );

                        log.Info( "Loading investment system " +
                            investmentSystem[ DatabaseObject.NAME ] +
                            " strategy: " + strategy[ DatabaseObject.NAME ] );
                    }
                }
            }
        }

        public void LoadActiveSet()
        {
            ClientAdapterResponse response = SendRpc( new GetActiveSet( ) );

            //
            //  Iterate through each overnight position
            //

            foreach( PositionOvernightData overnight_position in response.DealingResponse.PositionList )
            {
                positionManager.Add( new OvernightPosition( overnight_position ) );
            }

            //
            //  Iterate through each order message
            //

            foreach( OrderMessage order in response.DealingResponse.OrderList )
            {
                if( order.HasOrderData )
                {
                    Order o = new Order( order.OrderData );

                    foreach( AllocationData data in order.AllocationDataList )
                    {
                        o.AddAllocation( new Allocation( data ) );
                    }

                    //
                    //  Add the order to the active set
                    //

                    activeSet.Update( o );
                }

                //
                //  Get the releases...
                //

                foreach( ReleaseData data in order.ReleaseDataList )
                {
                    activeSet.Update( new Release( data ) );
                }

                //
                //  Get the executions...
                //

                foreach( ExecutionData data in order.ExecutionDataList )
                {
                    activeSet.Update( new Execution( data ) );
                }
            }
        }
    }
}
