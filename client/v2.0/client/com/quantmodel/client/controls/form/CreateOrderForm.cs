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

using GlacialComponents.Controls.GlacialTreeList;
using GlacialComponents.Controls.Common;
using GlacialComponents.Controls.GTLCommon;

using quantmodel;
using com.quantmodel.common.data;
using com.quantmodel.common.events;
using com.quantmodel.common.controls;

namespace com.quantmodel.client.controls.form
{
	public class CreateOrderForm : Form
	{
	    private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( CreateOrderForm ) );

	    private Panel buttonPanel = null;
	    private Panel headerPanel = null;
	    private Panel mainPanel = null;

	    private Instrument selectedInstrument = null;
	    private InvestmentSystem selectedInvestmentSystem = null;

        private readonly ClientController controller;
        private readonly Button buyButton = new Button();
        private readonly Button sellButton = new Button();
        private readonly TabControl tabControl = new TabControl();

        private readonly GlacialTreeList strategies =
            new GlacialTreeList( "0DAA8D90BB9ACC06C6FDDF11F7DD73E8" );

        private readonly GlacialTreeList accounts =
            new GlacialTreeList( "0DAA8D90BB9ACC06C6FDDF11F7DD73E8" );

	    private SplitButton investmentSystemButton = null;
        private ContextMenuStrip investmentSystemButtonDropDown = null;

        private SplitButton instrumentButton = null;
        private ContextMenuStrip instrumentButtonDropDown = null;

        private SplitButton symbolButton = null;
        private ContextMenuStrip symbolButtonDropDown = null;

        private readonly TextBox stopPriceTextBox = new TextBox();
        private readonly TextBox limitPriceTextBox = new TextBox();
        private readonly CheckBox releaseToFixCheckBox = new CheckBox();
        private readonly ComboBox tifComboBox = new ComboBox();
        private readonly ComboBox orderTypeComboBox = new ComboBox();

    	public CreateOrderForm( ClientController controller ) : base()
		{
            this.controller = controller;
            this.Font = new System.Drawing.Font( "Tahoma", 9.25F );

		    InitializeComponents();
		    this.Visible = false;

		    this.VisibleChanged += new EventHandler( FireVisibleChanged );
		    this.FormClosing += new FormClosingEventHandler( FireFormClosing );
		    tabControl.SelectedIndexChanged += new EventHandler( FireTabSelected );
		    orderTypeComboBox.SelectedIndexChanged += new EventHandler( FireOrderTypeChanged );
        }

        private void InitializeComponents()
    	{
    	    Label headerLabel = new Label();
			headerLabel.Location = new System.Drawing.Point( 10, 10 );
			headerLabel.Name = "headerLabel";
			headerLabel.Size = new System.Drawing.Size( 360, 20 );
			headerLabel.TabIndex = 3;
			headerLabel.Text = "Create Order:";
			headerLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			headerLabel.Font = new System.Drawing.Font(
			    "Tahoma", 10.25F, System.Drawing.FontStyle.Bold,
			        System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));


            buyButton.Size = new System.Drawing.Size( 84, 26 );
            buyButton.Text = "BUY";
            buyButton.TabIndex = 1;
            buyButton.TabStop = true;
            buyButton.Location = new System.Drawing.Point( 180, 10 );
            buyButton.Click += new EventHandler( FireExecuteButton );
            buyButton.Anchor = AnchorStyles.Right;
            buyButton.Enabled = false;

            sellButton.Size = new System.Drawing.Size( 84, 26 );
            sellButton.Text = "SELL";
            sellButton.TabIndex = 1;
            sellButton.TabStop = true;
            sellButton.Location = new System.Drawing.Point( 270, 10 );
            sellButton.Click += new EventHandler( FireExecuteButton );
            sellButton.Anchor = AnchorStyles.Right;
            sellButton.Enabled = false;

            Button cancel = new Button();
            cancel.Text = "Cancel";
            cancel.Size = new System.Drawing.Size( 84, 26 );
            cancel.Location = new System.Drawing.Point( 360, 10 );
            cancel.Click += new EventHandler( FireCancelButton );
            cancel.Anchor = AnchorStyles.Right;

            //
			//  Release to FIX
			//

			releaseToFixCheckBox.Text = "Release to FIX";
			releaseToFixCheckBox.AutoSize = true;
			releaseToFixCheckBox.Location = new System.Drawing.Point( 60, 16 );
            releaseToFixCheckBox.Name = "releaseToFix";
            releaseToFixCheckBox.Anchor = AnchorStyles.Right;
            releaseToFixCheckBox.Checked = false;

