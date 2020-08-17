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
    public sealed class AccountLimitList : UserListControl
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( AccountLimitList ) );

        private readonly ClientController controller;

        public AccountLimitList( ClientController controller )
		{
		    this.controller = controller;
		    this.SuspendLayout();
		    this.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Name = "AccountLimitList";
			InitializeComponents();
			this.ResumeLayout( false );

            this.controller.Dispatcher.NavigationPaneChanged +=
                new EventHandler<StringEventArgs>( FireNavigationPaneChanged );

            this.Enabled = false;

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

            data = ControlHelper.NewGlacialTreeList( "AccountLimitListData" );
            data.AlternatingColors = true;
            data.BeforeCellEdit += new GTLEmbeddedControlEventHandler( FireBeforeCellEdit );
			data.AfterCellEdit += new GTLEmbeddedControlEventHandler( FireAfterCellEdit );

			//
            //  Tree List columns
            //

            GTLColumn accountId = new GTLColumn();
            accountId.CheckBoxes = false;
			accountId.ImageIndex = -1;
			accountId.Name = DatabaseObject.ACCOUNT_ID;
			accountId.Text = "Account ID";
			accountId.SortType = SortTypes.InsertionSort;
			accountId.Visible = false;

			GTLColumn symbol = new GTLColumn();
            symbol.CheckBoxes = false;
			symbol.ImageIndex = -1;
			symbol.Name = DatabaseObject.SYMBOL;
			symbol.Text = "Symbol";
			symbol.SortType = SortTypes.InsertionSort;
			symbol.Width = 150;

			GTLColumn securityExchange = new GTLColumn();
            securityExchange.CheckBoxes = false;
			securityExchange.ImageIndex = -1;
			securityExchange.Name = DatabaseObject.SECURITY_EXCHANGE;
			securityExchange.Text = "Exchange";
			securityExchange.SortType = SortTypes.InsertionSort;
			securityExchange.Width = 150;

			GTLColumn fixSessionId = new GTLColumn();
            fixSessionId.CheckBoxes = false;
			fixSessionId.ImageIndex = -1;
			fixSessionId.Name = DatabaseObject.SESSION_ID;
			fixSessionId.Text = "FIX Session ID";
			fixSessionId.SortType = SortTypes.InsertionSort;
			fixSessionId.Width = 150;
			fixSessionId.EmbeddedDisplayConditions = GEmbeddedDisplayConditions.DoubleClick;
			fixSessionId.EmbeddedControlType = GEmbeddedControlTypes.TextBox;

			GTLColumn max_open_long_qty = new GTLColumn();
            max_open_long_qty.CheckBoxes = false;
			max_open_long_qty.ImageIndex = -1;
			max_open_long_qty.Name = DatabaseObject.MAX_OPEN_LONG_QTY;
			max_open_long_qty.Text = "Max Open Long Qty";
			max_open_long_qty.TextAlignment = ContentAlignment.MiddleRight;
			max_open_long_qty.SortType = SortTypes.InsertionSort;
            max_open_long_qty.Width = 150;
            max_open_long_qty.EmbeddedDisplayConditions = GEmbeddedDisplayConditions.DoubleClick;
			max_open_long_qty.EmbeddedControlType = GEmbeddedControlTypes.TextBox;

			GTLColumn max_open_short_qty = new GTLColumn();
            max_open_short_qty.CheckBoxes = false;
			max_open_short_qty.ImageIndex = -1;
			max_open_short_qty.Name = DatabaseObject.MAX_OPEN_SHORT_QTY;
			max_open_short_qty.Text = "Max Open Short Qty";
			max_open_short_qty.TextAlignment = ContentAlignment.MiddleRight;
			max_open_short_qty.SortType = SortTypes.InsertionSort;
			max_open_short_qty.Width = 150;
			max_open_short_qty.EmbeddedDisplayConditions = GEmbeddedDisplayConditions.DoubleClick;
			max_open_short_qty.EmbeddedControlType = GEmbeddedControlTypes.TextBox;

			GTLColumn max_release_qty = new GTLColumn();
            max_release_qty.CheckBoxes = false;
			max_release_qty.ImageIndex = -1;
			max_release_qty.Name = DatabaseObject.MAX_RELEASE_QTY;
			max_release_qty.Text = "Max Release Qty";
			max_release_qty.TextAlignment = ContentAlignment.MiddleRight;
			max_release_qty.SortType = SortTypes.InsertionSort;
			max_release_qty.Width = 150;
			max_release_qty.EmbeddedDisplayConditions = GEmbeddedDisplayConditions.DoubleClick;
			max_release_qty.EmbeddedControlType = GEmbeddedControlTypes.TextBox;

            data.Columns.AddRange(
			    new GTLColumn [] {
			        accountId,
			        symbol,
			        securityExchange,
			        fixSessionId,
			        max_open_long_qty,
			        max_open_short_qty,
			        max_release_qty
                } );

            this.Controls.AddRange( new Control [] { data } );
        }

        private void FireNavigationPaneChanged( object sender, StringEventArgs args )
		{
            if( args.Data.Equals( "AccountComplianceViewer" ) )
            {
                data.BeginUpdate();
                data.BeginNodeListChange();
                data.Nodes.Clear();
                data.EndNodeListChange();
                data.EndUpdate();

                this.controller.Dispatcher.AccountSelected +=
                    new EventHandler<StringEventArgs>( FireAccountSelected );
            }
            else
            {
                this.controller.Dispatcher.AccountSelected -=
                    new EventHandler<StringEventArgs>( FireAccountSelected );
                this.Enabled = false;
            }
		}

        private void FireAccountSelected( object sender, StringEventArgs args )
		{
		    data.BeginUpdate();
            data.BeginNodeListChange();
            data.Nodes.Clear();

            //
            //  Load the selected account limits
            //

            Account account = controller.Cache.GetAccount( args.Data );

            ClientAdapterResponse response = controller.SendRpc(
                new GetAccountLimitsByAccountId( args.Data ) );

            foreach( DatabaseEngineResponse.Types.ResultSet rs in response.DatabaseResponse.ResultSetList )
            {
                foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row row in rs.RowList )
                {
                    Hashtable fields = new Hashtable();

                    foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row.Types.Column col in row.ColumnList )
                    {
                        fields.Add( col.Name, col.Value );
                    }

                    GTLTreeNode instrument_node = new GTLTreeNode( );
                    instrument_node.SubItems.AddRange( new GTLSubItem [] {
                        new GTLSubItem( account[ DatabaseObject.ACCOUNT_ID ] ), //  account_id
                        new GTLSubItem( (string)fields[ DatabaseObject.SYMBOL ] ), // symbol
                        new GTLSubItem( (string)fields[ DatabaseObject.SECURITY_EXCHANGE ] ), //  security_exchange
                        new GTLSubItem( (string)fields[ DatabaseObject.SESSION_ID ] ), //  session_id
                        new GTLSubItem( (string)fields[ DatabaseObject.MAX_OPEN_LONG_QTY ] ), //  max_open_long_qty
                        new GTLSubItem( (string)fields[ DatabaseObject.MAX_OPEN_SHORT_QTY ] ), //  max_open_short_qty
                        new GTLSubItem( (string)fields[ DatabaseObject.MAX_RELEASE_QTY ] ) } );  //  max_release_qty

                    data.Nodes.Add( instrument_node );
                }
            }

            data.EndNodeListChange();
            data.EndUpdate();

            this.Enabled = true;
		}

        private void FireBeforeCellEdit( object source, GTLEmbeddedControlEventArgs args )
		{
		    args.Cancel = true;
		    GTLTreeNode node = args.TreeNode;
		    GTLSubItem sub = node.SubItems[ args.Column ];

		    if( node.SubItems[ args.Column ].Text != null &&
		        node.SubItems[ args.Column ].Text != "" )
		    {
                args.Cancel = false;
		    }
        }

        private void FireAfterCellEdit( object source, GTLEmbeddedControlEventArgs args )
		{
		    args.Cancel = true;
		    ColorSubItem csi = null;
		    GTLTreeNode node = args.TreeNode;
            GTLColumn col = data.Columns[ args.Column ];
            GTLSubItem sub = node.SubItems[ args.Column ];

		    if( col.Name.Equals( DatabaseObject.MAX_OPEN_LONG_QTY ) ||
		        col.Name.Equals( DatabaseObject.MAX_OPEN_SHORT_QTY ) ||
		        col.Name.Equals( DatabaseObject.MAX_RELEASE_QTY ) )
			{
			    TextBox tb = (TextBox)args.Control;

			    //
			    //  Ensure that the text box value is an integer greater
			    //  than or equal to zero.
			    //

			    try
			    {
			        int qty = Int32.Parse( tb.Text );

			        if( qty < 0 )
			        {
			            MessageBox.Show(
            			    " Invalid Quantity: " + tb.Text,
            			    "UPDATE Account Limit Error",
            			    MessageBoxButtons.OK,
            			    MessageBoxIcon.Exclamation );

                        data.ClearSelection();

                        csi = new RedColorSubItem( Color.White,
                            node.SubItems[ args.Column ],
                            node.Tag + data.Columns[ args.Column ].Text );

                        colorNodes[ csi.Tag ] = csi;
                        return;
			        }
			    }
			    catch( Exception )
                {
                    MessageBox.Show(
        			    " Invalid Quantity: " + tb.Text,
        			    "UPDATE Account Limit Error",
        			    MessageBoxButtons.OK,
        			    MessageBoxIcon.Exclamation );

                    data.ClearSelection();

                    csi = new RedColorSubItem( Color.White,
                        node.SubItems[ args.Column ],
                        node.Tag + data.Columns[ args.Column ].Text );

                    colorNodes[ csi.Tag ] = csi;
                    return;
                }

                //
                //  Update the account limit
                //

                string account_id,
                    symbol,
                    security_exchange,
                    fix_session_id,
                    max_open_long_qty,
                    max_open_short_qty,
                    max_release_qty;

                account_id = node.SubItems[ DatabaseObject.ACCOUNT_ID ].Text;
                symbol = node.SubItems[ DatabaseObject.SYMBOL ].Text;
                security_exchange = node.SubItems[ DatabaseObject.SECURITY_EXCHANGE ].Text;
                fix_session_id = node.SubItems[ DatabaseObject.SESSION_ID ].Text;

                if( col.Name.Equals( DatabaseObject.MAX_OPEN_LONG_QTY ) )
                {
                    max_open_long_qty = tb.Text;
                    max_open_short_qty = node.SubItems[ DatabaseObject.MAX_OPEN_SHORT_QTY ].Text;
                    max_release_qty = node.SubItems[ DatabaseObject.MAX_RELEASE_QTY ].Text;
                }
                else if( col.Name.Equals( DatabaseObject.MAX_OPEN_SHORT_QTY ) )
                {
                    max_open_long_qty = node.SubItems[ DatabaseObject.MAX_OPEN_LONG_QTY ].Text;
                    max_open_short_qty = tb.Text;
                    max_release_qty = node.SubItems[ DatabaseObject.MAX_RELEASE_QTY ].Text;
                }
                else
                {
                    max_open_long_qty = node.SubItems[ DatabaseObject.MAX_OPEN_LONG_QTY ].Text;
                    max_open_short_qty = node.SubItems[ DatabaseObject.MAX_OPEN_SHORT_QTY ].Text;
                    max_release_qty = tb.Text;
                }

                ClientAdapterResponse response =
                    controller.SendRpc( new UpdateAccountLimit(
                        account_id,
                        symbol,
                        security_exchange,
                        fix_session_id,
                        max_open_long_qty,
                        max_open_short_qty,
                        max_release_qty,
                        "1" ) );

                response = controller.SendRpc( new ReloadStaticData( ) );

                csi = new BlueColorSubItem( Color.White, sub,
                    node.Tag + data.Columns[ args.Column ].Text );

                colorNodes[ csi.Tag ] = csi;

                args.Cancel = false;

                data.ClearSelection();
			}

			if( col.Name.Equals( DatabaseObject.SESSION_ID ) )
			{
                TextBox tb = (TextBox)args.Control;

                if( "".Equals( tb.Text ) )
                {
                    MessageBox.Show(
        			    " Invalid FIX Session ID",
        			    "UPDATE Account Limit Error",
        			    MessageBoxButtons.OK,
        			    MessageBoxIcon.Exclamation );

                    data.ClearSelection();

                    csi = new RedColorSubItem( Color.White,
                        node.SubItems[ args.Column ],
                        node.Tag + data.Columns[ args.Column ].Text );

                    colorNodes[ csi.Tag ] = csi;
                    return;
			    }

			    //
                //  Update the account limit
                //

			    ClientAdapterResponse response =
                    controller.SendRpc( new UpdateAccountLimit(
                        node.SubItems[ DatabaseObject.ACCOUNT_ID ].Text,
                        node.SubItems[ DatabaseObject.SYMBOL ].Text,
                        node.SubItems[ DatabaseObject.SECURITY_EXCHANGE ].Text,
                        tb.Text,
                        node.SubItems[ DatabaseObject.MAX_OPEN_LONG_QTY ].Text,
                        node.SubItems[ DatabaseObject.MAX_OPEN_SHORT_QTY ].Text,
                        node.SubItems[ DatabaseObject.MAX_RELEASE_QTY ].Text,
                        "1" ) );

                response = controller.SendRpc( new ReloadStaticData( ) );

                csi = new BlueColorSubItem( Color.White, sub,
                    node.Tag + data.Columns[ args.Column ].Text );

                colorNodes[ csi.Tag ] = csi;

                args.Cancel = false;

                data.ClearSelection();
			}
		}
    }
}
