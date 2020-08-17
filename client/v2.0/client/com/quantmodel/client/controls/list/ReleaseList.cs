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

namespace com.quantmodel.client.controls.list
{
    public sealed class ReleaseList : UserListControl
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( ReleaseList ) );

        private readonly ClientController controller;

        public ReleaseList( ClientController controller )
		{
		    this.controller = controller;

		    this.SuspendLayout();
		    this.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Name = "ReleaseList";
			InitializeComponents();
			this.ResumeLayout( false );

            LoadReleases( controller.Cache.Releases );

            controller.Dispatcher.Rollover += new EventHandler<DealingEventArgs>( FireRollover );
            controller.Dispatcher.ReleaseAdded += new EventHandler<DealingEventArgs>( FireReleaseAdded );
            controller.Dispatcher.ReleaseChanged += new EventHandler<DealingEventArgs>( FireReleaseChanged );

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

            data = ControlHelper.NewGlacialTreeList( "ReleaseListData" );
            data.MultiSelect = true;
            data.BeforeCellEdit += new GTLEmbeddedControlEventHandler( FireBeforeCellEdit );
			data.AfterCellEdit += new GTLEmbeddedControlEventHandler( FireAfterCellEdit );

            //
            //  Tree List columns
            //

            GTLColumn release_id = new GTLColumn();
            release_id.BackColor = Color.White;
            release_id.CheckBoxes = false;
			release_id.ImageIndex = -1;
			release_id.Name = DatabaseObject.RELEASE_ID;
			release_id.Text = "Release ID";
			release_id.SortType = SortTypes.InsertionSort;
			release_id.ItemComparerType = ItemComparerTypes.Custom;
			release_id.ComparerHelper = new CompareById( "R" );
			release_id.SortDirection = GSortDirectionTypes.Descending;

            GTLColumn order_id = new GTLColumn();
            order_id.BackColor = Color.White;
            order_id.CheckBoxes = false;
			order_id.ImageIndex = -1;
			order_id.Name = DatabaseObject.ORDER_ID;
			order_id.Text = "Order ID";
			order_id.SortType = SortTypes.InsertionSort;

            GTLColumn investment_system_id = new GTLColumn();
            investment_system_id.BackColor = Color.White;
            investment_system_id.CheckBoxes = false;
			investment_system_id.ImageIndex = -1;
			investment_system_id.Name = DatabaseObject.INVESTMENT_SYSTEM_ID;
			investment_system_id.Text = "InvSys ID";
			investment_system_id.SortType = SortTypes.InsertionSort;
			investment_system_id.Visible = false;

            GTLColumn instrument_id = new GTLColumn();
            instrument_id.BackColor = Color.White;
            instrument_id.CheckBoxes = false;
			instrument_id.ImageIndex = -1;
			instrument_id.Name = DatabaseObject.INSTRUMENT_ID;
			instrument_id.Text = "Instrument ID";
			instrument_id.SortType = SortTypes.InsertionSort;
			instrument_id.Visible = false;

			GTLColumn account_name = new GTLColumn();
            account_name.BackColor = Color.White;
            account_name.CheckBoxes = false;
            account_name.ImageIndex = -1;
            account_name.Name = "account_name";
            account_name.Text = "Account Name";
            account_name.SortType = SortTypes.InsertionSort;

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

            GTLColumn client_order_id = new GTLColumn();
            client_order_id.BackColor = Color.White;
            client_order_id.CheckBoxes = false;
			client_order_id.ImageIndex = -1;
			client_order_id.Name = DatabaseObject.CLIENT_ORDER_ID;
			client_order_id.Text = "Client Order ID";
			client_order_id.SortType = SortTypes.InsertionSort;

			GTLColumn session_id = new GTLColumn();
            session_id.BackColor = Color.White;
            session_id.CheckBoxes = false;
			session_id.ImageIndex = -1;
			session_id.Name = DatabaseObject.SESSION_ID;
			session_id.Text = "Session ID";
			session_id.SortType = SortTypes.InsertionSort;

            GTLColumn counterparty_order_id = new GTLColumn();
            counterparty_order_id.BackColor = Color.White;
            counterparty_order_id.CheckBoxes = false;
			counterparty_order_id.ImageIndex = -1;
			counterparty_order_id.Name = DatabaseObject.COUNTERPARTY_ORDER_ID;
			counterparty_order_id.Text = "Cpty Order ID";
			counterparty_order_id.SortType = SortTypes.InsertionSort;

            GTLColumn release_status = new GTLColumn();
            release_status.BackColor = Color.White;
            release_status.CheckBoxes = false;
			release_status.ImageIndex = -1;
			release_status.Name = DatabaseObject.RELEASE_STATUS;
			release_status.Text = "Release Status";
			release_status.SortType = SortTypes.InsertionSort;

			GTLColumn release_type = new GTLColumn();
            release_type.BackColor = Color.White;
            release_type.CheckBoxes = false;
			release_type.ImageIndex = -1;
			release_type.Name = DatabaseObject.RELEASE_TYPE;
			release_type.Text = "Release Type";
			release_type.SortType = SortTypes.InsertionSort;

			GTLColumn release_qty = new GTLColumn();
            release_qty.CheckBoxes = false;
			release_qty.ImageIndex = -1;
			release_qty.Name = DatabaseObject.RELEASE_QTY;
			release_qty.Text = "Release Qty";
			release_qty.TextAlignment = ContentAlignment.MiddleRight;
			release_qty.SortType = SortTypes.InsertionSort;
			//release_qty.EmbeddedDisplayConditions = GEmbeddedDisplayConditions.DoubleClick;
			//release_qty.EmbeddedControlType = GEmbeddedControlTypes.TextBox;

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
			stop_prc.EmbeddedDisplayConditions = GEmbeddedDisplayConditions.DoubleClick;
			stop_prc.EmbeddedControlType = GEmbeddedControlTypes.TextBox;

            GTLColumn average_prc = new GTLColumn();
            average_prc.CheckBoxes = false;
			average_prc.ImageIndex = -1;
			average_prc.Name = DatabaseObject.AVERAGE_PRC;
			average_prc.Text = "Average Prc";
			average_prc.TextAlignment = ContentAlignment.MiddleRight;
			average_prc.SortType = SortTypes.InsertionSort;

            GTLColumn executed_val = new GTLColumn();
            executed_val.CheckBoxes = false;
			executed_val.ImageIndex = -1;
			executed_val.Name = DatabaseObject.EXECUTED_VAL;
			executed_val.Text = "Executed Val";
			executed_val.TextAlignment = ContentAlignment.MiddleRight;
			executed_val.SortType = SortTypes.InsertionSort;

            GTLColumn executed_qty = new GTLColumn();
            executed_qty.CheckBoxes = false;
			executed_qty.ImageIndex = -1;
			executed_qty.Name = DatabaseObject.EXECUTED_QTY;
			executed_qty.Text = "Executed Qty";
			executed_qty.TextAlignment = ContentAlignment.MiddleRight;
			executed_qty.SortType = SortTypes.InsertionSort;

            GTLColumn added_datetime = new GTLColumn();
            added_datetime.CheckBoxes = false;
			added_datetime.ImageIndex = -1;
			added_datetime.Name = DatabaseObject.ADDED_DATETIME;
			added_datetime.Text = "Added Datetime";
			added_datetime.TextAlignment = ContentAlignment.MiddleRight;
			added_datetime.SortType = SortTypes.InsertionSort;
			added_datetime.SortDirection = GSortDirectionTypes.Descending;

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
                    release_id,
                    release_status,
                    release_type,
                    order_id,
                    investment_system_id,
                    instrument_id,
                    account_name,
                    instrument_name,
                    symbol,
                    client_order_id,
                    session_id,
                    counterparty_order_id,
                    release_qty,
                    executed_qty,
                    limit_prc,
                    stop_prc,
                    average_prc,
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

			MenuItem cancel = new MenuItem( "CANCEL RELEASE",
			    new EventHandler( FireCancelRelease )  );

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
			    cancel,
			    new MenuItem( "-" ),
			    columnItems
			} );

            data.ContextMenu = ctxMenu;

			this.Controls.AddRange( new Control [] { data } );
        }

        private void FireReleaseAdded( object o, DealingEventArgs e )
        {
            Release release = (Release)e.Data;
            GTLTreeNode releaseNode = CreateReleaseNode( release );

            data.BeginUpdate();
            data.BeginNodeListChange();

            if( null != releaseNode )
            {
                data.Nodes.Add( releaseNode );
            }

            SortData( data.Columns[ DatabaseObject.RELEASE_ID ] );

            data.EndNodeListChange();
            data.EndUpdate();

            for( int i=0; i<releaseNode.SubItems.Count; i++ )
            {
                GTLSubItem sub = releaseNode.SubItems[ i ];
                ColorSubItem csi =
                    new BlueColorSubItem(
                        Color.White, releaseNode.SubItems[ i ],
                        releaseNode.Tag + data.Columns[ i ].Text );
                colorNodes[ csi.Tag ] = csi;
            }
        }

        private void FireReleaseChanged( object o, DealingEventArgs e )
        {
            Release release = (Release)e.Data;
            Release oldRelease = (Release)e.OldData;

            GTLTreeNode releaseNode = null;
            foreach( GTLTreeNode node in data.Nodes )
            {
                if( node.Tag.Equals( release.Id ) )
                {
                    releaseNode = node;
                    break;
                }
            }

            if( null != releaseNode )
            {
                //
                //  Update the columns that can change on a release:
                //
                //      RELEASE_STATUS
                //      RELEASE_TYPE
                //      CLIENT_ORDER_ID
                //      COUNTERPARTY_ORDER_ID
                //      LIMIT_PRC
                //      STOP_PRC
                //      AVERAGE_PRC
                //      RELEASE_QTY
                //      EXECUTED_QTY
                //      EXECUTED_VAL
                //      UPDATED_DATETIME
                //      UPDATED_BY
                //

                FireSubItem( releaseNode, DatabaseObject.RELEASE_STATUS, release.ReleaseStatus( ) );
                FireSubItem( releaseNode, DatabaseObject.RELEASE_TYPE, release.ReleaseType( ) );
                FireSubItem( releaseNode, DatabaseObject.CLIENT_ORDER_ID, release[ DatabaseObject.CLIENT_ORDER_ID ] );
                FireSubItem( releaseNode, DatabaseObject.COUNTERPARTY_ORDER_ID, release[ DatabaseObject.COUNTERPARTY_ORDER_ID ] );
                FireSubItem( releaseNode, DatabaseObject.LIMIT_PRC, release[ DatabaseObject.LIMIT_PRC ] );
                FireSubItem( releaseNode, DatabaseObject.STOP_PRC, release[ DatabaseObject.STOP_PRC ] );
                FireSubItem( releaseNode, DatabaseObject.AVERAGE_PRC, release[ DatabaseObject.AVERAGE_PRC ] );
                FireSubItem( releaseNode, DatabaseObject.RELEASE_QTY, release[ DatabaseObject.RELEASE_QTY ] );
                FireSubItem( releaseNode, DatabaseObject.EXECUTED_QTY, release[ DatabaseObject.EXECUTED_QTY ] );
                FireSubItem( releaseNode, DatabaseObject.EXECUTED_VAL, release[ DatabaseObject.EXECUTED_VAL ] );
                FireSubItem( releaseNode, DatabaseObject.UPDATED_DATETIME, release.ToLocalTime( DatabaseObject.UPDATED_DATETIME ) );
                FireSubItem( releaseNode, DatabaseObject.UPDATED_BY, release[ DatabaseObject.UPDATED_BY ] );
            }
        }

        private void FireBeforeCellEdit( object source, GTLEmbeddedControlEventArgs args )
		{
		    args.Cancel = true;
		    GTLTreeNode node = args.TreeNode;

		    //
		    //  You can cancel replace as long as it's
		    //
		    //      'NEW'
		    //      'PARTIALLY_FILLED'
		    //      'REPLACED'
		    //

            string status = node.SubItems[ DatabaseObject.RELEASE_STATUS ].Text;
		    if( status.Equals( "NEW" ) ||
		        status.Equals( "PARTIALLY_FILLED" ) ||
		        status.Equals( "REPLACED" ) )
            {
    		    GTLColumn col = data.Columns[ args.Column ];

    		    if( col.Name.Equals( DatabaseObject.LIMIT_PRC ) ||
    		        col.Name.Equals( DatabaseObject.STOP_PRC ) )
    			{
    			    args.Cancel = false;
    			}
            }
		}

        private void FireAfterCellEdit( object source, GTLEmbeddedControlEventArgs args )
		{
		    args.Cancel = true;
		    GTLTreeNode node = args.TreeNode;
            GTLColumn col = data.Columns[ args.Column ];

            if(! controller.Cache.HasRelease( node.Tag ) )
            {
                MessageBox.Show( "Unknown release: " + node.Tag );
                data.ClearSelection();
                return;
            }

            string release_type = node.SubItems[ DatabaseObject.RELEASE_TYPE ].Text;

		    if( col.Name.Equals( DatabaseObject.LIMIT_PRC ) &&
		      ( release_type.Equals( "LIMIT" ) || release_type.Equals( "STOP_LIMIT" ) ) )
            {
                //
                //  Get the current release
                //

                Release release = controller.Cache.GetRelease( node.Tag );
                TextBox tb = (TextBox)args.Control;

                //
                //  Ensure something changed...
                //

                double old_limit_prc = release.ToDouble( DatabaseObject.LIMIT_PRC );

                try
                {
                    //
                    //  Test for a valid number
                    //

                    double limit_prc = Double.Parse( tb.Text );

                    //
                    //  Ensure new limit price is >= 0 and != old limit price
                    //

                    if( limit_prc >= 0 && limit_prc != old_limit_prc )
                    {
                        controller.SendRpc(
                            new CancelReplaceRelease(
                                release.Id,
                                release[ DatabaseObject.STOP_PRC ],
                                Convert.ToString( limit_prc ) ) );
                    }
                }
                catch( Exception )
                {
                    MessageBox.Show(
        			    " Invalid Limit Price: " + tb.Text,
        			    "REPLACE RELEASE Error",
        			    MessageBoxButtons.OK,
        			    MessageBoxIcon.Exclamation );
                }

                data.ClearSelection();
            }

            if( col.Name.Equals( DatabaseObject.STOP_PRC ) &&
              ( release_type.Equals( "STOP" ) || release_type.Equals( "STOP_LIMIT" ) ) )
            {
                //
                //  Get the current release
                //

                Release release = controller.Cache.GetRelease( node.Tag );
                TextBox tb = (TextBox)args.Control;

                //
                //  Ensure something changed...
                //

                double old_stop_prc = release.ToDouble( DatabaseObject.STOP_PRC );

                try
                {
                    //
                    //  Test for a valid number
                    //

                    double stop_prc = Double.Parse( tb.Text );

                    //
                    //  Ensure new limit price is >= 0 and != old limit price
                    //

                    if( stop_prc >= 0 && stop_prc != old_stop_prc )
                    {
                        controller.SendRpc(
                            new CancelReplaceRelease(
                                release.Id,
                                Convert.ToString( stop_prc ),
                                release[ DatabaseObject.LIMIT_PRC ] ) );
                    }
                }
                catch( Exception )
                {
                    MessageBox.Show(
        			    " Invalid Stop Price: " + tb.Text,
        			    "REPLACE RELEASE Error",
        			    MessageBoxButtons.OK,
        			    MessageBoxIcon.Exclamation );
                }

                data.ClearSelection();
            }
		}

        private void FireCancelRelease( object o, EventArgs e )
        {
            string message = null;

            if( data.SelectedNodes.Count > 1 )
            {
                message = "Are you sure you want to CANCEL these releases?";
            }
            else
            {
    			message = "Are you sure you want to CANCEL this release?";
    	    }

    	    DialogResult result = MessageBox.Show(
    			    message,
    			    "CANCEL Release Confirmation",
    			    MessageBoxButtons.YesNo,
    			    MessageBoxIcon.Exclamation );

            if( result == System.Windows.Forms.DialogResult.Yes )
			{
                //
                //  Get the selected release(s)
                //

                foreach( GTLTreeNode releaseNode in data.SelectedNodes )
                {
                    if( controller.Cache.HasRelease( releaseNode.Tag ) )
                    {
                        Release release = controller.Cache.GetRelease( releaseNode.Tag );

                        if( "NEW".Equals( release.ReleaseStatus() ) ||
                            "PARTIALLY_FILLED".Equals( release.ReleaseStatus() ) ||
                            "PENDING_REPLACE".Equals( release.ReleaseStatus() ) ||
                            "REPLACED".Equals( release.ReleaseStatus() ) )
                        {
                            ClientAdapterResponse response =
                                controller.SendRpc( new CancelRelease( release.Id ) );
                        }
                        else
                        {
                            MessageBox.Show(
                			    release.Id + " Invalid Release Status: " +
                			        release.ReleaseStatus(),
                			    "CANCEL Release Error",
                			    MessageBoxButtons.OK,
                			    MessageBoxIcon.Exclamation );

                            data.ClearSelection();
                            return;
                        }
                    }
                    else
                    {
                        MessageBox.Show(
                            " Unknown Release: '" + releaseNode.Tag + "'",
            			    "CANCEL Release Error",
            			    MessageBoxButtons.OK,
            			    MessageBoxIcon.Exclamation );

                        data.ClearSelection();
                        return;
                    }
                }

                data.ClearSelection();
            }
        }

        private void LoadReleases( ArrayList releases )
        {
            data.BeginUpdate();
            data.BeginNodeListChange();

            foreach( Release release in releases )
            {
                GTLTreeNode releaseNode = CreateReleaseNode( release );

                if( null != releaseNode )
                {
                    data.Nodes.Add( releaseNode );
                }
            }

            SortData( data.Columns[ DatabaseObject.RELEASE_ID ] );

            data.EndNodeListChange();
            data.EndUpdate();
        }

        private GTLTreeNode CreateReleaseNode( Release release )
        {
            //
            //  Get the instrument on the release
            //

            Instrument instrument =
                controller.Cache.GetInstrument(
                    release[ DatabaseObject.INSTRUMENT_ID ] );

            if( null == instrument )
            {
                log.Warn( "Unknown instrument: " +
                    release[ DatabaseObject.INSTRUMENT_ID ] );

                return null;
            }

            //
            //  Get the account on the release
            //

            Account account =
                controller.Cache.GetAccount(
                    release[ DatabaseObject.ACCOUNT_ID ] );

            if( null == account )
            {
                log.Warn( "Unknown account: " +
                    release[ DatabaseObject.ACCOUNT_ID ] );

                return null;
            }

            if(! controller.Cache.HasInvestmentSystem(
                 release[ DatabaseObject.INVESTMENT_SYSTEM_ID ] ) )
            {
                log.Warn( "Unknown investment_system: " +
                    release[ DatabaseObject.INVESTMENT_SYSTEM_ID ] );

                return null;
            }

            GTLTreeNode releaseNode = new GTLTreeNode( release.Id );
            releaseNode.Checked = false;
            releaseNode.Collapse();
            releaseNode.Tag = release.Id;

            releaseNode.SubItems.AddRange( new GTLSubItem [] {
                new GTLSubItem( release.ReleaseStatus( ) ),
                new GTLSubItem( release.ReleaseType( ) ),
                new GTLSubItem( release[ DatabaseObject.ORDER_ID ] ),
                new GTLSubItem( release[ DatabaseObject.INVESTMENT_SYSTEM_ID ] ),
                new GTLSubItem( release[ DatabaseObject.INSTRUMENT_ID ] ),
                new GTLSubItem( account[ DatabaseObject.NAME ] ), // account_name
                new GTLSubItem( instrument[ DatabaseObject.NAME ] ), // instrument_name
                new GTLSubItem( instrument[ DatabaseObject.SYMBOL ] ), // symbol
                new GTLSubItem( release[ DatabaseObject.CLIENT_ORDER_ID ] ),
                new GTLSubItem( release[ DatabaseObject.SESSION_ID ] ),
                new GTLSubItem( release[ DatabaseObject.COUNTERPARTY_ORDER_ID ] ),
                new GTLSubItem( release[ DatabaseObject.RELEASE_QTY ] ),
                new GTLSubItem( release[ DatabaseObject.EXECUTED_QTY ] ),
                new GTLSubItem( release[ DatabaseObject.LIMIT_PRC ] ),
                new GTLSubItem( release[ DatabaseObject.STOP_PRC ] ),
                new GTLSubItem( release[ DatabaseObject.AVERAGE_PRC ] ),
                new GTLSubItem( release[ DatabaseObject.EXECUTED_VAL ] ),
                new GTLSubItem( release.ToLocalTime( DatabaseObject.ADDED_DATETIME ) ),
                new GTLSubItem( release[ DatabaseObject.ADDED_BY ] ),
                new GTLSubItem( release.ToLocalTime( DatabaseObject.UPDATED_DATETIME ) ),
                new GTLSubItem( release[ DatabaseObject.UPDATED_BY ] )
            } );

            return releaseNode;
        }
    }
}