			//
            //  Strategy List
            //

            strategies.BackColor = System.Drawing.SystemColors.Window;
			strategies.BackgroundStretchToFit = true;
			strategies.Dock = System.Windows.Forms.DockStyle.Fill;
            strategies.ItemHeight = 19;
            strategies.Location = new System.Drawing.Point( 10, 80 );
            strategies.MultiSelect = true;
            strategies.ShowPlusMinus = false;
            strategies.HotColumnTracking = false;
            strategies.HotNodeTracking = false;
            strategies.SnapLastColumn = true;
            strategies.AlternateBackground = Color.AliceBlue;
            strategies.ShowRootLines = false;
            strategies.ShowFocusRect = true;
            strategies.FastFind = true;
            strategies.AlternatingColors = false;
            strategies.Name = "accounts";
            strategies.OptimizeForHighNodeCount = false;
            strategies.HotTrackingColor = SystemColors.HotTrack;
            strategies.SelectedTextColor = SystemColors.HighlightText;
            strategies.SelectionColor =  SystemColors.Highlight;
            strategies.UnfocusedSelectionColor =  SystemColors.Highlight;
            strategies.ControlStyle = GTLControlStyles.XP;
            strategies.Font = new System.Drawing.Font( "Tahoma", 9.25F );

            GTLColumn strategy_name = new GTLColumn();
            strategy_name.BackColor = Color.White;
            strategy_name.CheckBoxes = false;
			strategy_name.ImageIndex = -1;
			strategy_name.Name = "strategy_name";
			strategy_name.Text = "Strategy Name";
			strategy_name.Width = 475;
			strategy_name.SortType = SortTypes.None;

			GTLColumn strategy_qty = new GTLColumn();
            strategy_qty.BackColor = Color.White;
            strategy_qty.CheckBoxes = false;
			strategy_qty.ImageIndex = -1;
			strategy_qty.Name = "strategy_qty";
			strategy_qty.Text = "Signal Size";
			strategy_qty.Width = 125;
			strategy_qty.TextAlignment = ContentAlignment.MiddleRight;
			strategy_qty.EmbeddedDisplayConditions = GEmbeddedDisplayConditions.SingleClick;
			strategy_qty.EmbeddedControlType = GEmbeddedControlTypes.TextBox;
			strategy_qty.SortType = SortTypes.None;

			strategies.Columns.AddRange(
			    new GTLColumn [] {
                    strategy_name,
                    strategy_qty } );

            //
            //  Account List
            //

            accounts.BackColor = System.Drawing.SystemColors.Window;
			accounts.BackgroundStretchToFit = true;
			accounts.Dock = System.Windows.Forms.DockStyle.Fill;
            accounts.ItemHeight = 19;
            accounts.Location = new System.Drawing.Point( 10, 80 );
            accounts.MultiSelect = true;
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

            GTLColumn account_name = new GTLColumn();
            account_name.BackColor = Color.White;
            account_name.CheckBoxes = false;
			account_name.ImageIndex = -1;
			account_name.Name = "account_name";
			account_name.Text = "Account Name";
			account_name.Width = 475;
			account_name.SortType = SortTypes.None;

			GTLColumn allocation_qty = new GTLColumn();
            allocation_qty.BackColor = Color.White;
            allocation_qty.CheckBoxes = false;
			allocation_qty.ImageIndex = -1;
			allocation_qty.Name = "allocation_qty";
			allocation_qty.Text = "Allocation Qty";
			allocation_qty.Width = 125;
			allocation_qty.TextAlignment = ContentAlignment.MiddleRight;
			allocation_qty.EmbeddedDisplayConditions = GEmbeddedDisplayConditions.SingleClick;
			allocation_qty.EmbeddedControlType = GEmbeddedControlTypes.TextBox;
			allocation_qty.SortType = SortTypes.None;

			accounts.Columns.AddRange(
			    new GTLColumn [] {
                    account_name,
                    allocation_qty } );

            //
			//  Context Menu
			//

			strategies.ContextMenu = new ContextMenu();
			strategies.ContextMenu.MenuItems.AddRange( new MenuItem []
			{
			    new MenuItem( "Clear...", new EventHandler( FireClear )  )
            } );

            //
            //  Disable until instrument selected
            //

            accounts.Enabled = false;
            strategies.Enabled = false;

			//
			//  Tab Pages
			//

			TabPage strategyPage = new TabPage();
			strategyPage.Text = "Strategies";
			strategyPage.Dock = DockStyle.Fill;
			strategyPage.Controls.Add( strategies );

