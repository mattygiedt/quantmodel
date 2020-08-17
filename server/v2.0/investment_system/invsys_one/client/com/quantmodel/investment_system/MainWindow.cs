using System;
using System.IO;
using System.Text;
using System.Drawing;
using System.Windows;
using System.Threading;
using System.Collections;
using System.Globalization;
using System.Configuration;
using System.Windows.Forms;
using System.Runtime.Serialization.Formatters.Binary;

using GlacialComponents.Controls.GlacialTreeList;
using GlacialComponents.Controls.Common;
using GlacialComponents.Controls.GTLCommon;

using quantmodel;
using investment_system.invsys_one;

namespace com.quantmodel.investment_system
{
    public class MainWindow : UserControl
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( MainWindow ) );

        private static readonly CultureInfo enUS = new CultureInfo( "en-US" );

        private readonly ClientController controller;

        private readonly System.Windows.Forms.Timer timer =
            new System.Windows.Forms.Timer();
        private readonly GlacialTreeList data =
            new GlacialTreeList( "0DAA8D90BB9ACC06C6FDDF11F7DD73E8" );

        private readonly DateTime EPOCH = new DateTime( 1970, 1, 1, 0, 0, 0, DateTimeKind.Utc );

        private Panel bidaskPanel = null;
        private Panel bottomPanel = null;

        private Panel statusPanel = null;
        private Panel openPLPanel = null;
        private Panel closedPLPanel = null;

        private TabControl tabControl = null;

        private Label statusLabel, openPLLabel, closedPLLabel, bidaskLabel;
        private Button pauseButton, flattenButton, stopButton;
        private Color normal_bordercolor;
        private ChartPanel chartPanel;
        private bool loginFlag = false;

        public MainWindow( ClientController controller )
		{
		    this.controller = controller;
		    this.SuspendLayout();
		    this.Padding = new Padding( 4, 10, 4, 0 );
		    this.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Name = "MainWindow";
			InitializeComponents();

            controller.OnBroadcast +=
                new EventHandler<BroadcastEventArgs>(
                    FireBroadcast );

		    this.Load += new EventHandler( FireLogin );

            this.ResumeLayout( false );
		}

		private void InitializeComponents()
        {
		    data.Name = "data";
            data.BackColor = System.Drawing.SystemColors.Window;
            data.BackgroundStretchToFit = true;
            data.Dock = System.Windows.Forms.DockStyle.Fill;
            data.ItemHeight = 19;
            data.Location = new System.Drawing.Point( 0, 0 );
            data.MultiSelect = false;
            data.ShowPlusMinus = false;
            data.HotColumnTracking = false;
            data.HotNodeTracking = false;
            data.SnapLastColumn = true;
            data.AlternateBackground = Color.AliceBlue;
            data.UnfocusedSelectionColor = Color.LightGray;
            data.ShowRootLines = false;
            data.ShowFocusRect = true;
            data.FastFind = false;
            data.AlternatingColors = false;
            data.OptimizeForHighNodeCount = false;
            data.HotTrackingColor = SystemColors.HotTrack;
            data.SelectedTextColor = SystemColors.HighlightText;
            data.SelectionColor =  SystemColors.Highlight;
            data.ControlStyle = GTLControlStyles.XP;
            data.Font = new System.Drawing.Font( "Tahoma", 9.25F );

            GTLColumn execution_timestamp = new GTLColumn();
            execution_timestamp.BackColor = Color.White;
            execution_timestamp.CheckBoxes = false;
			execution_timestamp.ImageIndex = -1;
			execution_timestamp.Name = "execution_timestamp";
			execution_timestamp.Text = "TIMESTAMP";
			execution_timestamp.SortType = SortTypes.InsertionSort;
			execution_timestamp.Visible = true;
			execution_timestamp.Width = 150;
			execution_timestamp.TextAlignment = ContentAlignment.MiddleRight;

			GTLColumn execution_timebar = new GTLColumn();
            execution_timebar.BackColor = Color.White;
            execution_timebar.CheckBoxes = false;
			execution_timebar.ImageIndex = -1;
			execution_timebar.Name = "execution_timebar";
			execution_timebar.Text = "TIMEBAR";
			execution_timebar.SortType = SortTypes.InsertionSort;
			execution_timebar.Visible = true;
			execution_timebar.Width = 150;
			execution_timebar.TextAlignment = ContentAlignment.MiddleRight;

			GTLColumn side_code = new GTLColumn();
            side_code.BackColor = Color.White;
            side_code.CheckBoxes = false;
			side_code.ImageIndex = -1;
			side_code.Name = "side_code";
			side_code.Text = "SIDE";
			side_code.SortType = SortTypes.InsertionSort;
			side_code.Visible = true;
			side_code.Width = 90;
			side_code.TextAlignment = ContentAlignment.MiddleRight;

			GTLColumn execution_quantity = new GTLColumn();
            execution_quantity.BackColor = Color.White;
            execution_quantity.CheckBoxes = false;
			execution_quantity.ImageIndex = -1;
			execution_quantity.Name = "execution_quantity";
			execution_quantity.Text = "QUANTITY";
			execution_quantity.SortType = SortTypes.InsertionSort;
			execution_quantity.Visible = true;
			execution_quantity.Width = 90;
			execution_quantity.TextAlignment = ContentAlignment.MiddleRight;

			GTLColumn execution_price = new GTLColumn();
            execution_price.BackColor = Color.White;
            execution_price.CheckBoxes = false;
			execution_price.ImageIndex = -1;
			execution_price.Name = "execution_price";
			execution_price.Text = "PRICE";
			execution_price.SortType = SortTypes.InsertionSort;
			execution_price.Visible = true;
			execution_price.Width = 90;
			execution_price.TextAlignment = ContentAlignment.MiddleRight;

            data.Columns.AddRange(
			    new GTLColumn [] {
			        execution_timestamp,
			        execution_timebar,
                    side_code,
                    execution_quantity,
                    execution_price } );

            Label statusDesc = new Label();
            statusDesc = new Label();
            statusDesc.ForeColor = Color.Black;
            statusDesc.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            statusDesc.Location = new System.Drawing.Point( 0, 48 );
            statusDesc.Size = new Size( 120, 20 );
            statusDesc.Text = "System Status";

            statusLabel = new Label();
            statusLabel.ForeColor = Color.Black;
            statusLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            statusLabel.Location = new System.Drawing.Point( 0, 12 );
            statusLabel.Size = new Size( 120, 40 );
            statusLabel.Text = "INIT";
            statusLabel.Font = new System.Drawing.Font( "Arial", 12.25F,
                System.Drawing.FontStyle.Bold,
                    System.Drawing.GraphicsUnit.Point,
                        ((System.Byte)(0)));

            Label openPLDesc = new Label();
            openPLDesc.ForeColor = Color.Black;
            openPLDesc.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            openPLDesc.Location = new System.Drawing.Point( 0, 48 );
            openPLDesc.Size = new Size( 120, 20 );
            openPLDesc.Text = "Open PL";

            openPLLabel = new Label();
            openPLLabel.ForeColor = Color.Black;
            openPLLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            openPLLabel.Location = new System.Drawing.Point( 0, 12 );
            openPLLabel.Size = new Size( 120, 40 );
            openPLLabel.Text = "0.00";
            openPLLabel.Font = new System.Drawing.Font( "Arial", 12.25F,
                System.Drawing.FontStyle.Bold,
                    System.Drawing.GraphicsUnit.Point,
                        ((System.Byte)(0)));

            Label closedPLDesc = new Label();
            closedPLDesc.ForeColor = Color.Black;
            closedPLDesc.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            closedPLDesc.Location = new System.Drawing.Point( 0, 48 );
            closedPLDesc.Size = new Size( 120, 20 );
            closedPLDesc.Text = "Closed PL";

            closedPLLabel = new Label();
            closedPLLabel.ForeColor = Color.Black;
            closedPLLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            closedPLLabel.Location = new System.Drawing.Point( 0, 12 );
            closedPLLabel.Size = new Size( 120, 40 );
            closedPLLabel.Text = "0.00";
            closedPLLabel.Font = new System.Drawing.Font( "Arial", 12.25F,
                System.Drawing.FontStyle.Bold,
                    System.Drawing.GraphicsUnit.Point,
                        ((System.Byte)(0)));

            bidaskLabel = new Label();
            bidaskLabel.BackColor = System.Drawing.Color.Transparent;
            bidaskLabel.ForeColor = Color.DarkSlateBlue;
            bidaskLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            bidaskLabel.Anchor = AnchorStyles.Top | AnchorStyles.Right;
            bidaskLabel.AutoSize = true;
            bidaskLabel.Text = "[ ASK , BID ]";
            bidaskLabel.Font = new System.Drawing.Font(
                "Tahoma", 12.25F, System.Drawing.FontStyle.Bold,
                    System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));

            bidaskPanel = new Panel();
	        bidaskPanel.BackColor = System.Drawing.SystemColors.Control;
			bidaskPanel.Anchor = AnchorStyles.Top | AnchorStyles.Right;
			bidaskPanel.Name = "bidaskPanel";
			bidaskPanel.Size = new System.Drawing.Size( 400, 30 );
			bidaskPanel.Controls.Add( bidaskLabel );
			bidaskPanel.Padding = new Padding( 0, 6, 2, 0 );

            //
            //  Chart
            //

            chartPanel = new ChartPanel();

            //
            //  Buttons
            //

			pauseButton = new Button();
            pauseButton.Size = new System.Drawing.Size( 194, 26 );
            pauseButton.Text = "PAUSE INVESTMENT SYSTEM";
            pauseButton.TabIndex = 1;
            pauseButton.TabStop = true;
            pauseButton.Location = new System.Drawing.Point( 60, 24 );
            pauseButton.Click += new EventHandler( FirePauseButton );
            pauseButton.Anchor = AnchorStyles.Right;
            pauseButton.Enabled = false;

            flattenButton = new Button();
            flattenButton.Size = new System.Drawing.Size( 184, 26 );
            flattenButton.Text = "FLATTEN OPEN POSITIONS";
            flattenButton.TabIndex = 1;
            flattenButton.TabStop = true;
            flattenButton.Location = new System.Drawing.Point( 260, 24 );
            flattenButton.Click += new EventHandler( FireFlattenButton );
            flattenButton.Anchor = AnchorStyles.Right;
            flattenButton.Enabled = false;

            stopButton = new Button();
            stopButton.Size = new System.Drawing.Size( 184, 26 );
            stopButton.Text = "STOP INVESTMENT SYSTEM";
            stopButton.TabIndex = 1;
            stopButton.TabStop = true;
            stopButton.Location = new System.Drawing.Point( 450, 24 );
            stopButton.Click += new EventHandler( FireShutdownButton );
            stopButton.Anchor = AnchorStyles.Right;
            stopButton.Enabled = true;

            //
            //  Info panels
            //

            statusPanel = new Panel();
	        statusPanel.BackColor = System.Drawing.SystemColors.Control;
			statusPanel.Name = "statusPanel";
            statusPanel.Dock = System.Windows.Forms.DockStyle.Left;
			statusPanel.Size = new System.Drawing.Size( 120, 74 );
            statusPanel.Controls.AddRange( new Control [] {
                statusDesc, statusLabel } );

            openPLPanel = new Panel();
	        openPLPanel.BackColor = System.Drawing.SystemColors.Control;
			openPLPanel.Name = "openPLPanel";
            openPLPanel.Dock = System.Windows.Forms.DockStyle.Left;
			openPLPanel.Size = new System.Drawing.Size( 120, 74 );
            openPLPanel.Controls.AddRange( new Control [] {
                openPLDesc, openPLLabel } );

            closedPLPanel = new Panel();
	        closedPLPanel.BackColor = System.Drawing.SystemColors.Control;
			closedPLPanel.Name = "closedPLPanel";
            closedPLPanel.Dock = System.Windows.Forms.DockStyle.Left;
			closedPLPanel.Size = new System.Drawing.Size( 120, 74 );
            closedPLPanel.Controls.AddRange( new Control [] {
                closedPLDesc, closedPLLabel } );

            bottomPanel = new Panel();
	        bottomPanel.BackColor = System.Drawing.SystemColors.Control;
			bottomPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
			bottomPanel.Name = "bottomPanel";
			bottomPanel.Size = new System.Drawing.Size( 660, 74 );

			bottomPanel.Controls.AddRange( new Control [] {
		        closedPLPanel, openPLPanel, statusPanel,
		        stopButton } );

			//
			//  TabControl
			//

			tabControl = new TabControl();
			tabControl.Padding = new System.Drawing.Point( 12, 6 );
			TabPage chartPage = new TabPage( "TimeBar" );
			chartPage.BackColor = System.Drawing.Color.WhiteSmoke;
			TabPage tradePage = new TabPage( "Trades" );
			TabPage propsPage = new TabPage( "Properties" );

			chartPage.Controls.Add( chartPanel );
			tradePage.Controls.Add( data );

			tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
			tabControl.Controls.AddRange( new Control [] { chartPage, tradePage, propsPage } );

            this.Controls.AddRange( new Control [] { bidaskPanel, tabControl, bottomPanel } );
        }

        private void FireBroadcast( object o, BroadcastEventArgs e )
        {
            DateTime dateValue;
            InvestmentSystemBroadcast investment_system_broadcast = e.Data;

            if( investment_system_broadcast.Type ==
                InvestmentSystemBroadcast.Types.BroadcastType.DATA )
            {
                InvestmentSystemOneData invsys_one_data =
                    InvestmentSystemOneData.ParseFrom(
                        investment_system_broadcast.Delegate );

                if( invsys_one_data.Type == InvestmentSystemOneData.Types.DataType.SYSTEM )
                {
                    DateTime.TryParseExact(
                        invsys_one_data.UpdateTm, "HH:mm.ss", enUS,
                        DateTimeStyles.AssumeUniversal, out dateValue );

                    statusLabel.Text = invsys_one_data.Status;

                    double ask = Convert.ToDouble( invsys_one_data.BestAsk );
                    double bid = Convert.ToDouble( invsys_one_data.BestBid );

                    openPLLabel.Text = invsys_one_data.OpenPl.ToString( "0.00" );
                    closedPLLabel.Text = invsys_one_data.ClosedPl.ToString( "0.00" );

                    if( invsys_one_data.OpenPl > 0 )
                    {
                        openPLLabel.ForeColor = Color.Green;
                    }
                    else if( invsys_one_data.OpenPl < 0 )
                    {
                        openPLLabel.ForeColor = Color.Red;
                    }
                    else
                    {
                        openPLLabel.ForeColor = Color.Black;
                    }

                    if( invsys_one_data.ClosedPl > 0 )
                    {
                        closedPLLabel.ForeColor = Color.Green;
                    }
                    else if( invsys_one_data.ClosedPl < 0 )
                    {
                        closedPLLabel.ForeColor = Color.Red;
                    }
                    else
                    {
                        closedPLLabel.ForeColor = Color.Black;
                    }

                    chartPanel.UpdateBidAsk( bid, ask );

                    bidaskLabel.Text = ask.ToString( controller.PriceFormat ) +
                    " , " + bid.ToString( controller.PriceFormat );

                    this.ParentForm.Text = controller.ConnectionElement.Name +
                        " " + invsys_one_data.Status + " " + dateValue.ToLongTimeString();

                    if( invsys_one_data.TimebarList.Count == 1 )
                    {
                        chartPanel.UpdateTimebarData( invsys_one_data.TimebarList[ 0 ], false );
                    }
                }
                else if( loginFlag == true &&
                         invsys_one_data.Type == InvestmentSystemOneData.Types.DataType.TIMEBAR )
                {
                    foreach( Timebar timebar in invsys_one_data.TimebarList )
                    {
                        chartPanel.UpdateTimebarData( timebar, true );
                    }
                }
                else if( loginFlag == true &&
                         invsys_one_data.Type == InvestmentSystemOneData.Types.DataType.TREND )
                {
                    foreach( Trend trend in invsys_one_data.TrendList )
                    {
                        chartPanel.AddTrendData( trend );
                    }
                }
                else if( loginFlag == true &&
                         invsys_one_data.Type == InvestmentSystemOneData.Types.DataType.EXECUTION )
                {
                    foreach( OrderExecution execution in invsys_one_data.OrderExecutionList )
                    {
                        chartPanel.AddOrderExecutionData( execution, true );

                        GTLTreeNode node = new GTLTreeNode(
                            EPOCH.AddSeconds( execution.Timestamp )
                                .ToLocalTime().ToString() );

                        if( execution.Type == OrderExecution.Types.ExecutionType.BUY )
                        {
                            node.SubItems.AddRange( new GTLSubItem [] {
                                new GTLSubItem( EPOCH.AddSeconds( execution.Timebar ).ToLocalTime().ToString() ),
                                new GTLSubItem( "BUY" ),
                                new GTLSubItem( "" + execution.Quantity ),
                                new GTLSubItem( "" + execution.Price ) } );
                        }
                        else
                        {
                            node.SubItems.AddRange( new GTLSubItem [] {
                                new GTLSubItem( EPOCH.AddSeconds( execution.Timebar ).ToLocalTime().ToString() ),
                                new GTLSubItem( "SELL" ),
                                new GTLSubItem( "" + execution.Quantity ),
                                new GTLSubItem( "" + execution.Price ) } );
                        }

                        data.Nodes.Insert( 0, node );
                    }
                }

                chartPanel.Redraw();
            }
        }

        private void FireLogin( object sender, EventArgs e )
        {
            string request_id = Guid.NewGuid().ToString();
            string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();

            InvestmentSystemOneRequest hades_request =
                InvestmentSystemOneRequest.CreateBuilder()
                    .SetRequestId( request_id )
                    .SetTimestamp( timestamp )
                    .SetType( InvestmentSystemOneRequest.Types.RequestType.LOGIN )
                .Build();

            InvestmentSystemResponse response = sendRPC(
                InvestmentSystemRequest.CreateBuilder()
                    .SetRequestId( request_id )
                    .SetSessionId( request_id )
                    .SetTimestamp( timestamp )
                    .SetType( InvestmentSystemRequest.Types.RequestType.COMMAND )
                    .SetDelegate( System.Text.Encoding.UTF8.GetString( hades_request.ToByteArray() ) )
                 .Build() );

            InvestmentSystemOneData invsys_one_data =
                InvestmentSystemOneData.ParseFrom( response.Delegate );

            foreach( Timebar timebar in invsys_one_data.TimebarList )
            {
                chartPanel.UpdateTimebarData( timebar, true );
            }

            foreach( Trend trend in invsys_one_data.TrendList )
            {
                chartPanel.AddTrendData( trend );
            }

            foreach( OrderExecution execution in invsys_one_data.OrderExecutionList )
            {
                chartPanel.AddOrderExecutionData( execution, true );

                GTLTreeNode node = new GTLTreeNode(
                    EPOCH.AddSeconds( execution.Timestamp )
                        .ToLocalTime().ToString() );

                if( execution.Type == OrderExecution.Types.ExecutionType.BUY )
                {
                    node.SubItems.AddRange( new GTLSubItem [] {
                        new GTLSubItem( EPOCH.AddSeconds( execution.Timebar ).ToLocalTime().ToString() ),
                        new GTLSubItem( "BUY" ),
                        new GTLSubItem( "" + execution.Quantity ),
                        new GTLSubItem( "" + execution.Price ) } );
                }
                else
                {
                    node.SubItems.AddRange( new GTLSubItem [] {
                        new GTLSubItem( EPOCH.AddSeconds( execution.Timebar ).ToLocalTime().ToString() ),
                        new GTLSubItem( "SELL" ),
                        new GTLSubItem( "" + execution.Quantity ),
                        new GTLSubItem( "" + execution.Price ) } );
                }

                data.Nodes.Insert( 0, node );
            }

            chartPanel.Redraw();
            loginFlag = true;
        }

        private void FirePauseButton( object o, EventArgs e )
        {
            if( ModifierKeys == Keys.Shift )
            {
                string request_id = Guid.NewGuid().ToString();
                string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();

                if( pauseButton.Text == "PAUSE INVESTMENT SYSTEM" )
                {
                    normal_bordercolor = pauseButton.ForeColor;
                    pauseButton.Text = "RESUME INVESTMENT SYSTEM";
                    pauseButton.ForeColor = Color.Red;

                    InvestmentSystemOneRequest hades_request =
                        InvestmentSystemOneRequest.CreateBuilder()
                            .SetRequestId( request_id )
                            .SetTimestamp( timestamp )
                            .SetType( InvestmentSystemOneRequest.Types.RequestType.PAUSE )
                        .Build();

                    InvestmentSystemResponse response = sendRPC(
                        InvestmentSystemRequest.CreateBuilder()
                            .SetRequestId( request_id )
                            .SetSessionId( request_id )
                            .SetTimestamp( timestamp )
                            .SetType( InvestmentSystemRequest.Types.RequestType.COMMAND )
                            .SetDelegate( System.Text.Encoding.UTF8.GetString( hades_request.ToByteArray() ) )
                         .Build() );
                }
                else
                {
                    pauseButton.Text = "PAUSE INVESTMENT SYSTEM";
                    pauseButton.ForeColor = normal_bordercolor;

                    InvestmentSystemOneRequest hades_request =
                        InvestmentSystemOneRequest.CreateBuilder()
                            .SetRequestId( request_id )
                            .SetTimestamp( timestamp )
                            .SetType( InvestmentSystemOneRequest.Types.RequestType.RESUME )
                        .Build();

                    InvestmentSystemResponse response = sendRPC(
                        InvestmentSystemRequest.CreateBuilder()
                            .SetRequestId( request_id )
                            .SetSessionId( request_id )
                            .SetTimestamp( timestamp )
                            .SetType( InvestmentSystemRequest.Types.RequestType.COMMAND )
                            .SetDelegate( System.Text.Encoding.UTF8.GetString( hades_request.ToByteArray() ) )
                         .Build() );
                }
            }
        }

        private void FireFlattenButton( object o, EventArgs e )
        {
            if( ModifierKeys == Keys.Shift )
            {
                string request_id = Guid.NewGuid().ToString();
                string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();

                InvestmentSystemOneRequest hades_request =
                    InvestmentSystemOneRequest.CreateBuilder()
                        .SetRequestId( request_id )
                        .SetTimestamp( timestamp )
                        .SetType( InvestmentSystemOneRequest.Types.RequestType.FLATTEN )
                    .Build();

                InvestmentSystemResponse response = sendRPC(
                    InvestmentSystemRequest.CreateBuilder()
                        .SetRequestId( request_id )
                        .SetSessionId( request_id )
                        .SetTimestamp( timestamp )
                        .SetType( InvestmentSystemRequest.Types.RequestType.COMMAND )
                        .SetDelegate( System.Text.Encoding.UTF8.GetString( hades_request.ToByteArray() ) )
                     .Build() );
            }
        }

        private void FireShutdownButton( object o, EventArgs e )
        {
            if( ModifierKeys == Keys.Shift )
            {
                //
                // Stop button is one-shot per login.
                //

                stopButton.Enabled = false;
                string request_id = Guid.NewGuid().ToString();
                string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();

                InvestmentSystemOneRequest hades_request =
                    InvestmentSystemOneRequest.CreateBuilder()
                        .SetRequestId( request_id )
                        .SetTimestamp( timestamp )
                        .SetType( InvestmentSystemOneRequest.Types.RequestType.SHUTDOWN )
                    .Build();

                InvestmentSystemResponse response = sendRPC(
                    InvestmentSystemRequest.CreateBuilder()
                        .SetRequestId( request_id )
                        .SetSessionId( request_id )
                        .SetTimestamp( timestamp )
                        .SetType( InvestmentSystemRequest.Types.RequestType.COMMAND )
                        .SetDelegate( System.Text.Encoding.UTF8.GetString( hades_request.ToByteArray() ) )
                     .Build() );
            }
        }

        private delegate void RpcEventDelegate( object state );

        private void TimeoutEvent( object state )
        {
            if( this.InvokeRequired )
            {
                BeginInvoke( new RpcEventDelegate( TimeoutEvent ), state );
            }

            //
            //  Show error dialog
            //

            System.Windows.Forms.MessageBox.Show( "RPC Timeout to the Investment System Proxy",
                "Application Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );

            Environment.Exit( 1 );
        }

        private InvestmentSystemResponse sendRPC( InvestmentSystemRequest request )
        {
            Context ctx = new Context( 1 );
            Socket callforward = ctx.Socket( SocketType.REQ );
            callforward.Connect( controller.ConnectionElement.Callforward );
            System.Threading.Timer timer = new System.Threading.Timer( this.TimeoutEvent, null, 5000, Timeout.Infinite );
            callforward.Send( request.ToByteArray() );
            InvestmentSystemResponse response = InvestmentSystemResponse.ParseFrom( callforward.Recv( ) );
            timer.Dispose();

            return response;
        }

/*
        private InvestmentSystemResponse sendRPC( InvestmentSystemRequest request )
        {
            Context ctx = new Context( 1 );
            Socket callforward = ctx.Socket( SocketType.REQ );
            callforward.Connect( (string)ConfigurationManager.AppSettings[ "callforward" ] );

            callforward.Send( request.ToByteArray() );
            return InvestmentSystemResponse.ParseFrom( callforward.Recv( ) );
        }
*/
    }
}
