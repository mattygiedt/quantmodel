using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Drawing;
using System.Threading;
using System.Xml.XPath;
using System.Collections;
using System.Reflection;
using System.Windows.Forms;
using System.ComponentModel;

using TD.Eyefinder;
using GlacialComponents.Controls.GlacialTreeList;
using GlacialComponents.Controls.Common;
using GlacialComponents.Controls.GTLCommon;

using quantmodel;
using com.quantmodel.common.data;
using com.quantmodel.common.events;
using com.quantmodel.common.controls;
using com.quantmodel.common.network.message;

namespace com.quantmodel.client.controls
{
    public sealed class DealingPane : TD.Eyefinder.NavigationPane
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( DealingPane ) );

        public static readonly string double_fmt = "#,##0.00;(#,##0.00);0.00";
        private ToolStripMenuItem toolStripMenuItem = null;
        private ToolStripSeparator toolStripSeparator = null;

        private readonly ClientController controller;
        private GlacialTreeList accounts = null;

        private SplitButton splitButton = null;
        private CheckBox sendToFix = null;
        private ContextMenuStrip splitButtonDropDown = null;

        public DealingPane( ClientController controller ) : base()
        {
		    this.controller = controller;

		    this.SuspendLayout();
		    this.AutoScroll = true;
		    this.Padding = new Padding( 6 );
            this.Text = "Dealing Engine";
            this.Name = "DealingPane";
            this.Tag = "DealingViewer";
            this.Dock = DockStyle.Fill;
            this.BackColor = System.Drawing.SystemColors.Control;
            InitializeComponents();
            this.ResumeLayout( false );

            LoadPositions( controller.Position.AccountPositions );
            LoadDealingStatus();

            controller.Dispatcher.Rollover +=
                new EventHandler<DealingEventArgs>( FireRollover );
            controller.Dispatcher.PositionChanged +=
                new EventHandler<PositionEventArgs>( FirePositionChanged );
            controller.Dispatcher.OnBroadcast +=
                new EventHandler<BroadcastEventArgs>( FireBroadcast );
        }

        private void InitializeComponents()
        {
            //
            //  Account List
            //

            accounts = new GlacialTreeList( "0DAA8D90BB9ACC06C6FDDF11F7DD73E8" );
            accounts.BackColor = System.Drawing.SystemColors.Window;
            accounts.BackgroundStretchToFit = true;
            accounts.Dock = System.Windows.Forms.DockStyle.Fill;
            accounts.ItemHeight = 19;
            accounts.Location = new System.Drawing.Point( 10, 80 );
            accounts.MultiSelect = false;
            accounts.ShowPlusMinus = false;
            accounts.HotColumnTracking = false;
            accounts.HotNodeTracking = false;
            accounts.SnapLastColumn = true;
            accounts.AlternateBackground = Color.AliceBlue;
            accounts.ShowRootLines = false;
            accounts.ShowFocusRect = true;
            accounts.FastFind = true;
            accounts.AlternatingColors = false;
            accounts.Name = "accounts";
            accounts.OptimizeForHighNodeCount = false;
            accounts.HotTrackingColor = SystemColors.HotTrack;
            accounts.SelectedTextColor = SystemColors.HighlightText;
            accounts.SelectionColor =  SystemColors.Highlight;
            accounts.UnfocusedSelectionColor =  SystemColors.Highlight;
            accounts.ControlStyle = GTLControlStyles.XP;
            accounts.Font = new System.Drawing.Font( "Tahoma", 9.25F );

            //
            //  account list columns
            //

            GTLColumn account_name = new GTLColumn();
            account_name.BackColor = Color.White;
            account_name.CheckBoxes = false;
            account_name.ImageIndex = -1;
            account_name.Name = "account_name";
            account_name.Text = "Account";
            account_name.Width = 132;
            account_name.SortType = SortTypes.InsertionSort;

            GTLColumn notional_pl = new GTLColumn();
            notional_pl.BackColor = Color.White;
            notional_pl.CheckBoxes = false;
            notional_pl.ImageIndex = -1;
            notional_pl.Name = "notional_pl";
            notional_pl.Text = "PL";
            notional_pl.Width = 100;
            notional_pl.TextAlignment = ContentAlignment.MiddleRight;
            notional_pl.SortType = SortTypes.InsertionSort;

            accounts.Columns.AddRange( new GTLColumn [] { account_name, notional_pl } );

            //
            //  Send to FIX checkbox
            //

            sendToFix = new CheckBox();
            sendToFix.AutoSize = true;
            sendToFix.CausesValidation = false;
            sendToFix.Checked = false;
            sendToFix.Text = "Release to FIX Engine";
            sendToFix.Location = new System.Drawing.Point( 10, 24 );
            sendToFix.Click  += new EventHandler( FireSendToFixChanged );

            //
            //  flatten menu-items
            //

            toolStripMenuItem = new ToolStripMenuItem();
            toolStripMenuItem.Name = "ALL";
            toolStripMenuItem.Size = new System.Drawing.Size( 188, 22 );
            toolStripMenuItem.Text = "Flatten ALL Open Positions";
            //toolStripMenuItem.Click += new EventHandler( FireFlattenAccount );

            //
            //  toolStripSeparator
            //

            toolStripSeparator = new ToolStripSeparator();
            toolStripSeparator.Name = "toolStripSeparator";
            toolStripSeparator.Size = new System.Drawing.Size( 185, 6 );

            System.ComponentModel.IContainer components = new System.ComponentModel.Container();
            splitButtonDropDown = new ContextMenuStrip( components );
            splitButtonDropDown.Name = "splitButtonDropDown";
            splitButtonDropDown.Size = new System.Drawing.Size( 189, 76 );

            //
            //  Split button
            //

            splitButton = new SplitButton();
            splitButton.Enabled = sendToFix.Checked;
            splitButton.AutoSize = true;
            splitButton.AlwaysDropDown = true;
            splitButton.Location = new System.Drawing.Point( 10, 52 );
            splitButton.ClickedImage = "SplitButtonClickedImage.gif";
            splitButton.ContextMenuStrip = splitButtonDropDown;
            splitButton.DisabledImage = "SplitButtonDisabledImage.gif";
            splitButton.HoverImage = "SplitButtonHoverImage.gif";
            splitButton.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            splitButton.ImageKey = "SplitButtonImage.gif";
            splitButton.Name = "splitButton";
            splitButton.NormalImage = "SplitButtonImage.gif";
            splitButton.Size = new System.Drawing.Size(125, 26);
            splitButton.TabIndex = 0;
            splitButton.Text = "Flatten Positions";
            splitButton.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            splitButton.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
            splitButton.UseVisualStyleBackColor = true;

            GroupBox statusGroupBox = new GroupBox();
            statusGroupBox.Dock = DockStyle.Top;
            statusGroupBox.AutoSize = true;
            statusGroupBox.FlatStyle = FlatStyle.Standard;
            statusGroupBox.Text = "Dealing Status";
            statusGroupBox.Controls.AddRange( new Control [] {
                sendToFix,
                splitButton
                } );

            Panel spacer1 = new Panel();
            spacer1.Size = new System.Drawing.Size( 125, 12 );
            spacer1.Dock = DockStyle.Top;

            Panel spacer2 = new Panel();
            spacer2.Size = new System.Drawing.Size( 125, 12 );
            spacer2.Dock = DockStyle.Top;

            this.Controls.AddRange( new Control[] {
                accounts,
                spacer1,
                statusGroupBox
                } );
        }

        private void LoadDealingStatus()
        {
            ClientAdapterResponse response = controller.SendRpc( new GetDealingStatus( ) );

            //
            //  Find the send_to_fix parameter
            //

            DealingEngineResponse dealing_response = response.DealingResponse;

            foreach( SystemParameterData system_parameter in dealing_response.SystemParameterList )
            {
                if( "send_to_fix".Equals( system_parameter.Name ) )
                {
                    sendToFix.Checked = "1".Equals( system_parameter.Value );
                    splitButton.Enabled = sendToFix.Checked;
                    return;
                }
            }
        }

        private void FireBroadcast( object sender, BroadcastEventArgs e )
		{
            ClientAdapterBroadcast broadcast = e.Data;

            if( broadcast.Type == ClientAdapterBroadcast.Types.BroadcastType.DEALING )
            {
                DealingEngineBroadcast dealing_broadcast = broadcast.DealingBroadcast;

                if( dealing_broadcast.Type == DealingEngineBroadcast.Types.BroadcastType.UPDATE_STATUS )
                {
                    foreach( SystemParameterData system_parameter in dealing_broadcast.SystemParameterList )
                    {
                        if( "send_to_fix".Equals( system_parameter.Name ) )
                        {
                            sendToFix.Checked = "1".Equals( system_parameter.Value );
                            splitButton.Enabled = sendToFix.Checked;
                            return;
                        }
                    }
                }
            }
        }

        private void FireRollover( object o, DealingEventArgs e )
        {
            accounts.BeginUpdate();
            accounts.BeginNodeListChange();
            accounts.Nodes.Clear();
            accounts.EndNodeListChange();
            accounts.EndUpdate();

            splitButtonDropDown.Items.Clear();
        }

        private void LoadPositions( ArrayList positions )
        {
            double notional_pl;

            accounts.BeginUpdate();
            accounts.BeginNodeListChange();

            foreach( AccountPosition account_position in positions )
            {
                //
                //  Get the account on the position
                //

                Account account = controller.Cache.GetAccount( account_position.AccountId );

                if( null == account )
                {
                    log.Warn( "Unknown account: " + account_position.AccountId );
                    continue;
                }

                ToolStripMenuItem tsmi = new ToolStripMenuItem();
                tsmi.Name = account.Id;
                tsmi.Size = new System.Drawing.Size( 188, 22 );
                tsmi.Text = account[ DatabaseObject.NAME ];
                tsmi.Click += new EventHandler( FireFlattenAccount );
                splitButtonDropDown.Items.Add( tsmi );

                notional_pl = 0.0;
                GTLTreeNode account_node = new GTLTreeNode( account[ DatabaseObject.NAME ] );

                foreach( InstrumentPosition instrument_position in account_position.InstrumentPositions )
                {
                    //
                    //  Get the instrument
                    //

                    Instrument instrument =
                        controller.Cache.GetInstrument(
                            instrument_position.InstrumentId );

                    if( null == instrument )
                    {
                        log.Warn( "Unknown instrument_id: " + instrument_position.InstrumentId  );
                        continue;
                    }

                    //
                    //  Calculate the PL
                    //

                    instrument_position.ProfitLoss( account, instrument );
                    notional_pl += instrument_position.NotionalPL;
                }

                account_node.SubItems.AddRange( new GTLSubItem [] {
                    new PercentSubItem( notional_pl, true ) } );

                account_node.Tag = account.Id;
                accounts.Nodes.Add( account_node );
            }

            accounts.EndNodeListChange();
            accounts.EndUpdate();
        }

        private void FirePositionChanged( object o, PositionEventArgs e )
        {
            double notional_pl;
            AccountPosition account_position = (AccountPosition)e.Data;

            accounts.BeginUpdate();
            accounts.BeginNodeListChange();

            //
            //  Get the account on the position
            //

            Account account = controller.Cache.GetAccount( account_position.AccountId );

            if( null == account )
            {
                log.Warn( "Unknown account: " + account_position.AccountId );
                return;
            }

            //
            //  Calculate the notional_pl
            //

            notional_pl = 0.0;
            foreach( InstrumentPosition instrument_position in account_position.InstrumentPositions )
            {
                //
                //  Get the instrument
                //

                Instrument instrument =
                    controller.Cache.GetInstrument(
                        instrument_position.InstrumentId );

                if( null == instrument )
                {
                    log.Warn( "Unknown instrument_id: " + instrument_position.InstrumentId  );
                    continue;
                }

                //
                //  Calculate the notional PL
                //

                instrument_position.ProfitLoss( account, instrument );
                notional_pl += instrument_position.NotionalPL;
            }

            //
            //  Find this account in our list?
            //

            bool found = false;
            foreach( GTLTreeNode account_node in accounts.Nodes )
            {
                if( account_node.Tag.Equals( account.Id ) )
                {
                    ((PercentSubItem)account_node.SubItems[ "notional_pl" ]).Update( notional_pl );
                    found = true;
                    break;
                }
            }

            if( !found )
            {
                GTLTreeNode account_node = new GTLTreeNode( account[ DatabaseObject.NAME ] );
                account_node.SubItems.AddRange( new GTLSubItem [] {
                    new PercentSubItem( notional_pl, true ) } );

                account_node.Tag = account.Id;
                accounts.Nodes.Add( account_node );

                ToolStripMenuItem tsmi = new ToolStripMenuItem();
                tsmi.Name = account.Id;
                tsmi.Size = new System.Drawing.Size( 188, 22 );
                tsmi.Text = account[ DatabaseObject.NAME ];
                tsmi.Click += new EventHandler( FireFlattenAccount );
                splitButtonDropDown.Items.Add( tsmi );
            }

            accounts.EndNodeListChange();
            accounts.EndUpdate();
        }

        private void FireSendToFixChanged( object o, EventArgs e )
        {
            if(! sendToFix.Checked )
            {
                //
                //  Display 'are you sure' message box...
                //

                DialogResult result = MessageBox.Show(
                    "Are you sure you want to FLATTEN ALL OPEN POSITIONS and stop sending releases to the FIX Engine?",
                    "Stop Sending to FIX Confirmation",
                    MessageBoxButtons.YesNo,
                    MessageBoxIcon.Exclamation );

                if( result == System.Windows.Forms.DialogResult.No )
                {
                    //
                    //  Cancel the event
                    //

                    sendToFix.Checked = true;
                }
                else
                {
                    //
                    //  Generate offsetting market orders for all accounts
                    //  with open positions.
                    //

                    foreach( AccountPosition account_position in controller.Position.AccountPositions )
                    {
                        CloseAccountPosition( account_position );
                    }
                }

                ClientAdapterResponse response = controller.SendRpc(
                    new UpdateDealingStatus( "send_to_fix", "0" ) );
            }
            else
            {
                ClientAdapterResponse response = controller.SendRpc(
                    new UpdateDealingStatus( "send_to_fix", "1" ) );
            }

            splitButton.Enabled = sendToFix.Checked;
        }

        private void FireFlattenAccount( object sender, EventArgs e )
        {
            ToolStripMenuItem tsmi = (ToolStripMenuItem)sender;

            string message = null;
            if( "ALL".Equals( tsmi.Name ) )
            {
                message = "Are you sure you want to flatten ALL open positions?";
            }
            else
            {
                message = "Are you sure you want to flatten open positions for account:\n" + tsmi.Text;
            }

            DialogResult result = MessageBox.Show(
                message,
                "Flatten Positions Confirmation",
                MessageBoxButtons.YesNo,
                MessageBoxIcon.Exclamation );

            if( result == System.Windows.Forms.DialogResult.Yes )
            {
                foreach( AccountPosition account_position in controller.Position.AccountPositions )
                {
                    if( account_position.AccountId.Equals( tsmi.Name ) )
                    {
                        CloseAccountPosition( account_position );
                        break;
                    }
                }
            }
        }

        private void CloseAccountPosition( AccountPosition account_position )
        {
            foreach( InstrumentPosition instrument_position in account_position.InstrumentPositions )
            {
                DraftOrderData.Types.SideCode sideCode;

                if( instrument_position.Exposure == InstrumentPosition.PositionExposure.LONG )
                {
                    sideCode = DraftOrderData.Types.SideCode.SELL;
                }
                else if( instrument_position.Exposure == InstrumentPosition.PositionExposure.SHORT )
                {
                    sideCode = DraftOrderData.Types.SideCode.BUY;
                }
                else
                {
                    //
                    //  No need to close a flat position.
                    //

                    continue;
                }

                DraftOrderData draftOrder = DraftOrderData.CreateBuilder()
                    .SetInvestmentSystemId( instrument_position.InvestmentSystemId )
                    .SetInstrumentId( instrument_position.InstrumentId )
                    .SetSideCode( sideCode )
                    .SetTif( DraftOrderData.Types.TimeInForce.DAY )
                    .SetOrderType( DraftOrderData.Types.OrderType.MARKET )
                    .SetOrderQty( Convert.ToString( instrument_position.OpenPosition ) )
                    .SetLimitPrc( "0" )
                    .SetStopPrc( "0" )
                .Build();

                DraftAllocationData draftAllocation = DraftAllocationData.CreateBuilder()
                    .SetAccountId( account_position.AccountId )
                    .SetAllocationQty( Convert.ToString( instrument_position.OpenPosition ) )
                .Build();

                ClientAdapterResponse create_order_response = controller.SendRpc( new CreateOrder( draftOrder, draftAllocation ) );

                if( create_order_response.Type == ClientAdapterResponse.Types.ResponseType.ACKNOWLEDGEMENT )
                {
                    DealingEngineResponse dealing_order_response = create_order_response.DealingResponse;

                    if( dealing_order_response.Type == DealingEngineResponse.Types.ResponseType.ACKNOWLEDGEMENT )
                    {
                        //
                        //  Release the market order
                        //

                        foreach( OrderMessage order_msg in dealing_order_response.OrderList )
                        {
                            ClientAdapterResponse release_order_response = controller.SendRpc( new ReleaseOrder( order_msg.OrderData.OrderId ) );

                            if( release_order_response.Type == ClientAdapterResponse.Types.ResponseType.ACKNOWLEDGEMENT )
                            {
                                DealingEngineResponse dealing_release_response = release_order_response.DealingResponse;

                                if( dealing_release_response.Type != DealingEngineResponse.Types.ResponseType.ACKNOWLEDGEMENT )
                                {

                                }
                            }
                        }
                    }
                }
            }
        }