			TabPage accountPage = new TabPage();
			accountPage.Text = "Accounts";
			accountPage.Dock = DockStyle.Fill;
			accountPage.Controls.Add( accounts );

            tabControl.Dock = DockStyle.Fill;
            tabControl.Size = new System.Drawing.Size( 200, 40 );
            tabControl.Name = "tabControl";
            tabControl.Controls.AddRange( new Control [] {
                strategyPage,
                accountPage
            } );

            //
            //  Investment system button drop down
            //

            investmentSystemButtonDropDown = new ContextMenuStrip( new System.ComponentModel.Container() );
            investmentSystemButtonDropDown.Name = "investmentSystemButtonDropDown";
            investmentSystemButtonDropDown.Size = new System.Drawing.Size( 189, 76 );

            //
            //  Investment system button
            //

            investmentSystemButton = new SplitButton();
            investmentSystemButton.AutoSize = true;
            investmentSystemButton.AlwaysDropDown = true;
            investmentSystemButton.Location = new System.Drawing.Point( 10, 40 );
            investmentSystemButton.ClickedImage = "SplitButtonClickedImage.gif";
            investmentSystemButton.ContextMenuStrip = investmentSystemButtonDropDown;
            investmentSystemButton.DisabledImage = "SplitButtonDisabledImage.gif";
            investmentSystemButton.HoverImage = "SplitButtonHoverImage.gif";
            investmentSystemButton.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            investmentSystemButton.ImageKey = "SplitButtonImage.gif";
            investmentSystemButton.Name = "investmentSystemButton";
            investmentSystemButton.NormalImage = "SplitButtonImage.gif";
            investmentSystemButton.Size = new System.Drawing.Size( 250, 26 );
            investmentSystemButton.TabIndex = 0;
            investmentSystemButton.Text = "Investment System...";
            investmentSystemButton.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            investmentSystemButton.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
            investmentSystemButton.UseVisualStyleBackColor = true;

            //
            //  Symbol button drop down
            //

            symbolButtonDropDown = new ContextMenuStrip( new System.ComponentModel.Container() );
            symbolButtonDropDown.Name = "instrumentButtonDropDown";
            symbolButtonDropDown.Size = new System.Drawing.Size( 189, 76 );

            //
            //  Symbol button
            //

            symbolButton = new SplitButton();
            symbolButton.AutoSize = true;
            symbolButton.AlwaysDropDown = true;
            symbolButton.Location = new System.Drawing.Point( 10, 74 );
            symbolButton.ClickedImage = "SplitButtonClickedImage.gif";
            symbolButton.ContextMenuStrip = symbolButtonDropDown;
            symbolButton.DisabledImage = "SplitButtonDisabledImage.gif";
            symbolButton.HoverImage = "SplitButtonHoverImage.gif";
            symbolButton.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            symbolButton.ImageKey = "SplitButtonImage.gif";
            symbolButton.Name = "symbolButton";
            symbolButton.NormalImage = "SplitButtonImage.gif";
            symbolButton.Size = new System.Drawing.Size( 250, 26 );
            symbolButton.TabIndex = 0;
            symbolButton.Text = "Symbol...";
            symbolButton.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            symbolButton.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
            symbolButton.UseVisualStyleBackColor = true;

            //
            //  Instrument button drop down
            //

            instrumentButtonDropDown = new ContextMenuStrip( new System.ComponentModel.Container() );
            instrumentButtonDropDown.Name = "instrumentButtonDropDown";
            instrumentButtonDropDown.Size = new System.Drawing.Size( 189, 76 );

            //
            //  Instrument button
            //

            instrumentButton = new SplitButton();
            instrumentButton.AutoSize = true;
            instrumentButton.AlwaysDropDown = true;
            instrumentButton.Location = new System.Drawing.Point( 10, 108 );
            instrumentButton.ClickedImage = "SplitButtonClickedImage.gif";
            instrumentButton.ContextMenuStrip = instrumentButtonDropDown;
            instrumentButton.DisabledImage = "SplitButtonDisabledImage.gif";
            instrumentButton.HoverImage = "SplitButtonHoverImage.gif";
            instrumentButton.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            instrumentButton.ImageKey = "SplitButtonImage.gif";
            instrumentButton.Name = "instrumentButton";
            instrumentButton.NormalImage = "SplitButtonImage.gif";
            instrumentButton.Size = new System.Drawing.Size( 250, 26 );
            instrumentButton.TabIndex = 0;
            instrumentButton.Text = "Instrument...";
            instrumentButton.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            instrumentButton.TextImageRelation = System.Windows.Forms.TextImageRelation.TextBeforeImage;
            instrumentButton.UseVisualStyleBackColor = true;

