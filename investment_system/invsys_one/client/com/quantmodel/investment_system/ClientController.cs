using System;
using System.IO;
using System.Text;
using System.Windows;
using System.Threading;
using System.Collections;
using System.Configuration;
using System.Windows.Forms;

using quantmodel;

namespace com.quantmodel.investment_system
{
    public class ClientController : UserControl
    {
        private delegate void BroadcastEventDelegate( BroadcastEventArgs args );

        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( ClientController ) );

        private static readonly Context broadcast_ctx = new Context( 1 );

        private readonly ConnectionElement connectionConfiguration;

        public string PriceFormat { get { return connectionConfiguration.PriceFormat; } }
        public ConnectionElement ConnectionElement { get { return connectionConfiguration; } }

        public ClientController( ConnectionElement conn ) : base()
        {
            connectionConfiguration = conn;

            //
            //  Create the broadcast receiver thread
            //

            Thread broadcastThread = new Thread( new ThreadStart( BroadcastReceiver ) );
            broadcastThread.Name = "BroadcastReceiver";
            broadcastThread.IsBackground = true;
            broadcastThread.Start();
        }

        public event EventHandler<BroadcastEventArgs> OnBroadcast = null;
        public virtual void OnBroadcastCallback( BroadcastEventArgs e )
        {
            EventHandler<BroadcastEventArgs> hdlr = OnBroadcast;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new BroadcastEventDelegate( OnBroadcastCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }

        private void BroadcastReceiver()
        {
            log.Info( "Starting broadcast receiver" );

            try
            {
                Socket broadcast = broadcast_ctx.Socket( SocketType.SUB );
                broadcast.Subscribe( "", Encoding.Unicode );
                broadcast.Connect( connectionConfiguration.Broadcast );

                while( true )
                {
                    InvestmentSystemBroadcast message =
                        InvestmentSystemBroadcast.ParseFrom( broadcast.Recv( ) );

                    if( log.IsDebugEnabled )
                    {
                        log.Debug( " -b-> " + message.ToString() );
                    }

                    OnBroadcastCallback( new BroadcastEventArgs( message ) );
                }
            }
            catch( System.Exception e )
            {
                log.Error( "BroadcastReceiver exiting:", e );
            }
        }
    }
}
