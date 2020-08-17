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
using GlacialComponents.Controls.GTLCommon;

using quantmodel;
using com.quantmodel.common.data;
using com.quantmodel.common.events;
using com.quantmodel.common.controls;
using com.quantmodel.common.network.message;

using com.quantmodel.client.controls.form;

namespace com.quantmodel.client.controls.list
{
    public sealed class OrderList : UserListControl
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( OrderList ) );

        private readonly CreateOrderForm createOrderForm;
        private readonly ClientController controller;
        private readonly Splitter splitter = new Splitter();
        private readonly GlacialTreeList allocations = ControlHelper.NewGlacialTreeList( "AllocationListData" );

        public OrderList( ClientController controller )
		{
		    this.controller = controller;
		    this.SuspendLayout();
		    this.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Name = "OrderList";
			InitializeComponents();
			this.ResumeLayout( false );

			createOrderForm = new CreateOrderForm( controller );
			createOrderForm.Size = new Size( 650, 600 );
            createOrderForm.FormBorderStyle = FormBorderStyle.Sizable;
            createOrderForm.StartPosition = FormStartPosition.CenterScreen;

            LoadOrders( controller.Cache.Orders );

            data.SelectedIndexChanged +=
                new GTSelectionChangedEventHandler(
                    FireSelectedIndexChanged );

            controller.Dispatcher.Rollover += new EventHandler<DealingEventArgs>( FireRollover );
            controller.Dispatcher.OrderAdded += new EventHandler<DealingEventArgs>( FireOrderAdded );
            controller.Dispatcher.OrderChanged += new EventHandler<DealingEventArgs>( FireOrderChanged );

            //
            //  Timer Events
            //

            timer.Tick += new EventHandler( FireTimerEvent );
            timer.Interval = 200;
            timer.Start();
		}

		private void InitializeComponents()
        {
            //
            //  Tree List
            //

            data = ControlHelper.NewGlacialTreeList( "OrderListData" );
            data.MultiSelect = true;
            data.BeforeCellEdit += new GTLEmbeddedControlEventHandler( FireBeforeCellEdit );
			data.AfterCellEdit += new GTLEmbeddedControlEventHandler( FireAfterCellEdit );

            //
            //  Tree List columns
            //

            GTLColumn order_id = new GTLColumn();
            order_id.BackColor = Color.White;
            order_id.CheckBoxes = false;
			order_id.ImageIndex = -1;
			order_id.Name = DatabaseObject.ORDER_ID;
			order_id.Text = "Order ID";
			order_id.SortType = SortTypes.InsertionSort;
			order_id.ItemComparerType = ItemComparerTypes.Custom;
			order_id.ComparerHelper = new CompareById( "O" );
			order_id.SortDirection = GSortDirectionTypes.Descending;

			GTLColumn external_order_id = new GTLColumn();
            external_order_id.BackColor = Color.White;
            external_order_id.CheckBoxes = false;
			external_order_id.ImageIndex = -1;
			external_order_id.Name = DatabaseObject.ORDER_ID;
			external_order_id.Text = "Ext Order ID";
			external_order_id.SortType = SortTypes.InsertionSort;
			external_order_id.Visible = false;

            GTLColumn parent_order_id = new GTLColumn();
            parent_order_id.BackColor = Color.White;
            parent_order_id.CheckBoxes = false;
			parent_order_id.ImageIndex = -1;
			parent_order_id.Name = DatabaseObject.PARENT_ORDER_ID;
			parent_order_id.Text = "Prnt Order ID";
			parent_order_id.SortType = SortTypes.InsertionSort;
			parent_order_id.Visible = false;

            GTLColumn investment_system_id = new GTLColumn();
            investment_system_id.BackColor = Color.White;
            investment_system_id.CheckBoxes = false;
			investment_system_id.ImageIndex = -1;
			investment_system_id.Name = DatabaseObject.INVESTMENT_SYSTEM_ID;
			investment_system_id.Text = "InvSys ID";
			investment_system_id.SortType = SortTypes.InsertionSort;
			investment_system_id.Visible = true;

            GTLColumn order_type = new GTLColumn();
            order_type.BackColor = Color.White;
            order_type.CheckBoxes = false;
			order_type.ImageIndex = -1;
			order_type.Name = DatabaseObject.ORDER_TYPE;
			order_type.Text = "Order Type";
			order_type.SortType = SortTypes.InsertionSort;
			order_type.EmbeddedDisplayConditions = GEmbeddedDisplayConditions.DoubleClick;
			order_type.EmbeddedControlType = GEmbeddedControlTypes.ComboBox;

            GTLColumn order_status = new GTLColumn();
            order_status.BackColor = Color.White;
            order_status.CheckBoxes = false;
			order_status.ImageIndex = -1;
			order_status.Name = DatabaseObject.ORDER_STATUS;
			order_status.Text = "Order Status";
			order_status.SortType = SortTypes.InsertionSort;

            GTLColumn instrument_id = new GTLColumn();
            instrument_id.BackColor = Color.White;
            instrument_id.CheckBoxes = false;
			instrument_id.ImageIndex = -1;
			instrument_id.Name = DatabaseObject.INSTRUMENT_ID;
			instrument_id.Text = "Instrument ID";
			instrument_id.SortType = SortTypes.InsertionSort;
			instrument_id.Visible = false;

			GTLColumn instrument_name = new GTLColumn();
            instrument_name.BackColor = Color.White;
            instrument_name.CheckBoxes = false;
			instrument_name.ImageIndex = -1;
			instrument_name.Name = "instrument_name";
			instrument_name.Text = "Instrument Name";
			instrument_name.SortType = SortTypes.InsertionSort;
			instrument_name.Width = 220;

            GTLColumn symbol = new GTLColumn();
            symbol.BackColor = Color.White;
            symbol.CheckBoxes = false;
			symbol.ImageIndex = -1;
			symbol.Name = DatabaseObject.SYMBOL;
			symbol.Text = "Symbol";
			symbol.SortType = SortTypes.InsertionSort;

            GTLColumn tif = new GTLColumn();
            tif.BackColor = Color.White;
            tif.CheckBoxes = false;
			tif.ImageIndex = -1;
			tif.Name = DatabaseObject.TIF;
			tif.Text = "TIF";
			tif.SortType = SortTypes.InsertionSort;

            GTLColumn side_code = new GTLColumn();
            side_code.BackColor = Color.White;
            side_code.CheckBoxes = false;
			side_code.ImageIndex = -1;
			side_code.Name = DatabaseObject.SIDE_CODE;
			side_code.Text = "Side Code";
			side_code.SortType = SortTypes.InsertionSort;

            GTLColumn base_ccy = new GTLColumn();
            base_ccy.BackColor = Color.White;
            base_ccy.CheckBoxes = false;
			base_ccy.ImageIndex = -1;
			base_ccy.Name = DatabaseObject.BASE_CCY;
			base_ccy.Text = "Base Ccy";
			base_ccy.SortType = SortTypes.InsertionSort;
			base_ccy.Visible = false;

            GTLColumn order_qty = new GTLColumn();
            order_qty.CheckBoxes = false;
			order_qty.ImageIndex = -1;
			order_qty.Name = DatabaseObject.ORDER_QTY;
			order_qty.Text = "Order Qty";
			order_qty.TextAlignment = ContentAlignment.MiddleRight;
			order_qty.SortType = SortTypes.InsertionSort;

			GTLColumn open_qty = new GTLColumn();
            open_qty.CheckBoxes = false;
			open_qty.ImageIndex = -1;
			open_qty.Name = DatabaseObject.OPEN_QTY;
			open_qty.Text = "Open Qty";
			open_qty.TextAlignment = ContentAlignment.MiddleRight;
			open_qty.SortType = SortTypes.InsertionSort;

            GTLColumn released_qty = new GTLColumn();
            released_qty.CheckBoxes = false;
			released_qty.ImageIndex = -1;
			released_qty.Name = DatabaseObject.RELEASED_QTY;
			released_qty.Text = "Released Qty";
			released_qty.TextAlignment = ContentAlignment.MiddleRight;
			released_qty.SortType = SortTypes.InsertionSort;

            GTLColumn executed_qty = new GTLColumn();
            executed_qty.CheckBoxes = false;
			executed_qty.ImageIndex = -1;
			executed_qty.Name = DatabaseObject.EXECUTED_QTY;
			executed_qty.Text = "Executed Qty";
			executed_qty.TextAlignment = ContentAlignment.MiddleRight;
			executed_qty.SortType = SortTypes.InsertionSort;

            GTLColumn limit_prc = new GTLColumn();
            limit_prc.CheckBoxes = false;
			limit_prc.ImageIndex = -1;
			limit_prc.Name = DatabaseObject.LIMIT_PRC;
			limit_prc.Text = "Limit Prc";
			limit_prc.TextAlignment = ContentAlignment.MiddleRight;
			limit_prc.SortType = SortTypes.InsertionSort;
			limit_prc.EmbeddedDisplayConditions = GEmbeddedDisplayConditions.DoubleClick;
			limit_prc.EmbeddedControlType = GEmbeddedControlTypes.TextBox;

			GTLColumn stop_prc = new GTLColumn();
            stop_prc.CheckBoxes = false;
			stop_prc.ImageIndex = -1;
			stop_prc.Name = DatabaseObject.STOP_PRC;
			stop_prc.Text = "Stop Prc";
			stop_prc.TextAlignment = ContentAlignment.MiddleRight;
			stop_prc.SortType = SortTypes.InsertionSort;

			GTLColumn entry_prc = new GTLColumn();
            entry_prc.CheckBoxes = false;
			entry_prc.ImageIndex = -1;
			entry_prc.Name = DatabaseObject.ENTRY_PRC;
			entry_prc.Text = "Entry Prc";
			entry_prc.TextAlignment = ContentAlignment.MiddleRight;
			entry_prc.SortType = SortTypes.InsertionSort;

            GTLColumn average_prc = new GTLColumn();
            average_prc.CheckBoxes = false;
			average_prc.ImageIndex = -1;
			average_prc.Name = DatabaseObject.AVERAGE_PRC;
			average_prc.Text = "Average Prc";
			average_prc.TextAlignment = ContentAlignment.MiddleRight;
			average_prc.SortType = SortTypes.InsertionSort;

            GTLColumn released_val = new GTLColumn();
            released_val.CheckBoxes = false;
			released_val.ImageIndex = -1;
			released_val.Name = DatabaseObject.RELEASED_VAL;
			released_val.Text = "Released Val";
			released_val.TextAlignment = ContentAlignment.MiddleRight;
			released_val.SortType = SortTypes.InsertionSort;

            GTLColumn executed_val = new GTLColumn();
            executed_val.CheckBoxes = false;
			executed_val.ImageIndex = -1;
			executed_val.Name = DatabaseObject.EXECUTED_VAL;
			executed_val.Text = "Executed Val";
			executed_val.TextAlignment = ContentAlignment.MiddleRight;
			executed_val.SortType = SortTypes.InsertionSort;

            GTLColumn added_datetime = new GTLColumn();
            added_datetime.CheckBoxes = false;
			added_datetime.ImageIndex = -1;
			added_datetime.Name = DatabaseObject.ADDED_DATETIME;
			added_datetime.Text = "Added Datetime";
			added_datetime.TextAlignment = ContentAlignment.MiddleRight;
			added_datetime.SortType = SortTypes.InsertionSort;

            GTLColumn added_by = new GTLColumn();
            added_by.CheckBoxes = false;
			added_by.ImageIndex = -1;
			added_by.Name = DatabaseObject.ADDED_BY;
			added_by.Text = "Added By";
			added_by.SortType = SortTypes.InsertionSort;

            GTLColumn updated_datetime = new GTLColumn();
            updated_datetime.CheckBoxes = false;
			updated_datetime.ImageIndex = -1;
			updated_datetime.Name = DatabaseObject.UPDATED_DATETIME;
			updated_datetime.Text = "Updated Datetime";
			updated_datetime.TextAlignment = ContentAlignment.MiddleRight;
			updated_datetime.SortType = SortTypes.InsertionSort;

            GTLColumn updated_by = new GTLColumn();
            updated_by.CheckBoxes = false;
			updated_by.ImageIndex = -1;
			updated_by.Name = DatabaseObject.UPDATED_BY;
			updated_by.Text = "Updated By";
			updated_by.SortType = SortTypes.InsertionSort;

			data.Columns.AddRange(
			    new GTLColumn [] {
                    order_id,
                    external_order_id,
                    order_status,
                    order_type,
                    parent_order_id,
                    investment_system_id,
                    instrument_id,
                    instrument_name,
                    symbol,
                    tif,
                    side_code,
                    base_ccy,
                    order_qty,
                    open_qty,
                    released_qty,
                    executed_qty,
                    limit_prc,
                    stop_prc,
                    average_prc,
                    entry_prc,
                    released_val,
                    executed_val,
                    added_datetime,
                    added_by,
                    updated_datetime,
                    updated_by
			    } );

            //
			//  Context Menu
			//

			ContextMenu ctxMenu = new ContextMenu();

            //
			//  First, build the actions that you can do
			//

			MenuItem create = new MenuItem( "CREATE ORDER",
			    new EventHandler( FireCreateOrder )  );

			MenuItem cancel = new MenuItem( "CANCEL ORDER",
			    new EventHandler( FireCancelOrder )  );

			MenuItem release = new MenuItem( "RELEASE ORDER",
			    new EventHandler( FireReleaseOrder )  );

			MenuItem showAllocs = new MenuItem( "SHOW ALLOCATIONS",
			    new EventHandler( FireShowAllocations )  );

			showAllocs.Checked = false;

            //
            //  Now build the columns, skipping the first
            //

            MenuItem columnItems = new MenuItem( "Show Column..." );
            for( int i=1; i<data.Columns.Count; i++ )
            {
                GTLColumn col = data.Columns[ i ];
                MenuItem item = new MenuItem( col.Text,
                    new EventHandler( FireShowColumn ) );

                item.Checked = col.Visible;
                columnItems.MenuItems.Add( item );
            }

			ctxMenu.MenuItems.AddRange( new MenuItem []
			{
			    create,
			    new MenuItem( "-" ),
			    release,
			    new MenuItem( "-" ),
			    cancel,
			    new MenuItem( "-" ),
			    showAllocs,
			    new MenuItem( "-" ),
			    columnItems
			} );

            data.ContextMenu = ctxMenu;

            //
            //  Splitter
            //

            splitter.Size = new System.Drawing.Size( 2, 2 );
            splitter.BackColor = System.Drawing.Color.LightSteelBlue;
            splitter.Dock = System.Windows.Forms.DockStyle.Right;

            //
            //  Allocations
            //

            GTLColumn allocation_id = new GTLColumn();
            allocation_id.BackColor = Color.White;
            allocation_id.CheckBoxes = false;
            allocation_id.Width = 50;
			allocation_id.ImageIndex = -1;
			allocation_id.Name = DatabaseObject.ALLOCATION_ID;
			allocation_id.Text = "Alloc ID";
			allocation_id.SortType = SortTypes.InsertionSort;

			GTLColumn allocation_order_id = new GTLColumn();
            allocation_order_id.BackColor = Color.White;
            allocation_order_id.CheckBoxes = false;
            allocation_order_id.Width = 50;
			allocation_order_id.ImageIndex = -1;
			allocation_order_id.Name = DatabaseObject.ORDER_ID;
			allocation_order_id.Text = "Order ID";
			allocation_order_id.SortType = SortTypes.InsertionSort;

			GTLColumn account_name = new GTLColumn();
            account_name.BackColor = Color.White;
            account_name.CheckBoxes = false;
			account_name.ImageIndex = -1;
			account_name.Name = DatabaseObject.NAME;
			account_name.Text = "Account Name";
			account_name.SortType = SortTypes.InsertionSort;

			GTLColumn allocation_qty = new GTLColumn();
            allocation_qty.CheckBoxes = false;
			allocation_qty.ImageIndex = -1;
			allocation_qty.Name = DatabaseObject.AVERAGE_PRC;
			allocation_qty.Text = "Allocation Qty";
			allocation_qty.TextAlignment = ContentAlignment.MiddleRight;
			allocation_qty.SortType = SortTypes.InsertionSort;

			GTLColumn alloc_executed_qty = new GTLColumn();
            alloc_executed_qty.CheckBoxes = false;
			alloc_executed_qty.ImageIndex = -1;
			alloc_executed_qty.Name = DatabaseObject.EXECUTED_QTY;
			alloc_executed_qty.Text = "Executed Qty";
			alloc_executed_qty.TextAlignment = ContentAlignment.MiddleRight;
			alloc_executed_qty.SortType = SortTypes.InsertionSort;

			GTLColumn alloc_average_prc = new GTLColumn();
            alloc_average_prc.CheckBoxes = false;
			alloc_average_prc.ImageIndex = -1;
			alloc_average_prc.Name = DatabaseObject.AVERAGE_PRC;
			alloc_average_prc.Text = "Average Prc";
			alloc_average_prc.TextAlignment = ContentAlignment.MiddleRight;
			alloc_average_prc.SortType = SortTypes.InsertionSort;

			allocations.Columns.AddRange(
			    new GTLColumn [] {
                    allocation_id,
                    allocation_order_id,
                    account_name,
                    allocation_qty,
                    alloc_executed_qty,
                    alloc_average_prc } );

			this.Controls.AddRange( new Control [] { data, splitter, allocations } );
        }

        private void FireOrderAdded( object o, DealingEventArgs e )
        {
            Order order = (Order)e.Data;
            GTLTreeNode orderNode = CreateOrderNode( order );

            data.BeginUpdate();
            data.BeginNodeListChange();

            if( null != orderNode )
            {
                data.Nodes.Add( orderNode );
            }

            //
            //  Play a sound!!!
            //

            System.Media.SystemSounds.Asterisk.Play();

            SortData( data.Columns[ DatabaseObject.ORDER_ID ] );

            data.EndNodeListChange();
            data.EndUpdate();

            for( int i=0; i<orderNode.SubItems.Count; i++ )
            {
                GTLSubItem sub = orderNode.SubItems[ i ];
                ColorSubItem csi =
                    new BlueColorSubItem(
                        Color.White, orderNode.SubItems[ i ],
                        orderNode.Tag + data.Columns[ i ].Text );
                colorNodes[ csi.Tag ] = csi;
            }
        }

        private void FireOrderChanged( object o, DealingEventArgs e )
        {
            Order order = (Order)e.Data;
            Order oldOrder = (Order)e.OldData;

            GTLTreeNode orderNode = null;
            foreach( GTLTreeNode node in data.Nodes )
            {
                if( node.Tag.Equals( order.Id ) )
                {
                    orderNode = node;
                    break;
                }
            }

            if( null != orderNode )
            {
                //
                //  Update the columns that can change on an order:
                //
                //      ORDER_STATUS
                //      ORDER_TYPE
                //      LIMIT_PRC
                //      STOP_PRC
                //      OPEN_QTY
                //      RELEASED_QTY
                //      EXECUTED_QTY
                //      RELEASED_VAL
                //      EXECUTED_VAL
                //      UPDATED_DATETIME
                //      UPDATED_BY
                //

                FireSubItem( orderNode, DatabaseObject.ORDER_STATUS, order.OrderStatus() );
                FireSubItem( orderNode, DatabaseObject.ORDER_TYPE, order.OrderType() );
                FireSubItem( orderNode, DatabaseObject.LIMIT_PRC, order[ DatabaseObject.LIMIT_PRC ] );
                FireSubItem( orderNode, DatabaseObject.STOP_PRC, order[ DatabaseObject.STOP_PRC ] );
                FireSubItem( orderNode, DatabaseObject.AVERAGE_PRC, order[ DatabaseObject.AVERAGE_PRC ] );
                FireSubItem( orderNode, DatabaseObject.OPEN_QTY, order[ DatabaseObject.OPEN_QTY ] );
                FireSubItem( orderNode, DatabaseObject.RELEASED_QTY, order[ DatabaseObject.RELEASED_QTY ] );
                FireSubItem( orderNode, DatabaseObject.EXECUTED_QTY, order[ DatabaseObject.EXECUTED_QTY ] );
                FireSubItem( orderNode, DatabaseObject.RELEASED_VAL, order[ DatabaseObject.RELEASED_VAL ] );
                FireSubItem( orderNode, DatabaseObject.EXECUTED_VAL, order[ DatabaseObject.EXECUTED_VAL ] );
                FireSubItem( orderNode, DatabaseObject.UPDATED_DATETIME, order.ToLocalTime( DatabaseObject.UPDATED_DATETIME ) );
                FireSubItem( orderNode, DatabaseObject.UPDATED_BY, order[ DatabaseObject.UPDATED_BY ] );
            }
        }

        private void FireCreateOrder( object o, EventArgs e )
        {
            createOrderForm.Show();
        }

        private void FireCancelOrder( object o, EventArgs e )
        {
            string message = null;

            if( data.SelectedNodes.Count > 1 )
            {
                message = "Are you sure you want to CANCEL these orders?";
            }
            else
            {
    			message = "Are you sure you want to CANCEL this order?";
    	    }

    	    DialogResult result = MessageBox.Show(
    			    message,
    			    "CANCEL Order Confirmation",
    			    MessageBoxButtons.YesNo,
    			    MessageBoxIcon.Exclamation );

			if( result == System.Windows.Forms.DialogResult.Yes )
			{
                //
                //  Get the selected orders
                //

                foreach( GTLTreeNode orderNode in data.SelectedNodes )
                {
                    if( controller.Cache.HasOrder( orderNode.Tag ) )
                    {
                        Order order = controller.Cache.GetOrder( orderNode.Tag );

                        if( "NEW".Equals( order.OrderStatus() ) ||
                            "SUPPRESSED".Equals( order.OrderStatus() ) )
                        {
                            ClientAdapterResponse response =
                                controller.SendRpc( new CancelOrder( order.Id ) );

                            if( response.Type == ClientAdapterResponse.Types.ResponseType.ACKNOWLEDGEMENT )
                            {
                                DealingEngineResponse dealing_response = response.DealingResponse;

                                if( dealing_response.Type == DealingEngineResponse.Types.ResponseType.REJECT )
                                {
                                    MessageBox.Show(
                        			   order.Id + ": " + dealing_response.ErrorMsg,
                        			   "CANCEL Order Error",
                        			   MessageBoxButtons.OK,
                        			   MessageBoxIcon.Exclamation );

                                    data.ClearSelection();
                                    return;
                                }
                            }
                        }
                        else if( "PARTIALLY_FILLED".Equals( order.OrderStatus() ) )
                        {
                            try
                            {
                                double released_qty = Convert.ToDouble( order[ DatabaseObject.RELEASED_QTY ] );
                                double executed_qty = Convert.ToDouble( order[ DatabaseObject.EXECUTED_QTY ] );

                                if( released_qty == executed_qty )
                                {
                                    ClientAdapterResponse response =
                                        controller.SendRpc( new CancelOrder( order.Id ) );

                                    if( response.Type == ClientAdapterResponse.Types.ResponseType.ACKNOWLEDGEMENT )
                                    {
                                        DealingEngineResponse dealing_response = response.DealingResponse;

                                        if( dealing_response.Type == DealingEngineResponse.Types.ResponseType.REJECT )
                                        {
                                            MessageBox.Show(
                                			   order.Id + ": " + dealing_response.ErrorMsg,
                                			   "CANCEL Order Error",
                                			   MessageBoxButtons.OK,
                                			   MessageBoxIcon.Exclamation );

                                            data.ClearSelection();
                                            return;
                                        }
                                    }
                                }
                                else
                                {
                                    MessageBox.Show(
                        			    order.Id + " Invalid Order Status: order has open releases.",
                        			    "CANCEL Order Error",
                        			    MessageBoxButtons.OK,
                        			    MessageBoxIcon.Exclamation );

                                    data.ClearSelection();
                                    return;
                                }
                            }
                            catch( Exception )
                            {
                                MessageBox.Show(
                    			    order.Id + " invalid released_qty / executed_qty:",
                    			    "CANCEL Order Error",
                    			    MessageBoxButtons.OK,
                    			    MessageBoxIcon.Exclamation );

                                data.ClearSelection();
                                return;
                            }
                        }
                        else
                        {
                            MessageBox.Show(
                			    order.Id + " Invalid Order Status: " + order.OrderStatus(),
                			    "CANCEL Order Error",
                			    MessageBoxButtons.OK,
                			    MessageBoxIcon.Exclamation );

                            data.ClearSelection();
                            return;
                        }
                    }
                    else
                    {
                        MessageBox.Show(
                            " Unknown Order: '" + orderNode.Tag + "'",
            			    "CANCEL Order Error",
            			    MessageBoxButtons.OK,
            			    MessageBoxIcon.Exclamation );

                        data.ClearSelection();
                        return;
                    }
                }

                data.ClearSelection();
            }
        }

        private void FireShowAllocations( object o, EventArgs e )
        {
            MenuItem item = (MenuItem)o;
            if( item.Checked )
            {
                item.Checked = splitter.Visible = allocations.Visible = false;
            }
            else
            {
                item.Checked = splitter.Visible = allocations.Visible = true;
            }
        }

        private void FireReleaseOrder( object o, EventArgs e )
        {
            string message = null;

            if( data.SelectedNodes.Count > 1 )
            {
                message = "Are you sure you want to RELEASE these orders?";
            }
            else
            {
    			message = "Are you sure you want to RELEASE this order?";
    	    }

    	    DialogResult result = MessageBox.Show(
    			    message,
    			    "RELEASE Order Confirmation",
    			    MessageBoxButtons.YesNo,
    			    MessageBoxIcon.Exclamation );

			if( result == System.Windows.Forms.DialogResult.Yes )
			{
                //
                //  Get the selected orders
                //

                foreach( GTLTreeNode orderNode in data.SelectedNodes )
                {
                    if( controller.Cache.HasOrder( orderNode.Tag ) )
                    {
                        Order order = controller.Cache.GetOrder( orderNode.Tag );

                        if( "NEW".Equals( order.OrderStatus() ) ||
                            "PARTIALLY_FILLED".Equals( order.OrderStatus() ) ||
                            "SUPPRESSED".Equals( order.OrderStatus() ) )
                        {
                            ClientAdapterResponse response =
                                controller.SendRpc( new ReleaseOrder( order.Id ) );

                            if( response.Type != ClientAdapterResponse.Types.ResponseType.ACKNOWLEDGEMENT )
                            {
                                MessageBox.Show(
                    			    response.ErrorMsg,
                    			    "RELEASE Order Error",
                    			    MessageBoxButtons.OK,
                    			    MessageBoxIcon.Exclamation );

                                data.ClearSelection();
                                return;
                            }

                            DealingEngineResponse dealing_response = response.DealingResponse;

                            if( dealing_response.Type != DealingEngineResponse.Types.ResponseType.ACKNOWLEDGEMENT )
                            {
                                MessageBox.Show(
                    			    dealing_response.ErrorMsg,
                    			    "RELEASE Order Error",
                    			    MessageBoxButtons.OK,
                    			    MessageBoxIcon.Exclamation );

                                data.ClearSelection();
                                return;
                            }
                        }
                        else
                        {
                            MessageBox.Show(
                			    order.Id + " Invalid Order Status: " + order.OrderStatus(),
                			    "RELEASE Order Error",
                			    MessageBoxButtons.OK,
                			    MessageBoxIcon.Exclamation );

                            data.ClearSelection();
                            return;
                        }
                    }
                }

                data.ClearSelection();
            }
        }

        private void FireBeforeCellEdit( object source, GTLEmbeddedControlEventArgs args )
		{
		    GTLTreeNode node = args.TreeNode;

		    //
		    //  You can only edit 'NEW' or 'SUPPRESSED' orders
		    //

		    if( node.SubItems[ DatabaseObject.ORDER_STATUS ].Text.Equals( "NEW" ) ||
		        node.SubItems[ DatabaseObject.ORDER_STATUS ].Text.Equals( "SUPPRESSED" ) )
            {
    		    GTLColumn col = data.Columns[ args.Column ];

    		    if( col.Name.Equals( DatabaseObject.ORDER_TYPE ) )
    			{
    			    args.Cancel = false;
    				ComboBox cb = (ComboBox)args.Control;
    				cb.Items.Add( "LIMIT" );
    				cb.Items.Add( "MARKET" );
    				cb.Text = col.Text;
    			}
    			else if( col.Name.Equals( DatabaseObject.LIMIT_PRC ) )
                {
                    args.Cancel = false;
                }
                else
                {
                    args.Cancel = true;
                }
            }
            else
            {
                args.Cancel = true;
            }
		}

        private void FireAfterCellEdit( object source, GTLEmbeddedControlEventArgs args )
		{
		    args.Cancel = true;
		    GTLTreeNode node = args.TreeNode;
            GTLColumn col = data.Columns[ args.Column ];

		    if( col.Name.Equals( DatabaseObject.ORDER_TYPE ) )
			{
			    //ComboBox cb = (ComboBox)args.Control;
			    //if(! col.Text.Equals( cb.Text ) )
			    //{
			    //    OrderData orderData = (OrderData)
                //        DealingEngineViewer.ORDERS[ node.Tag ];
                //
                //    if( "LIMIT".Equals( cb.Text ) )
                //    {
                //        //
                //        //  Only set to limit if limit_prc is > 0
                //        //
                //
                //        if( orderData.ToDouble( DatabaseObject.LIMIT_PRC ) > 0 )
                //        {
                //            orderData.SetAttribute( DatabaseObject.ORDER_TYPE,
                //                orderData.OrderType( cb.Text ) );
                //
                //            //
                //            //  Create an 'UpdateOrder' request
                //            //
                //
                //            //XmlDocument doc = new XmlDocument();
                //            //doc.LoadXml( "<bdsq></bdsq>" );
                //            //
                //            //XmlElement dealingEngineRequest =
                //            //    doc.CreateElement( "DealingEngineRequest" );
                //            //dealingEngineRequest.SetAttribute( "id", "UpdateOrder" );
                //            //doc.DocumentElement.AppendChild( dealingEngineRequest );
                //            //
                //            //XmlElement order = doc.CreateElement( "Order" );
                //            //orderData.ToXML( order );
                //            //dealingEngineRequest.AppendChild( order );
                //            //
                //            //data.ClearSelection();
                //            //controller.Dispatcher.SendRpc( doc, MessageEventArgs.DEALING_CALLFORWARD );
                //        }
                //        else
                //        {
                //            MessageBox.Show(
                //			    orderData.Id + " Invalid Limit Price: " +
                //			        orderData[ DatabaseObject.LIMIT_PRC ],
                //			    "UPDATE Order Error",
                //			    MessageBoxButtons.OK,
                //			    MessageBoxIcon.Exclamation );
                //
                //            data.ClearSelection();
                //        }
                //    }
                //    else
                //    {
                //        //
                //        //  Changing order type to MARKET
                //        //
                //
                //        orderData.SetAttribute( DatabaseObject.ORDER_TYPE,
                //            orderData.OrderType( cb.Text ) );
                //
                //        //
                //        //  Create an 'UpdateOrder' request
                //        //
                //
                //        //XmlDocument doc = new XmlDocument();
                //        //doc.LoadXml( "<bdsq></bdsq>" );
                //        //
                //        //XmlElement dealingEngineRequest =
                //        //    doc.CreateElement( "DealingEngineRequest" );
                //        //dealingEngineRequest.SetAttribute( "id", "UpdateOrder" );
                //        //doc.DocumentElement.AppendChild( dealingEngineRequest );
                //        //
                //        //XmlElement order = doc.CreateElement( "Order" );
                //        //orderData.ToXML( order );
                //        //dealingEngineRequest.AppendChild( order );
                //        //
                //        //data.ClearSelection();
                //        //controller.Dispatcher.SendRpc( doc, MessageEventArgs.DEALING_CALLFORWARD );
                //    }
                //}
			}
			else if( col.Name.Equals( DatabaseObject.LIMIT_PRC ) )
            {
                //OrderData orderData = (OrderData)
                //    DealingEngineViewer.ORDERS[ node.Tag ];
                //
                //TextBox tb = (TextBox)args.Control;
                //try
                //{
                //    //
                //    //  Test for a valid number
                //    //
                //
                //    Double.Parse( tb.Text );
                //
                //    //
                //    //  Ensure new limit price is >= 0
                //    //
                //
                //    if( Double.Parse( tb.Text ) >= 0 )
                //    {
                //        //
                //        //  Create an 'UpdateOrder' request
                //        //
                //
                //        orderData.SetAttribute( DatabaseObject.LIMIT_PRC, tb.Text );
                //
                //        //
                //        //  Create an 'UpdateOrder' request
                //        //
                //
                //        //XmlDocument doc = new XmlDocument();
                //        //doc.LoadXml( "<bdsq></bdsq>" );
                //        //
                //        //XmlElement dealingEngineRequest = doc.CreateElement( "DealingEngineRequest" );
                //        //dealingEngineRequest.SetAttribute( "id", "UpdateOrder" );
                //        //doc.DocumentElement.AppendChild( dealingEngineRequest );
                //        //
                //        //XmlElement order = doc.CreateElement( "Order" );
                //        //orderData.ToXML( order );
                //        //dealingEngineRequest.AppendChild( order );
                //        //
                //        //data.ClearSelection();
                //        //controller.Dispatcher.SendRpc( doc, MessageEventArgs.DEALING_CALLFORWARD );
                //    }
                //    else
                //    {
                //        MessageBox.Show(
            	//		    orderData.Id + " Invalid Limit Price: " + tb.Text,
            	//		    "UPDATE Order Error",
            	//		    MessageBoxButtons.OK,
            	//		    MessageBoxIcon.Exclamation );
                //
                //        data.ClearSelection();
                //    }
                //}
                //catch( Exception )
                //{
                //    MessageBox.Show(
        		//	    orderData.Id + " Invalid Limit Price: " + tb.Text,
        		//	    "UPDATE Order Error",
        		//	    MessageBoxButtons.OK,
        		//	    MessageBoxIcon.Exclamation );
                //
                //    data.ClearSelection();
                //}
            }
		}

        private void LoadOrders( ArrayList orders )
        {
            data.BeginUpdate();
            data.BeginNodeListChange();
            data.Nodes.Clear();

            foreach( Order order in orders )
            {
                GTLTreeNode orderNode = CreateOrderNode( order );

                if( null != orderNode )
                {
                    data.Nodes.Add( orderNode );
                }
            }

            SortData( data.Columns[ DatabaseObject.ORDER_ID ] );

            data.EndNodeListChange();
            data.EndUpdate();
        }

        private GTLTreeNode CreateOrderNode( Order order )
        {
            //
            //  Get the instrument on the order
            //

            Instrument instrument =
                controller.Cache.GetInstrument(
                    order[ DatabaseObject.INSTRUMENT_ID ] );

            if( null == instrument )
            {
                log.Warn( "Unknown instrument: " +
                    order[ DatabaseObject.INSTRUMENT_ID ] );

                return null;
            }

            if(! controller.Cache.HasInvestmentSystem(
                 order[ DatabaseObject.INVESTMENT_SYSTEM_ID ] ) )
            {
                log.Warn( "Unknown investment_system: " +
                    order[ DatabaseObject.INVESTMENT_SYSTEM_ID ] );

                return null;
            }

            GTLTreeNode orderNode = new GTLTreeNode( order.Id );
            orderNode.Checked = false;
            orderNode.Collapse();
            orderNode.Tag = order.Id;

            orderNode.SubItems.AddRange( new GTLSubItem [] {
                new GTLSubItem( order[ DatabaseObject.EXTERNAL_ORDER_ID ] ),
                new GTLSubItem( order.OrderStatus( ) ),
                new GTLSubItem( order.OrderType( ) ),
                new GTLSubItem( order[ DatabaseObject.PARENT_ORDER_ID ] ),
                new GTLSubItem( order[ DatabaseObject.INVESTMENT_SYSTEM_ID ] ),
                new GTLSubItem( order[ DatabaseObject.INSTRUMENT_ID ] ),
                new GTLSubItem( instrument[ DatabaseObject.NAME ] ), // instrument_name
                new GTLSubItem( order[ DatabaseObject.SYMBOL ] ),
                new GTLSubItem( order.Tif( ) ),
                new GTLSubItem( order.SideCode( ) ),
                new GTLSubItem( order[ DatabaseObject.BASE_CCY ] ),
                new GTLSubItem( order[ DatabaseObject.ORDER_QTY ] ),
                new GTLSubItem( order[ DatabaseObject.OPEN_QTY ] ),
                new GTLSubItem( order[ DatabaseObject.RELEASED_QTY ] ),
                new GTLSubItem( order[ DatabaseObject.EXECUTED_QTY ] ),
                new GTLSubItem( order[ DatabaseObject.LIMIT_PRC ] ),
                new GTLSubItem( order[ DatabaseObject.STOP_PRC ] ),
                new GTLSubItem( order[ DatabaseObject.AVERAGE_PRC ] ),
                new GTLSubItem( order[ DatabaseObject.ENTRY_PRC ] ),
                new GTLSubItem( order[ DatabaseObject.RELEASED_VAL ] ),
                new GTLSubItem( order[ DatabaseObject.EXECUTED_VAL ] ),
                new GTLSubItem( order.ToLocalTime( DatabaseObject.ADDED_DATETIME ) ),
                new GTLSubItem( order[ DatabaseObject.ADDED_BY ] ),
                new GTLSubItem( order.ToLocalTime( DatabaseObject.UPDATED_DATETIME ) ),
                new GTLSubItem( order[ DatabaseObject.UPDATED_BY ] )
            } );

            return orderNode;
        }


        //private void FireStatusUpdate( object sender, MessageEventArgs args )
		//{
		//    XmlNode node = args.Node;
        //
		//    foreach( XmlNode status_node in node.SelectNodes( "DealingEngineParameters" ) )
        //    {
        //        //
        //        //  Is the DE sending to FIX?
        //        //
        //
        //        data.ContextMenu.MenuItems[ 0 ].Enabled =
        //            "1".Equals( status_node.Attributes[ "send_to_fix" ].Value );
        //    }
        //}
        //
        //private void FireLoadOrders( object sender, MessageEventArgs args )
		//{
        //    XmlNode node = args.Node;
        //
        //    this.SuspendLayout();
        //    data.Nodes.Clear();
        //
        //    foreach( XmlNode order_node in node.SelectNodes( "Orders/Order" ) )
        //    {
        //        AddOrder( order_node );
        //    }
        //
        //    SortData( data.Columns[ DatabaseObject.ORDER_ID ] );
        //
		//	this.ResumeLayout( false );
		//	this.PerformLayout();
		//}
        //
		//private void FireCreateOrder( object sender, MessageEventArgs args )
		//{
		//    XmlNode node = args.Node;
        //
        //    this.SuspendLayout();
        //
        //    foreach( XmlNode order_node in node.SelectNodes( "Order" ) )
        //    {
        //        if( log.IsDebugEnabled )
        //        {
        //            log.Debug( "ADD_ORDER: " + order_node.Attributes[ "order_id" ].Value );
        //        }
        //
        //        AddOrder( order_node, true );
        //
        //        //
        //        //  Play a sound!!!
        //        //
        //
        //        System.Media.SystemSounds.Asterisk.Play();
        //    }
        //
        //    //if( data.LastSortColumnIndex > 0 )
        //        SortData( data.Columns[ data.LastSortColumnIndex ] );
        //
		//	this.ResumeLayout( false );
		//	this.PerformLayout();
	    //}
        //
		//private void FireUpdateOrder( object sender, MessageEventArgs args )
		//{
		//    XmlNode node = args.Node;
        //
        //    this.SuspendLayout();
        //
        //    foreach( XmlNode order_node in node.SelectNodes( "Order" ) )
        //    {
        //        UpdateOrder( order_node );
        //    }
        //
        //    SortData( data.Columns[ data.LastSortColumnIndex ] );
        //
		//	this.ResumeLayout( false );
		//	this.PerformLayout();
	    //}

	    private void AddOrder( XmlNode order_node )
	    {
	        AddOrder( order_node, false );
	    }

	    private void AddOrder( XmlNode order_node, bool fireColor )
	    {
//	        OrderData orderData = new OrderData( order_node );
//
//            if( log.IsDebugEnabled )
//            {
//                if( log.IsDebugEnabled )
//                {
//                    log.Debug( "ADDING ORDER: " + orderData.Id );
//                }
//            }
//
//            if( DealingEngineViewer.ORDERS.ContainsKey( orderData.Id ) )
//            {
//                log.Warn( "DealingEngineViewer.ORDERS already has key: " + orderData.Id );
//                return;
//            }
//            else
//            {
//                DealingEngineViewer.ORDERS.Add( orderData.Id, orderData );
//            }
//
///*
//            XmlDocument instrument = new XmlDocument();
//            XmlDocument investmentSystem = new XmlDocument();
//
//            InstrumentData instrumentData = null;
//            InvestmentSystemData invSysData = null;
//
//            string instrumentXml = controller.GetDirect(
//                "instrument::instrument_id::" + orderData[
//                    DatabaseObject.INSTRUMENT_ID ] );
//
//            string invSysXml = controller.GetDirect(
//                "investment_system::investment_system_id::" + orderData[
//                    DatabaseObject.INVESTMENT_SYSTEM_ID ] );
//
//            if( log.IsDebugEnabled )
//            {
//                log.Debug( "InstrumentXML: " + instrumentXml );
//                log.Debug( "InvestmentSystemXML: " + invSysXml );
//            }
//
//            instrument.LoadXml( instrumentXml );
//            instrumentData = new InstrumentData(
//                instrument.DocumentElement );
//
//            investmentSystem.LoadXml( invSysXml );
//            invSysData = new InvestmentSystemData(
//                investmentSystem.DocumentElement );
//*/
//            ///
//            //
//            ///
//
//            XmlDocument instrument = new XmlDocument();
//            XmlDocument investmentSystem = new XmlDocument();
//
//            InstrumentData instrumentData = null;
//            if( DealingEngineViewer.INSTRUMENTS.ContainsKey(
//                orderData[ DatabaseObject.INSTRUMENT_ID ] ) )
//            {
//                instrumentData = (InstrumentData)
//                    DealingEngineViewer.INSTRUMENTS[
//                        orderData[ DatabaseObject.INSTRUMENT_ID ] ];
//            }
//            else
//            {
//                string instrumentXml = controller.GetDirect(
//                    "instrument::instrument_id::" + orderData[
//                        DatabaseObject.INSTRUMENT_ID ] );
//
//                instrument.LoadXml( instrumentXml );
//                instrumentData = new InstrumentData(
//                    instrument.DocumentElement );
//s
//                DealingEngineViewer.INSTRUMENTS.Add(
//                    instrumentData.Id, instrumentData );
//            }
//
//            InvestmentSystemData invSysData = null;
//            if( DealingEngineViewer.INVESTMENT_SYSTEMS.ContainsKey(
//                orderData[ DatabaseObject.INVESTMENT_SYSTEM_ID ] ) )
//            {
//                invSysData = (InvestmentSystemData)
//                    DealingEngineViewer.INVESTMENT_SYSTEMS[
//                        orderData[ DatabaseObject.INVESTMENT_SYSTEM_ID ] ];
//            }
//            else
//            {
//                string invSysXml = controller.GetDirect(
//                    "investment_system::investment_system_id::" + orderData[
//                        DatabaseObject.INVESTMENT_SYSTEM_ID ] );
//
//                investmentSystem.LoadXml( invSysXml );
//                invSysData = new InvestmentSystemData(
//                    investmentSystem.DocumentElement );
//
//                DealingEngineViewer.INVESTMENT_SYSTEMS.Add(
//                    invSysData.Id, invSysData );
//            }
//
//            ///
//            //
//            ///
//
//            GTLTreeNode orderRow = new GTLTreeNode( orderData.Id );
//            orderRow.Checked = false;
//            orderRow.Collapse();
//            orderRow.Tag = orderData.Id;
//
//            orderRow.SubItems.AddRange( new GTLSubItem [] {
//                new GTLSubItem( orderData.OrderStatus( ) ),
//                new GTLSubItem( orderData.OrderType( ) ),
//                new GTLSubItem( orderData[ DatabaseObject.PARENT_ORDER_ID ] ),
//                new GTLSubItem( orderData[ DatabaseObject.INVESTMENT_SYSTEM_ID ] ),
//                new GTLSubItem( invSysData[ DatabaseObject.NAME ] ), // investment_system_name
//                new GTLSubItem( orderData[ DatabaseObject.INSTRUMENT_ID ] ),
//                new GTLSubItem( instrumentData[ DatabaseObject.NAME ] ), // instrument_name
//                new GTLSubItem( orderData[ DatabaseObject.SYMBOL ] ),
//                new GTLSubItem( orderData.Tif( ) ),
//                new GTLSubItem( orderData.SideCode( ) ),
//                new GTLSubItem( orderData[ DatabaseObject.BASE_CCY ] ),
//                new GTLSubItem( orderData[ DatabaseObject.ORDER_QTY ] ),
//                new GTLSubItem( orderData[ DatabaseObject.OPEN_QTY ] ),
//                new GTLSubItem( orderData[ DatabaseObject.RELEASED_QTY ] ),
//                new GTLSubItem( orderData[ DatabaseObject.EXECUTED_QTY ] ),
//                new GTLSubItem( orderData[ DatabaseObject.LIMIT_PRC ] ),
//                new GTLSubItem( orderData[ DatabaseObject.STOP_PRC ] ),
//                new GTLSubItem( orderData[ DatabaseObject.AVERAGE_PRC ] ),
//                new GTLSubItem( orderData[ DatabaseObject.ENTRY_PRC ] ),
//                new GTLSubItem( orderData[ DatabaseObject.ENTRY_VAL ] ),
//                new GTLSubItem( orderData[ DatabaseObject.RELEASED_VAL ] ),
//                new GTLSubItem( orderData[ DatabaseObject.EXECUTED_VAL ] ),
//                new GTLSubItem( orderData.ToLocalTime( DatabaseObject.ADDED_DATETIME ) ),
//                new GTLSubItem( orderData[ DatabaseObject.ADDED_BY ] ),
//                new GTLSubItem( orderData.ToLocalTime( DatabaseObject.UPDATED_DATETIME ) ),
//                new GTLSubItem( orderData[ DatabaseObject.UPDATED_BY ] )
//            } );
//
//            data.BeginUpdate();
//            data.BeginNodeListChange();
//            data.Nodes.Add( orderRow );
//            data.EndNodeListChange();
//		    data.EndUpdate();
//
//		    if( fireColor )
//		    {
//		        for( int i=0; i<orderRow.SubItems.Count; i++ )
//                {
//                    GTLSubItem sub = orderRow.SubItems[ i ];
//                    ColorSubItem csi =
//                        new BlueColorSubItem(
//                            Color.White, orderRow.SubItems[ i ],
//                                orderRow.Tag + data.Columns[ i ].Text );
//                    colorNodes[ csi.Tag ] = csi;
//                }
//            }
	    }

	    private void UpdateOrder( XmlNode order_node )
	    {
//	        OrderData orderData = new OrderData( order_node );
//
//	        if(! DealingEngineViewer.ORDERS.ContainsKey( orderData.Id ) )
//	        {
//	            AddOrder( order_node, true );
//	            return;
//	        }
//
//	        OrderData oldOrderData = (OrderData) DealingEngineViewer.ORDERS[ orderData.Id ];
//
//	        if( string.Compare( orderData[ DatabaseObject.UPDATED_DATETIME ],
//	            oldOrderData[ DatabaseObject.UPDATED_DATETIME ] ) > 0 )
//            {
//    	        DealingEngineViewer.ORDERS[ orderData.Id ] = orderData;
//
//    	        //
//                //  Search the top level nodes for the order
//                //
//
//                GTLTreeNode orderRow = null;
//                foreach( GTLTreeNode node in data.Nodes )
//                {
//                    if( node.Tag.Equals( orderData.Id ) )
//                    {
//                        orderRow = node;
//                        break;
//                    }
//                }
//
//                if( null != orderRow )
//                {
//                    //
//                    //  Update the columns that can change on an order:
//                    //
//                    //      ORDER_STATUS
//                    //      AVERAGE_PRC
//                    //      RELEASED_QTY
//                    //      EXECUTED_QTY
//                    //      RELEASED_VAL
//                    //      EXECUTED_VAL
//                    //      UPDATED_DATETIME
//                    //      UPDATED_BY
//                    //
//
//                    if(! orderRow.SubItems[ DatabaseObject.ORDER_STATUS ].Text.Equals( orderData.OrderStatus( ) ) )
//                    {
//                        orderRow.SubItems[ DatabaseObject.ORDER_STATUS ].Text = orderData.OrderStatus( );
//                        ColorSubItem csi =
//                            new BlueColorSubItem(
//                                Color.White, orderRow.SubItems[ DatabaseObject.ORDER_STATUS ],
//                                   orderRow.Tag + data.Columns[ DatabaseObject.ORDER_STATUS ].Text );
//                        colorNodes[ csi.Tag ] = csi;
//                    }
//
//                    if(! orderRow.SubItems[ DatabaseObject.ORDER_TYPE ].Text.Equals( orderData.OrderType( ) ) )
//                    {
//                        orderRow.SubItems[ DatabaseObject.ORDER_TYPE ].Text = orderData.OrderType( );
//                        ColorSubItem csi =
//                            new BlueColorSubItem(
//                                Color.White, orderRow.SubItems[ DatabaseObject.ORDER_TYPE ],
//                                   orderRow.Tag + data.Columns[ DatabaseObject.ORDER_TYPE ].Text );
//                        colorNodes[ csi.Tag ] = csi;
//                    }
//
//                    if(! orderRow.SubItems[ DatabaseObject.LIMIT_PRC ].Text.Equals( orderData[ DatabaseObject.LIMIT_PRC ] ) )
//                    {
//                        orderRow.SubItems[ DatabaseObject.LIMIT_PRC ].Text = orderData[ DatabaseObject.LIMIT_PRC ];
//                        ColorSubItem csi =
//                            new BlueColorSubItem(
//                                Color.White, orderRow.SubItems[ DatabaseObject.LIMIT_PRC ],
//                                   orderRow.Tag + data.Columns[ DatabaseObject.LIMIT_PRC ].Text );
//                        colorNodes[ csi.Tag ] = csi;
//                    }
//
//                    if(! orderRow.SubItems[ DatabaseObject.AVERAGE_PRC ].Text.Equals( orderData[ DatabaseObject.AVERAGE_PRC ] ) )
//                    {
//                        orderRow.SubItems[ DatabaseObject.AVERAGE_PRC ].Text = orderData[ DatabaseObject.AVERAGE_PRC ];
//                        ColorSubItem csi =
//                            new BlueColorSubItem(
//                                Color.White, orderRow.SubItems[ DatabaseObject.AVERAGE_PRC ],
//                                   orderRow.Tag + data.Columns[ DatabaseObject.AVERAGE_PRC ].Text );
//                        colorNodes[ csi.Tag ] = csi;
//                    }
//
//                    if(! orderRow.SubItems[ DatabaseObject.OPEN_QTY ].Text.Equals( orderData[ DatabaseObject.OPEN_QTY ] ) )
//                    {
//                        orderRow.SubItems[ DatabaseObject.OPEN_QTY ].Text = orderData[ DatabaseObject.OPEN_QTY ];
//                        ColorSubItem csi =
//                            new BlueColorSubItem(
//                                Color.White, orderRow.SubItems[ DatabaseObject.OPEN_QTY ],
//                                   orderRow.Tag + data.Columns[ DatabaseObject.OPEN_QTY ].Text );
//                        colorNodes[ csi.Tag ] = csi;
//                    }
//
//                    if(! orderRow.SubItems[ DatabaseObject.RELEASED_QTY ].Text.Equals( orderData[ DatabaseObject.RELEASED_QTY ] ) )
//                    {
//                        orderRow.SubItems[ DatabaseObject.RELEASED_QTY ].Text = orderData[ DatabaseObject.RELEASED_QTY ];
//                        ColorSubItem csi =
//                            new BlueColorSubItem(
//                                Color.White, orderRow.SubItems[ DatabaseObject.RELEASED_QTY ],
//                                   orderRow.Tag + data.Columns[ DatabaseObject.RELEASED_QTY ].Text );
//                        colorNodes[ csi.Tag ] = csi;
//                    }
//
//                    if(! orderRow.SubItems[ DatabaseObject.EXECUTED_QTY ].Text.Equals( orderData[ DatabaseObject.EXECUTED_QTY ] ) )
//                    {
//                        orderRow.SubItems[ DatabaseObject.EXECUTED_QTY ].Text = orderData[ DatabaseObject.EXECUTED_QTY ];
//                        ColorSubItem csi =
//                            new BlueColorSubItem(
//                                Color.White, orderRow.SubItems[ DatabaseObject.EXECUTED_QTY ],
//                                   orderRow.Tag + data.Columns[ DatabaseObject.EXECUTED_QTY ].Text );
//                        colorNodes[ csi.Tag ] = csi;
//                    }
//
//
//                    if(! orderRow.SubItems[ DatabaseObject.RELEASED_VAL ].Text.Equals( orderData[ DatabaseObject.RELEASED_VAL ] ) )
//                    {
//                        orderRow.SubItems[ DatabaseObject.RELEASED_VAL ].Text = orderData[ DatabaseObject.RELEASED_VAL ];
//                        ColorSubItem csi =
//                            new BlueColorSubItem(
//                                Color.White, orderRow.SubItems[ DatabaseObject.RELEASED_VAL ],
//                                   orderRow.Tag + data.Columns[ DatabaseObject.RELEASED_VAL ].Text );
//                        colorNodes[ csi.Tag ] = csi;
//                    }
//
//                    if(! orderRow.SubItems[ DatabaseObject.EXECUTED_VAL ].Text.Equals( orderData[ DatabaseObject.EXECUTED_VAL ] ) )
//                    {
//                        orderRow.SubItems[ DatabaseObject.EXECUTED_VAL ].Text = orderData[ DatabaseObject.EXECUTED_VAL ];
//                        ColorSubItem csi =
//                            new BlueColorSubItem(
//                                Color.White, orderRow.SubItems[ DatabaseObject.EXECUTED_VAL ],
//                                   orderRow.Tag + data.Columns[ DatabaseObject.EXECUTED_VAL ].Text );
//                        colorNodes[ csi.Tag ] = csi;
//                    }
//
//                    if(! orderRow.SubItems[ DatabaseObject.UPDATED_DATETIME ].Text.Equals( orderData.ToLocalTime( DatabaseObject.UPDATED_DATETIME ) ) )
//                    {
//                        orderRow.SubItems[ DatabaseObject.UPDATED_DATETIME ].Text = orderData.ToLocalTime( DatabaseObject.UPDATED_DATETIME );
//                        ColorSubItem csi =
//                            new BlueColorSubItem(
//                                Color.White, orderRow.SubItems[ DatabaseObject.UPDATED_DATETIME ],
//                                   orderRow.Tag + data.Columns[ DatabaseObject.UPDATED_DATETIME ].Text );
//                        colorNodes[ csi.Tag ] = csi;
//                    }
//
//                    if(! orderRow.SubItems[ DatabaseObject.UPDATED_BY ].Text.Equals( orderData[ DatabaseObject.UPDATED_BY ] ) )
//                    {
//                        orderRow.SubItems[ DatabaseObject.UPDATED_BY ].Text = orderData[ DatabaseObject.UPDATED_BY ];
//                        ColorSubItem csi =
//                            new BlueColorSubItem(
//                                Color.White, orderRow.SubItems[ DatabaseObject.UPDATED_BY ],
//                                   orderRow.Tag + data.Columns[ DatabaseObject.UPDATED_BY ].Text );
//                        colorNodes[ csi.Tag ] = csi;
//                    }
//                }
//            }
        }

        private void FireSelectedIndexChanged( object o, GTSelectionChangedEventArgs args )
        {
//            if( args.NewValue )
//            {
//                allocations.BeginUpdate();
//                allocations.BeginNodeListChange();
//                allocations.Nodes.Clear();
//
//                foreach( GTLTreeNode orderNode in data.SelectedNodes )
//                {
//                    OrderData orderData = (OrderData)
//                        DealingEngineViewer.ORDERS[ orderNode.Tag ];
//                        //DealingEngineViewer.ORDERS[ args.TreeNode.Tag ];
//
//                    foreach( AllocationData allocationData in orderData.Allocations )
//                    {
//                        string accountXml = controller.GetDirect(
//                            "account::account_id::" + allocationData[
//                            DatabaseObject.ACCOUNT_ID ] );
//
//                        XmlDocument account = new XmlDocument();
//                        account.LoadXml( accountXml );
//                        AccountData accountData = new AccountData(
//                            account.DocumentElement );
//
//                        GTLTreeNode allocationNode = new GTLTreeNode();
//                        allocationNode.SubItems.AddRange( new GTLSubItem [] {
//                            new GTLSubItem( allocationData[ DatabaseObject.ALLOCATION_ID ] ),
//                            new GTLSubItem( allocationData[ DatabaseObject.ORDER_ID ] ),
//                            new GTLSubItem( accountData[ DatabaseObject.NAME ] ),
//                            new GTLSubItem( allocationData[ DatabaseObject.ALLOCATION_QTY ] ),
//                            new GTLSubItem( allocationData[ DatabaseObject.EXECUTED_QTY] ),
//                            new GTLSubItem( allocationData[ DatabaseObject.AVERAGE_PRC ] )
//                        } );
//
//                        allocations.Nodes.Add( allocationNode );
//                    }
//                }
//
//                allocations.EndNodeListChange();
//        		allocations.EndUpdate();
//            }
        }
    }
}