            //
            //  Stop price
            //

            Label stopPriceLabel = new Label();
            stopPriceLabel.AutoSize = true;
            stopPriceLabel.Location = new System.Drawing.Point( 280, 52 );
            stopPriceLabel.Name = "stopPriceLabel";
            stopPriceLabel.Text = "Stop Price:";
            stopPriceLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;

            stopPriceTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			stopPriceTextBox.Name = "stopPriceTextBox";
			stopPriceTextBox.Size = new System.Drawing.Size( 80, 22 );
			stopPriceTextBox.Location = new System.Drawing.Point( 280, 76 );
			stopPriceTextBox.TextAlign = HorizontalAlignment.Right;
			stopPriceTextBox.TabIndex = 6;
			stopPriceTextBox.MaxLength = 8;
			stopPriceTextBox.Text = "";
			stopPriceTextBox.Enabled = false;

			//
            //  Limit price
            //

            Label limitPriceLabel = new Label();
            limitPriceLabel.AutoSize = true;
            limitPriceLabel.Location = new System.Drawing.Point( 370, 52 );
            limitPriceLabel.Name = "limitPriceLabel";
            limitPriceLabel.Text = "Limit Price:";
            limitPriceLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;

            limitPriceTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			limitPriceTextBox.Name = "limitPriceTextBox";
			limitPriceTextBox.Size = new System.Drawing.Size( 80, 22 );
			limitPriceTextBox.Location = new System.Drawing.Point( 370, 76 );
			limitPriceTextBox.TextAlign = HorizontalAlignment.Right;
			limitPriceTextBox.TabIndex = 6;
			limitPriceTextBox.MaxLength = 8;
			limitPriceTextBox.Text = "";
			limitPriceTextBox.Enabled = false;

			//
			//  TIF
			//

			Label tifLabel = new Label();
            tifLabel.AutoSize = true;
            tifLabel.Location = new System.Drawing.Point( 460, 52 );
            tifLabel.Name = "tifLabel";
            tifLabel.Text = "TIF:";
            tifLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;

            tifComboBox.Name = "tifComboBox";
            tifComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            tifComboBox.Size = new System.Drawing.Size( 60, 22 );
            tifComboBox.Location = new System.Drawing.Point( 460, 76 );
            tifComboBox.Items.AddRange( new object [] { "DAY", "GTC" } );
            tifComboBox.SelectedIndex = 0;

            //
			//  Order Type -- note that Globex doesn't support MOC orders, so
			//  until we have a way of differentiating order type by instrument,
			//  we're not going to include this order type in the dropdown.
			//

			Label orderTypeLabel = new Label();
            orderTypeLabel.AutoSize = true;
            orderTypeLabel.Location = new System.Drawing.Point( 530, 52 );
            orderTypeLabel.Name = "orderTypeLabel";
            orderTypeLabel.Text = "Order Type:";
            orderTypeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;

            orderTypeComboBox.Name = "orderTypeComboBox";
            orderTypeComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            orderTypeComboBox.Size = new System.Drawing.Size( 90, 22 );
            orderTypeComboBox.Location = new System.Drawing.Point( 530, 76 );
            //orderTypeComboBox.Items.AddRange( new object [] { "MARKET", "LIMIT", "STOP", "STOP LIMIT", "MKT ON CLOSE" } );
            orderTypeComboBox.Items.AddRange( new object [] { "MARKET", "LIMIT", "STOP", "STOP LIMIT" } );
            orderTypeComboBox.SelectedIndex = 0;

			//
			//  Form panels
			//

            mainPanel = new Panel();
	        mainPanel.BackColor = System.Drawing.SystemColors.Window;
			mainPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			mainPanel.Name = "mainPanel";
			mainPanel.Size = new System.Drawing.Size( 460, 360 );

			mainPanel.Controls.AddRange( new Control [] { tabControl } );

			headerPanel = new Panel();
	        headerPanel.BackColor = System.Drawing.SystemColors.Window;
			headerPanel.Dock = System.Windows.Forms.DockStyle.Top;
			headerPanel.Name = "headerPanel";
			headerPanel.Size = new System.Drawing.Size( 460, 150 );
			headerPanel.TabIndex = 8;
			headerPanel.Controls.AddRange( new Control[] {
			  headerLabel,
			  investmentSystemButton,
			  symbolButton,
			  instrumentButton,
			  stopPriceLabel,
			  stopPriceTextBox,
			  limitPriceLabel,
			  limitPriceTextBox,
			  tifLabel,
			  tifComboBox,
			  orderTypeLabel,
			  orderTypeComboBox
			} );