//
//			//
//			//  Send to FIX checkbox
//			//
//
//            sendToFix = new CheckBox();
//            sendToFix.AutoSize = true;
//            sendToFix.CausesValidation = false;
//            sendToFix.Checked = false;
//            sendToFix.Text = "Release to FIX Engine";
//            sendToFix.Location = new System.Drawing.Point( 10, 24 );
//            sendToFix.CheckStateChanged  += new EventHandler( FireSendToFixChanged );
//
//            //
//            //  flatten menu-items
//            //
//
//            toolStripMenuItem = new ToolStripMenuItem();
//            toolStripMenuItem.Name = "ALL";
//            toolStripMenuItem.Size = new System.Drawing.Size( 188, 22 );
//            toolStripMenuItem.Text = "Flatten ALL Open Positions";
//            toolStripMenuItem.Click += new EventHandler( FireFlattenAccount );
//
//            //
//            // toolStripSeparator1
//            //
//
//            toolStripSeparator = new ToolStripSeparator();
//            toolStripSeparator.Name = "toolStripSeparator";
//            toolStripSeparator.Size = new System.Drawing.Size( 185, 6 );
//
//            System.ComponentModel.IContainer components = new System.ComponentModel.Container();
//            splitButtonDropDown = new ContextMenuStrip( components );
//            splitButtonDropDown.Name = "splitButtonDropDown";
//            splitButtonDropDown.Size = new System.Drawing.Size( 189, 76 );
//
//            //System.ComponentModel.ComponentResourceManager resources =
//            //    new System.ComponentModel.ComponentResourceManager(typeof(SplitButton));
//
//            //
//            //System.Windows.Forms.ImageList splitButtonImages;
//            //splitButtonImages = new System.Windows.Forms.ImageList( components );
//            //splitButtonImages.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("SplitButtonImages.ImageStream")));
//            //splitButtonImages.TransparentColor = System.Drawing.Color.Transparent;
//            //splitButtonImages.Images.SetKeyName(0, "SplitButtonImage.gif");
//            //splitButtonImages.Images.SetKeyName(1, "SplitButtonHoverImage.gif");
//            //splitButtonImages.Images.SetKeyName(2, "SplitButtonClickedImage.gif");
//            //splitButtonImages.Images.SetKeyName(3, "SplitButtonDisabledImage.gif");
//
//            splitButton = new SplitButton();
//            splitButton.Enabled = false;
//            splitButton.AutoSize = true;
//            splitButton.AlwaysDropDown = true;
//            splitButton.Location = new System.Drawing.Point( 10, 52 );
//            //splitButton.ImageList = splitButtonImages;
//            //splitButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
//            splitButton.ClickedImage = "SplitButtonClickedImage.gif";
//            splitButton.ContextMenuStrip = splitButtonDropDown;
//            splitButton.DisabledImage = "SplitButtonDisabledImage.gif";
//            //splitButton.FocusedImage = "Focused";
//            splitButton.HoverImage = "SplitButtonHoverImage.gif";
//            splitButton.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
//            splitButton.ImageKey = "SplitButtonImage.gif";
//            splitButton.Name = "splitButton";
//            splitButton.NormalImage = "SplitButtonImage.gif";
//            splitButton.Size = new System.Drawing.Size(125, 26);
//            splitButton.TabIndex = 0;
//            splitButton.Text = "Flatten Positions";
//            splitButton.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
//            splitButton.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
//            splitButton.UseVisualStyleBackColor = true;
//
//            GroupBox statusGroupBox = new GroupBox();
//            statusGroupBox.Dock = DockStyle.Top;
//            statusGroupBox.AutoSize = true;
//            statusGroupBox.FlatStyle = FlatStyle.Standard;
//            statusGroupBox.Text = "Dealing Status";
//            statusGroupBox.Controls.AddRange( new Control [] {
//                sendToFix,
//                splitButton
//            } );
//
//            Panel spacer1 = new Panel();
//            spacer1.Size = new System.Drawing.Size( 125, 12 );
//            spacer1.Dock = DockStyle.Top;
//
//            Panel spacer2 = new Panel();
//            spacer2.Size = new System.Drawing.Size( 125, 12 );
//            spacer2.Dock = DockStyle.Top;
//
//            this.Controls.AddRange( new Control[] {
//                accounts,
//                spacer1,
//                //investmentSystems,
//                //spacer2,
//                statusGroupBox
//            } );
//
//            //this.Controls.Add( flowLayoutPanel );
//        }
//
//        private void FireFlattenAccount( object sender, EventArgs e )
//        {
//            ToolStripMenuItem tsmi = (ToolStripMenuItem)sender;
//
//            log.Debug( "FireFlattenAccount: " + tsmi.Name );
//
//            string message = null;
//            if( "ALL".Equals( tsmi.Name ) )
//            {
//                message = "Are you sure you want to flatten ALL open positions?";
//            }
//            else
//            {
//                message = "Are you sure you want to flatten open positions for account:\n" + tsmi.Text;
//            }
//
//            DialogResult result = MessageBox.Show(
//			    message,
//			    "Flatten Positions Confirmation",
//			    MessageBoxButtons.YesNo,
//			    MessageBoxIcon.Exclamation );
//
//			if( result == System.Windows.Forms.DialogResult.Yes )
//			{
//			    XmlDocument doc = new XmlDocument();
//    			doc.LoadXml( "<bdsq></bdsq>" );
//
//    			XmlElement dealingEngineRequest = doc.CreateElement( "DealingEngineRequest" );
//    			dealingEngineRequest.SetAttribute( "id", "FlattenPositions" );
//                doc.DocumentElement.AppendChild( dealingEngineRequest );
//
//                if( "ALL".Equals( tsmi.Name ) )
//                {
//                    foreach( GTLTreeNode accountNode in accounts.Nodes )
//                    {
//                        //
//                        //  Get the account
//                        //
//
//                        AccountData accountData = (AccountData)accountNode.Tag;
//
//                        XmlElement account = doc.CreateElement( "Account" );
//                        account.SetAttribute( "account_id", accountData.Id );
//                        dealingEngineRequest.AppendChild( account );
//                    }
//                }
//                else
//                {
//                    XmlElement account = doc.CreateElement( "Account" );
//                    account.SetAttribute( "account_id", tsmi.Name );
//                    dealingEngineRequest.AppendChild( account );
//                }
//
//    			controller.Dispatcher.SendRpc( doc, MessageEventArgs.DEALING_CALLFORWARD );
//            }
//        }
//
//        private void FireBeforeCellEdit( object source, GTLEmbeddedControlEventArgs args )
//		{
//
//		}
//
//        private void FireAfterCellEdit( object source, GTLEmbeddedControlEventArgs args )
//		{
//
//		}
//
//        private void FireFlattenButton( object o, EventArgs e )
//        {
//
//        }
//
//        private void FireSendToFixChanged( object o, EventArgs e )
//        {
//            if( ignoreSendToFixEvent )
//            {
//                ignoreSendToFixEvent = false;
//                return;
//            }
//
//            XmlDocument doc = null;
//            XmlElement dealingEngineRequest = null;
//
//            if(! sendToFix.Checked )
//            {
//                //
//                //  Display 'are you sure' message box...
//                //
//
//    			DialogResult result = MessageBox.Show(
//    			    "Are you sure you want to stop sending orders to the FIX Engine?",
//    			    "Stop Sending to FIX Confirmation",
//    			    MessageBoxButtons.YesNo,
//    			    MessageBoxIcon.Exclamation );
//
//    			if( result == System.Windows.Forms.DialogResult.No )
//    			{
//                    //
//                    //  Cancel the event
//                    //
//
//                    ignoreSendToFixEvent = true;
//                    sendToFix.Checked = true;
//    			}
//    			else
//    			{
//
//    			    doc = new XmlDocument();
//        			doc.LoadXml( "<bdsq></bdsq>" );
//
//        			dealingEngineRequest = doc.CreateElement( "DealingEngineRequest" );
//        			dealingEngineRequest.SetAttribute( "id", "UpdateStatus" );
//                    doc.DocumentElement.AppendChild( dealingEngineRequest );
//
//                    XmlElement dealingEngineParameters =
//                        doc.CreateElement( "DealingEngineParameters" );
//                    dealingEngineParameters.SetAttribute( "send_to_fix", "0" );
//                    dealingEngineRequest.AppendChild( dealingEngineParameters );
//
//        			controller.Dispatcher.SendRpc( doc, MessageEventArgs.DEALING_CALLFORWARD );
//                }
//            }
//            else
//            {
//                //
//                //  Display new release to fix control...
//                //
//
//                ReleaseToFixForm releaseToFix = new ReleaseToFixForm();
//                releaseToFix.Size = new Size( 400, 300 );
//                releaseToFix.FormBorderStyle = FormBorderStyle.FixedDialog;
//                releaseToFix.StartPosition = FormStartPosition.CenterScreen;
//
//                if( releaseToFix.ShowDialog( this ) == DialogResult.OK )
//                {
//                    doc = new XmlDocument();
//        			doc.LoadXml( "<bdsq></bdsq>" );
//
//        			dealingEngineRequest = doc.CreateElement( "DealingEngineRequest" );
//        			dealingEngineRequest.SetAttribute( "id", "UpdateStatus" );
//                    doc.DocumentElement.AppendChild( dealingEngineRequest );
//
//                    XmlElement dealingEngineParameters =
//                        doc.CreateElement( "DealingEngineParameters" );
//                    dealingEngineParameters.SetAttribute( "send_to_fix", "1" );
//                    dealingEngineRequest.AppendChild( dealingEngineParameters );
//
//        			controller.Dispatcher.SendRpc( doc, MessageEventArgs.DEALING_CALLFORWARD );
//
//        			if( releaseToFix.AutoSelected )
//        			{
//        			    foreach( DictionaryEntry de in DealingEngineViewer.ORDERS )
//        			    {
//                            OrderData orderData = (OrderData)de.Value;
//                            if( "NEW".Equals( orderData.OrderStatus() ) )
//                            {
//                                doc = new XmlDocument();
//                    			doc.LoadXml( "<bdsq></bdsq>" );
//
//                    			dealingEngineRequest = doc.CreateElement( "DealingEngineRequest" );
//                    			dealingEngineRequest.SetAttribute( "id", "ReleaseOrder" );
//                                doc.DocumentElement.AppendChild( dealingEngineRequest );
//
//                                XmlElement order = doc.CreateElement( "Order" );
//                                order.SetAttribute( "order_id", orderData.Id );
//                                dealingEngineRequest.AppendChild( order );
//
//                    			controller.Dispatcher.SendRpc( doc, MessageEventArgs.DEALING_CALLFORWARD );
//                            }
//        			    }
//                    }
//                }
//                else
//                {
//                    ignoreSendToFixEvent = true;
//                    sendToFix.Checked = false;
//                }
//            }
//
//            splitButton.Enabled = sendToFix.Checked;
//        }
//
//        private void FireMarketDataUpdate( object sender, MessageEventArgs args )
//		{
//            this.SuspendLayout();
//
//            accounts.BeginUpdate();
//            accounts.BeginNodeListChange();
//
//            foreach( XmlNode instrument_node in args.Node.ChildNodes )
//            {
//                InstrumentData instrumentData = new InstrumentData( instrument_node );
//                double last_px = instrumentData.ToDouble( DatabaseObject.LAST_PRC );
//
//                //
//                //  For each account...
//                //
//
//                foreach( GTLTreeNode accountNode in accounts.Nodes )
//                {
//                    //
//                    //  Get the account
//                    //
//
//                    AccountData accountData = (AccountData)accountNode.Tag;
//                    double notional = accountData.ToDouble( DatabaseObject.NOTIONAL_VAL );
//
//                    ProfitLossData aggData = new ProfitLossData( accountData.Id );
//
//                    //
//                    //  Find this instrument
//                    //
//
//                    if( positions.ContainsKey( accountData.Id ) )
//                    {
//                        ArrayList list = (ArrayList)positions[ accountData.Id ];
//                        foreach( ProfitLossData plData in list )
//                        {
//                            if( plData.InstrumentId.Equals( instrumentData.Id ) &&
//                                plData.LastPx != last_px )
//                            {
//                                plData.Update( last_px );
//                            }
//
//                            aggData.Aggregate( plData );
//                        }
//                    }
//
//                    //
//                    //  Update the total pct pl
//                    //
//
//                    double totalPctPL = notional == 0 ? 0.0 : ( aggData.TotalPL / notional ) * 100;
//                    ((PercentSubItem)accountNode.SubItems[ "current_pl" ]).Update( totalPctPL );
//                }
//            }
//
//            accounts.EndNodeListChange();
//		    accounts.EndUpdate();
//			this.ResumeLayout( false );
//			this.PerformLayout();
//		}
//
//        private void FireFlattenPositions( object sender, MessageEventArgs args )
//		{
//             XmlNode node = args.Node;
//
//            foreach( XmlNode position_node in node.SelectNodes( "OpenPosition" ) )
//            {
//                //
//                //  Create an offsetting order
//                //
//
//                double order_qty = -1 * Convert.ToDouble( position_node.Attributes[ "open_qty" ].Value );
//                string instrument_id = position_node.Attributes[ "instrument_id" ].Value;
//                string account_id = position_node.Attributes[ "account_id" ].Value;
//
//                XmlDocument doc = new XmlDocument();
//                doc.LoadXml( "<bdsq></bdsq>" );
//
//                XmlElement dealingEngineRequest = doc.CreateElement( "DealingEngineRequest" );
//                dealingEngineRequest.SetAttribute( "id", "CreateOrder" );
//                doc.DocumentElement.AppendChild( dealingEngineRequest );
//
//                XmlElement draftOrder = doc.CreateElement( "DraftOrder" );
//                draftOrder.SetAttribute( "tif", "day" );
//                draftOrder.SetAttribute( "order_type", "market" );
//                draftOrder.SetAttribute( "order_qty", order_qty.ToString() );
//                draftOrder.SetAttribute( "investment_system_id", "CLIENT" );
//                draftOrder.SetAttribute( "instrument_id", instrument_id );
//
//                XmlElement draftAllocation = doc.CreateElement( "DraftAllocation" );
//                draftAllocation.SetAttribute( "order_qty", order_qty.ToString() );
//                draftAllocation.SetAttribute( "account_id", account_id );
//
//                draftOrder.AppendChild( draftAllocation );
//                dealingEngineRequest.AppendChild( draftOrder );
//
//                controller.Dispatcher.SendRpc( doc, MessageEventArgs.DEALING_CALLFORWARD );
//            }
//		}
//
//        private void FireStatusUpdate( object sender, MessageEventArgs args )
//		{
//		    XmlNode node = args.Node;
//
//            foreach( XmlNode status_node in node.SelectNodes( "DealingEngineParameters" ) )
//            {
//                //
//                //  Is the DE sending to FIX?
//                //
//
//                if( sendToFix.Checked && "0".Equals( status_node.Attributes[ "send_to_fix" ].Value ) )
//                {
//                    ignoreSendToFixEvent = true;
//                    sendToFix.Checked = false;
//                }
//
//                if( !sendToFix.Checked && "1".Equals( status_node.Attributes[ "send_to_fix" ].Value ) )
//                {
//                    ignoreSendToFixEvent = true;
//                    sendToFix.Checked = true;
//                }
//
//                splitButton.Enabled = sendToFix.Checked;
//            }
//        }
//
//        private void FireLoadPositions( object sender, MessageEventArgs args )
//		{
//		    XmlNode node = args.Node;
//
//		    this.SuspendLayout();
//
//		    accounts.Nodes.Clear();
//            positions.Clear();
//            splitButtonDropDown.Items.Clear();
//
//		    splitButtonDropDown.Items.AddRange(
//		        new ToolStripItem[] {
//                    toolStripMenuItem,
//                    toolStripSeparator
//            } );
//
//            foreach( XmlNode account_node in node.ChildNodes )
//            {
//                //
//                //  There is one node per account...
//                //
//
//                AddPositionIntraday( account_node );
//            }
//
//			this.ResumeLayout( false );
//			this.PerformLayout();
//		}
//
//		private void FireUpdatePositions( object sender, MessageEventArgs args )
//		{
//		    XmlNode node = args.Node;
//
//		    this.SuspendLayout();
//
//		    accounts.BeginUpdate();
//            accounts.BeginNodeListChange();
//            accounts.Nodes.Clear();
//            positions.Clear();
//
//            splitButtonDropDown.Items.Clear();
//            splitButtonDropDown.Items.AddRange(
//                new ToolStripItem[] {
//                    toolStripMenuItem,
//                    toolStripSeparator
//            } );
//
//            foreach( XmlNode account_node in node.ChildNodes )
//            {
//                //
//                //  There is one node per account...
//                //
//
//                AddPositionIntraday( account_node );
//            }
//
//            accounts.EndNodeListChange();
//		    accounts.EndUpdate();
//			this.ResumeLayout( false );
//			this.PerformLayout();
//		}
//
//        private void AddPositionIntraday( XmlNode account_node )
//        {
//		    XmlDocument account = new XmlDocument();
//
//		    AccountData accountData = null;
//		    InstrumentData instrumentData = null;
//
//		    string account_id = account_node.Attributes[ DatabaseObject.ACCOUNT_ID ].Value;
//
//            string accountXml = controller.GetDirect(
//                "account::account_id::" + account_id );
//
//            account.LoadXml( accountXml );
//            accountData = new AccountData( account.DocumentElement );
//            double notional = accountData.ToDouble( DatabaseObject.NOTIONAL_VAL );
//
//            ProfitLossData aggData = new ProfitLossData( account_id );
//
//            if(! positions.ContainsKey( account_id ) )
//            {
//                positions[ account_id ] = new ArrayList();
//            }
//
//            //
//            //  Loop through each instrument, calculating the PL
//            //
//
//            foreach( XmlNode pl_node in account_node.ChildNodes )
//            {
//                XmlDocument instrument = new XmlDocument();
//
//                string instrument_id = pl_node.Attributes[
//                    DatabaseObject.INSTRUMENT_ID ].Value;
//
//                string instrumentXml = controller.GetDirect(
//                    "instrument::instrument_id::" + instrument_id );
//
//                instrument.LoadXml( instrumentXml );
//                instrumentData = new InstrumentData( instrument.DocumentElement );
//
//                double last_px = instrumentData.ToDouble( DatabaseObject.LAST_PRC );
//                double multiplier = instrumentData.ToDouble( DatabaseObject.MULTIPLIER );
//
//                ProfitLossData plData = new ProfitLossData(
//                    account_id, pl_node, last_px, multiplier );
//
//                aggData.Aggregate( plData );
//
//                //
//                //  Save the PLData
//                //
//
//                ((ArrayList)positions[ account_id ]).Add( plData );
//            }
//
//            double totalPctPL = notional == 0 ? 0.0 : ( aggData.TotalPL / notional ) * 100;
//
//            GTLTreeNode node = new GTLTreeNode( accountData[ DatabaseObject.NAME ] );
//            node.Tag = accountData;
//            node.SubItems.AddRange( new GTLSubItem [] {
//                new PercentSubItem( totalPctPL, true ),
//                new GTLSubItem( aggData.OpenPos.ToString() )
//            } );
//
//            accounts.BeginUpdate();
//            accounts.BeginNodeListChange();
//            accounts.Nodes.Add( node );
//            accounts.EndNodeListChange();
//		    accounts.EndUpdate();
//
//		    ToolStripMenuItem tsmi = new ToolStripMenuItem();
//		    tsmi.Name = accountData.Id;
//            tsmi.Size = new System.Drawing.Size( 188, 22 );
//            tsmi.Text = accountData[ DatabaseObject.NAME ];
//            tsmi.Click += new EventHandler( FireFlattenAccount );
//            splitButtonDropDown.Items.Add( tsmi );
//        }
    }
}
