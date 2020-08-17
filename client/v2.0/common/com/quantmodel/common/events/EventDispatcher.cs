using System;
using System.Windows.Forms;

namespace com.quantmodel.common.events
{
    public class EventDispatcher : UserControl
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( EventDispatcher ) );

        private readonly IntPtr handle;
        private delegate void StringEventDelegate( StringEventArgs args );
        private delegate void DealingEventDelegate( DealingEventArgs args );
        private delegate void PositionEventDelegate( PositionEventArgs args );
        private delegate void BroadcastEventDelegate( BroadcastEventArgs args );
        private delegate void MarketDataEventDelegate( MarketDataEventArgs args );

        public EventDispatcher()
        {
            //
            //  Force the handle creation
            //

            handle = this.Handle;
        }

        //
        //  Virtual methods used to raise events
        //

        public event EventHandler Shutdown = null;
        protected virtual void ShutdownCallback( EventArgs e )
        {
            EventHandler hdlr = Shutdown;
            if( hdlr != null ) hdlr( this, e );
        }

        ////////////////////////////////////////////////////////////////////////
        //  Client Adapter Broadcast

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

        ////////////////////////////////////////////////////////////////////////
        //  Client Navigation

        public event EventHandler<StringEventArgs> NavigationPaneChanged = null;
        public virtual void NavigationPaneChangedCallback( StringEventArgs e )
        {
            EventHandler<StringEventArgs> hdlr = NavigationPaneChanged;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new StringEventDelegate( NavigationPaneChangedCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }

        public event EventHandler<StringEventArgs> AdministrationPageChanged = null;
        public virtual void AdministrationPageChangedCallback( StringEventArgs e )
        {
            EventHandler<StringEventArgs> hdlr = AdministrationPageChanged;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new StringEventDelegate( AdministrationPageChangedCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }

        public event EventHandler<StringEventArgs> ReportDataChanged = null;
        public virtual void ReportDataChangedCallback( StringEventArgs e )
        {
            EventHandler<StringEventArgs> hdlr = ReportDataChanged;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new StringEventDelegate( ReportDataChangedCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }

        public event EventHandler<StringEventArgs> InstrumentSelected = null;
        public virtual void InstrumentSelectedCallback( StringEventArgs e )
        {
            EventHandler<StringEventArgs> hdlr = InstrumentSelected;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new StringEventDelegate( InstrumentSelectedCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }

        public event EventHandler<StringEventArgs> AccountSelected = null;
        public virtual void AccountSelectedCallback( StringEventArgs e )
        {
            EventHandler<StringEventArgs> hdlr = AccountSelected;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new StringEventDelegate( AccountSelectedCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        //  Position Manager

        public event EventHandler<PositionEventArgs> PositionChanged = null;
        public virtual void PositionChangedCallback( PositionEventArgs e )
        {
            EventHandler<PositionEventArgs> hdlr = PositionChanged;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new PositionEventDelegate( PositionChangedCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        //  Active Set

        public event EventHandler<DealingEventArgs> Rollover = null;
        public virtual void RolloverCallback( DealingEventArgs e )
        {
            EventHandler<DealingEventArgs> hdlr = Rollover;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new DealingEventDelegate( RolloverCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }

        public event EventHandler<MarketDataEventArgs> InstrumentChanged = null;
        public virtual void InstrumentChangedCallback( MarketDataEventArgs e )
        {
            EventHandler<MarketDataEventArgs> hdlr = InstrumentChanged;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new MarketDataEventDelegate( InstrumentChangedCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }

        public event EventHandler<DealingEventArgs> OrderAdded = null;
        public virtual void OrderAddedCallback( DealingEventArgs e )
        {
            EventHandler<DealingEventArgs> hdlr = OrderAdded;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new DealingEventDelegate( OrderAddedCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }

        public event EventHandler<DealingEventArgs> OrderChanged = null;
        public virtual void OrderChangedCallback( DealingEventArgs e )
        {
            EventHandler<DealingEventArgs> hdlr = OrderChanged;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new DealingEventDelegate( OrderChangedCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }

        public event EventHandler<DealingEventArgs> ReleaseAdded = null;
        public virtual void ReleaseAddedCallback( DealingEventArgs e )
        {
            EventHandler<DealingEventArgs> hdlr = ReleaseAdded;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new DealingEventDelegate( ReleaseAddedCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }

        public event EventHandler<DealingEventArgs> ReleaseChanged = null;
        public virtual void ReleaseChangedCallback( DealingEventArgs e )
        {
            EventHandler<DealingEventArgs> hdlr = ReleaseChanged;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new DealingEventDelegate( ReleaseChangedCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }

        public event EventHandler<DealingEventArgs> ExecutionAdded = null;
        public virtual void ExecutionAddedCallback( DealingEventArgs e )
        {
            EventHandler<DealingEventArgs> hdlr = ExecutionAdded;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new DealingEventDelegate( ExecutionAddedCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }

        public event EventHandler<DealingEventArgs> ExecutionChanged = null;
        public virtual void ExecutionChangedCallback( DealingEventArgs e )
        {
            EventHandler<DealingEventArgs> hdlr = ExecutionChanged;
            if( hdlr != null )
            {
                if( InvokeRequired )
                {
                    BeginInvoke( new DealingEventDelegate( ExecutionChangedCallback ), e );
                }
                else
                {
                    hdlr( this, e );
                }
            }
        }
    }
}