			buttonPanel = new Panel();
	        buttonPanel.BackColor = System.Drawing.SystemColors.Control;
			buttonPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
			buttonPanel.Name = "buttonPanel";
			buttonPanel.Size = new System.Drawing.Size( 460, 50 );
			buttonPanel.Controls.AddRange( new Control[] {
			  releaseToFixCheckBox,
			  buyButton,
			  sellButton,
			  cancel
			} );

            this.Controls.AddRange( new Control [] {
                mainPanel,
                headerPanel,
                buttonPanel
            } );
	    }

	    private void FireTabSelected( object o, EventArgs args )
        {
            if( tabControl.SelectedIndex == 0 )
            {
                investmentSystemButton.Enabled = true;
            }
            else
            {
                investmentSystemButton.Enabled = false;
            }
        }

	    private void FireOrderTypeChanged( object o, EventArgs args )
        {
            if( "MARKET".Equals( orderTypeComboBox.Text ) )
            {
                limitPriceTextBox.Text = "";
                limitPriceTextBox.Enabled = false;

                stopPriceTextBox.Text = "";
                stopPriceTextBox.Enabled = false;

                return;
            }

            if( "LIMIT".Equals( orderTypeComboBox.Text ) )
            {
                limitPriceTextBox.Enabled = true;

                stopPriceTextBox.Text = "";
                stopPriceTextBox.Enabled = false;

                return;
            }

            if( "STOP".Equals( orderTypeComboBox.Text ) )
            {
                limitPriceTextBox.Text = "";
                limitPriceTextBox.Enabled = false;

                stopPriceTextBox.Enabled = true;

                return;
            }

            if( "STOP LIMIT".Equals( orderTypeComboBox.Text ) )
            {
                limitPriceTextBox.Enabled = true;
                stopPriceTextBox.Enabled = true;

                return;
            }

            if( "MKT ON CLOSE".Equals( orderTypeComboBox.Text ) )
            {
                limitPriceTextBox.Text = "";
                limitPriceTextBox.Enabled = false;

                stopPriceTextBox.Text = "";
                stopPriceTextBox.Enabled = false;

                return;
            }
        }

	    private void FireVisibleChanged( object o, EventArgs args )
        {
            if( this.Visible )
            {
                //
                //  Load the investment systems
                //

                foreach( InvestmentSystem investmentSystem in controller.Cache.InvestmentSystems )
                {
                    ToolStripMenuItem tsmi = new ToolStripMenuItem();
                    tsmi.Name = investmentSystem[ DatabaseObject.INVESTMENT_SYSTEM_ID ];
                    tsmi.Text = investmentSystem[ DatabaseObject.INVESTMENT_SYSTEM_ID ];
                    tsmi.Size = new System.Drawing.Size( 188, 22 );
                    tsmi.Click += new EventHandler( FireInvestmentSystemSelected );
                    tsmi.Tag = investmentSystem;
                    investmentSystemButtonDropDown.Items.Add( tsmi );
                }

                //
                //  Load the symbols
                //

                ArrayList symbols = new ArrayList();

                foreach( Instrument instrument in controller.Cache.Instruments )
                {
                    if(! symbols.Contains( instrument[ DatabaseObject.SYMBOL ] ) )
                    {
                        symbols.Add( instrument[ DatabaseObject.SYMBOL ] );
                    }
                }

                symbols.Sort();

                foreach( string symbol in symbols )
                {
                    ToolStripMenuItem tsmi = new ToolStripMenuItem();
                    tsmi.Name = symbol;
                    tsmi.Text = symbol;
        	        tsmi.Size = new System.Drawing.Size( 188, 22 );
                    tsmi.Click += new EventHandler( FireSymbolSelected );
                    tsmi.Tag = symbol;
                    symbolButtonDropDown.Items.Add( tsmi );
                }

                //
                //  Load the active accounts
                //

                foreach( Account account in controller.Cache.Accounts )
                {
                    if( "1".Equals( account[ DatabaseObject.IS_ACTIVE ] ) )
                    {
                        GTLTreeNode accountNode = new GTLTreeNode();
                        accountNode.SubItems.AddRange( new GTLSubItem [] {
                            new GTLSubItem( account[ DatabaseObject.NAME ] ),
                            new GTLSubItem( "0" )
                        } );

        		        accountNode.Tag = account.Id;
        		        accounts.Nodes.Add( accountNode );
    		        }
    		    }
            }
            else
            {
                investmentSystemButton.Text = "Investment System...";
                investmentSystemButtonDropDown.Items.Clear();

                symbolButton.Text = "Symbol...";
                symbolButtonDropDown.Items.Clear();

                instrumentButton.Text = "Instrument...";
                instrumentButtonDropDown.Items.Clear();

                accounts.Nodes.Clear();
                strategies.Nodes.Clear();

                buyButton.Enabled = false;
                sellButton.Enabled = false;
                accounts.Enabled = false;
                strategies.Enabled = false;
            }
        }

		private void FireInvestmentSystemSelected( object sender, EventArgs e )
        {
            ToolStripMenuItem tsmi = (ToolStripMenuItem)sender;
            investmentSystemButton.Text = tsmi.Text;
            selectedInvestmentSystem = (InvestmentSystem)tsmi.Tag;

            //
            //  Load the strategies for this investment system
            //

            strategies.Nodes.Clear();

            foreach( Strategy strategy in selectedInvestmentSystem.Strategies )
            {
                GTLTreeNode strategyNode = new GTLTreeNode();
                strategyNode.SubItems.AddRange( new GTLSubItem [] {
                    new GTLSubItem( strategy[ DatabaseObject.NAME ] ),
                    new GTLSubItem( "0" )
                } );

		        strategyNode.Tag = strategy[ DatabaseObject.SIGNAL_ID ];
		        strategies.Nodes.Add( strategyNode );
		    }
        }

        private void FireSymbolSelected( object sender, EventArgs e )
        {
            ToolStripMenuItem tsmi = (ToolStripMenuItem)sender;
            symbolButton.Text = tsmi.Text;

            this.SuspendLayout();
            instrumentButtonDropDown.SuspendLayout();
            instrumentButton.SuspendLayout();

            instrumentButtonDropDown.Items.Clear();
            instrumentButton.Text = "Instrument...";

            foreach( Instrument instrument in controller.Cache.Instruments )
            {
                if( tsmi.Tag.Equals( instrument[ DatabaseObject.SYMBOL ] ) )
                {
                    ToolStripMenuItem instrument_tsmi = new ToolStripMenuItem();
                    instrument_tsmi.Name = instrument[ DatabaseObject.INSTRUMENT_ID ];
                    instrument_tsmi.Text = instrument[ DatabaseObject.NAME ];
            	    instrument_tsmi.Size = new System.Drawing.Size( 188, 22 );
                    instrument_tsmi.Click += new EventHandler( FireInstrumentSelected );
                    instrument_tsmi.Tag = instrument;
                    instrumentButtonDropDown.Items.Add( instrument_tsmi );
                }
            }

            instrumentButtonDropDown.ResumeLayout();
            instrumentButton.ResumeLayout();
            this.ResumeLayout();
        }

    	private void FireInstrumentSelected( object sender, EventArgs e )
        {
            ToolStripMenuItem tsmi = (ToolStripMenuItem)sender;
            instrumentButton.Text = tsmi.Text;
            selectedInstrument = (Instrument)tsmi.Tag;
            buyButton.Enabled = true;
            sellButton.Enabled = true;
            accounts.Enabled = true;
            strategies.Enabled = true;
        }

	    private void FireFormClosing( object sender, FormClosingEventArgs args )
	    {
            args.Cancel = true;
            this.Hide();
	    }

	    private void FireCancelButton( object o, EventArgs e )
        {
            this.Hide();
        }

        private void FireClear( object o, EventArgs e )
        {
            foreach( GTLTreeNode node in strategies.Nodes )
            {
                node.SubItems[ 1 ].Text = "0";
            }

            foreach( GTLTreeNode node in accounts.Nodes )
            {
                node.SubItems[ 1 ].Text = "0";
            }
        }

        private void FireExecuteButton( object sender, EventArgs args )
        {
            int signal_qty = 0;
            double limit_prc = 0.0, stop_prc = 0.0;
            string side_code, order_type;
            DraftOrderData.Types.SideCode side_code_type;
            DraftOrderData.Types.OrderType draft_order_type;

            if( sender == buyButton )
            {
                side_code = "BUY";
                side_code_type = DraftOrderData.Types.SideCode.BUY;
            }
            else
            {
                side_code = "SELL";
                side_code_type = DraftOrderData.Types.SideCode.SELL;
            }

            if( orderTypeComboBox.Text == "LIMIT" )
            {
                order_type = "LIMIT";
                draft_order_type = DraftOrderData.Types.OrderType.LIMIT;

                if( "".Equals( limitPriceTextBox.Text ) )
                {
                    MessageBox.Show( "LIMIT order type requires limit price." );
                    return;
                }
                else
                {
                    try
                    {
                        limit_prc = Convert.ToDouble( limitPriceTextBox.Text );

                        if( limit_prc <= 0.0 )
                        {
                            MessageBox.Show( "Invalid limit price: " + limitPriceTextBox.Text );
                            return;
                        }
                    }
                    catch( Exception limitError )
                    {
                        log.Error( "Signal qty error.", limitError );
                        MessageBox.Show( "Invalid limit price: " +
                            limitPriceTextBox.Text );
                        return;
                    }
                }
            }
            else if( orderTypeComboBox.Text == "STOP" )
            {
                order_type = "STOP";
                draft_order_type = DraftOrderData.Types.OrderType.STOP;

                if( "".Equals( stopPriceTextBox.Text ) )
                {
                    MessageBox.Show( "STOP order type requires stop price." );
                    return;
                }
                else
                {
                    try
                    {
                        stop_prc = Convert.ToDouble( stopPriceTextBox.Text );

                        if( stop_prc <= 0.0 )
                        {
                            MessageBox.Show( "Invalid stop price: " + stopPriceTextBox.Text );
                            return;
                        }
                    }
                    catch( Exception stopError )
                    {
                        log.Error( "Signal qty error.", stopError );
                        MessageBox.Show( "Invalid stop price: '" +
                            stopPriceTextBox.Text + "'" );
                        return;
                    }
                }
            }
            else if( orderTypeComboBox.Text == "STOP LIMIT" )
            {
                order_type = "STOP_LIMIT";
                draft_order_type = DraftOrderData.Types.OrderType.STOP_LIMIT;

                if( "".Equals( limitPriceTextBox.Text ) )
                {
                    MessageBox.Show( "STOP LIMIT order type requires limit price." );
                    return;
                }
                else
                {
                    try
                    {
                        limit_prc = Convert.ToDouble( limitPriceTextBox.Text );

                        if( limit_prc <= 0.0 )
                        {
                            MessageBox.Show( "Invalid limit price: " + limitPriceTextBox.Text );
                            return;
                        }
                    }
                    catch( Exception limitError )
                    {
                        log.Error( "Signal qty error.", limitError );
                        MessageBox.Show( "Invalid limit price: " + limitPriceTextBox.Text );
                        return;
                    }
                }

                if( "".Equals( stopPriceTextBox.Text ) )
                {
                    MessageBox.Show( "STOP LIMIT order type requires stop price." );
                    return;
                }
                else
                {
                    try
                    {
                        stop_prc = Convert.ToDouble( stopPriceTextBox.Text );

                        if( stop_prc <= 0.0 )
                        {
                            MessageBox.Show( "Invalid stop price: " + stopPriceTextBox.Text );
                            return;
                        }
                    }
                    catch( Exception stopError )
                    {
                        log.Error( "Signal qty error.", stopError );
                        MessageBox.Show( "Invalid stop price: '" +
                            stopPriceTextBox.Text + "'" );
                        return;
                    }
                }
            }
            else if( orderTypeComboBox.Text == "MKT ON CLOSE" )
            {
                order_type = "MARKET_ON_CLOSE";
                draft_order_type = DraftOrderData.Types.OrderType.MARKET_ON_CLOSE;
            }
            else
            {
                order_type = "MARKET";
                draft_order_type = DraftOrderData.Types.OrderType.MARKET;
            }

            if( tabControl.SelectedIndex == 0 )
            {
                //
                //  Validate all strategy quantities
                //

                foreach( GTLTreeNode strategy in strategies.Nodes )
                {
                    try
                    {
                        signal_qty = Convert.ToInt32( strategy.SubItems[ 1 ].Text );
                    }
                    catch( Exception qtyError )
                    {
                        log.Error( "Signal qty error.", qtyError );
                        MessageBox.Show( "Invalid signal size: " +
                            strategy.SubItems[ 1 ].Text );
                        return;
                    }

                    if( signal_qty < 0 )
                    {
                        MessageBox.Show( strategy.SubItems[ 0 ].Text +
                            " has invalid signal size: " +
                            strategy.SubItems[ 1 ].Text );
                        return;
                    }
                }

                //
                //  All imputs are ok, create the signals
                //

                foreach( GTLTreeNode strategy in strategies.Nodes )
                {
                    signal_qty = Convert.ToInt32( strategy.SubItems[ 1 ].Text );

                    if( signal_qty > 0 )
                    {
                        SignalData signalData = SignalData.CreateBuilder()
                            .SetInvestmentSystemId( selectedInvestmentSystem.Id )
                            .SetSignalId( (string)strategy.Tag )
                            .SetInstrumentId( selectedInstrument.Id )
                            .SetSideCode( side_code )
                            .SetSignalQty( strategy.SubItems[ 1 ].Text )
                            .SetOrderType( order_type )
                            .SetLimitPrc( Convert.ToString( limit_prc ) )
                            .SetStopPrc( Convert.ToString( stop_prc ) )
                        .Build();

                        if( releaseToFixCheckBox.Checked )
                        {
                            ClientAdapterResponse response = controller.SendRpc(
                                new com.quantmodel.common.network.message.ExecuteSignal( signalData ) );
                        }
                        else
                        {
                            ClientAdapterResponse response = controller.SendRpc(
                                new com.quantmodel.common.network.message.CreateDraftOrder( signalData ) );

                            if( response.Type == ClientAdapterResponse.Types.ResponseType.ACKNOWLEDGEMENT )
                            {
                                DealingEngineResponse dealing_response = response.DealingResponse;

                                if( dealing_response.Type == DealingEngineResponse.Types.ResponseType.ACKNOWLEDGEMENT )
                                {
                                    foreach( DraftOrderMessage draft_order_msg in dealing_response.DraftOrderList )
                                    {
                                        ClientAdapterResponse create_order_response = controller.SendRpc(
                                            new com.quantmodel.common.network.message.CreateOrder( draft_order_msg ) );
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                //
                //  Validate all account quantities
                //

                foreach( GTLTreeNode account in accounts.Nodes )
                {
                    try
                    {
                        signal_qty = Convert.ToInt32( account.SubItems[ 1 ].Text );
                    }
                    catch( Exception qtyError )
                    {
                        log.Error( "Allocation qty error.", qtyError );
                        MessageBox.Show( "Invalid allocation quantity: " +
                            account.SubItems[ 1 ].Text );
                        return;
                    }

                    if( signal_qty < 0 )
                    {
                        MessageBox.Show( account.SubItems[ 0 ].Text +
                            " has invalid allocation quantity: " +
                            account.SubItems[ 1 ].Text );
                        return;
                    }
                }

                //
                //  All imputs are ok, create the draft orders
                //

                foreach( GTLTreeNode account in accounts.Nodes )
                {
                    signal_qty = Convert.ToInt32( account.SubItems[ 1 ].Text );

                    if( signal_qty > 0 )
                    {
                        DraftOrderData draftOrder = DraftOrderData.CreateBuilder()
                            .SetInvestmentSystemId( "CLIENT" )
                            .SetInstrumentId( selectedInstrument.Id )
                            .SetSideCode( side_code_type )
                            .SetTif( DraftOrderData.Types.TimeInForce.DAY )
                            .SetOrderType( draft_order_type )
                            .SetOrderQty( Convert.ToString( signal_qty ) )
                            .SetLimitPrc( Convert.ToString( limit_prc ) )
                            .SetStopPrc( Convert.ToString( stop_prc ) )
                        .Build();

                        DraftAllocationData draftAllocation = DraftAllocationData.CreateBuilder()
                            .SetAccountId( (string)account.Tag )
                            .SetAllocationQty( Convert.ToString( signal_qty ) )
                        .Build();

                        ClientAdapterResponse create_order_response = controller.SendRpc(
                            new com.quantmodel.common.network.message.CreateOrder( draftOrder, draftAllocation ) );

                        if( create_order_response.Type == ClientAdapterResponse.Types.ResponseType.ACKNOWLEDGEMENT )
                        {
                            DealingEngineResponse dealing_order_response = create_order_response.DealingResponse;

                            if( dealing_order_response.Type == DealingEngineResponse.Types.ResponseType.ACKNOWLEDGEMENT &&
                                releaseToFixCheckBox.Checked )
                            {
                                //
                                //  Release the order
                                //

                                foreach( OrderMessage order_msg in dealing_order_response.OrderList )
                                {
                                    ClientAdapterResponse release_order_response = controller.SendRpc(
                                        new com.quantmodel.common.network.message.ReleaseOrder( order_msg.OrderData.OrderId ) );

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
            }
        }
	}
}
