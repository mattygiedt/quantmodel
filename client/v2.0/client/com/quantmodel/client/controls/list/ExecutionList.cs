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

using com.quantmodel.common.data;
using com.quantmodel.common.events;
using com.quantmodel.common.controls;
using com.quantmodel.common.network.message;

using com.quantmodel.client.controls.form;

namespace com.quantmodel.client.controls.list
{
    public sealed class ExecutionList : UserListControl
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( ExecutionList ) );

        private readonly ClientController controller;
        
        private readonly CreateBlotterForm createBlotterForm;

        public ExecutionList( ClientController controller )
		{
		    this.controller = controller;

		    this.SuspendLayout();
		    this.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Name = "ExecutionList";
			InitializeComponents();
			this.ResumeLayout( false );
            
            createBlotterForm = new CreateBlotterForm( controller );
			createBlotterForm.Size = new Size( 450, 200 );
            createBlotterForm.FormBorderStyle = FormBorderStyle.Sizable;
            createBlotterForm.StartPosition = FormStartPosition.CenterScreen;

            LoadExecutions( controller.Cache.Executions );

            controller.Dispatcher.Rollover += new EventHandler<DealingEventArgs>( FireRollover );
            controller.Dispatcher.ExecutionAdded += new EventHandler<DealingEventArgs>( FireExecutionAdded );
            controller.Dispatcher.ExecutionChanged += new EventHandler<DealingEventArgs>( FireExecutionChanged );

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

            data = ControlHelper.NewGlacialTreeList( "ExecutionListData" );

            //
            //  Tree List columns
            //

            GTLColumn execution_id = new GTLColumn();
            execution_id.BackColor = Color.White;
            execution_id.CheckBoxes = false;
			execution_id.ImageIndex = -1;
			execution_id.Name = DatabaseObject.EXECUTION_ID;
			execution_id.Text = "Execution ID";
			execution_id.SortType = SortTypes.InsertionSort;
			execution_id.ItemComparerType = ItemComparerTypes.Custom;
			execution_id.ComparerHelper = new CompareById( "E" );
			execution_id.SortDirection = GSortDirectionTypes.Descending;

            GTLColumn order_id = new GTLColumn();
            order_id.BackColor = Color.White;
            order_id.CheckBoxes = false;
			order_id.ImageIndex = -1;
			order_id.Name = DatabaseObject.ORDER_ID;
			order_id.Text = "Order ID";
			order_id.SortType = SortTypes.InsertionSort;

            GTLColumn release_id = new GTLColumn();
            release_id.BackColor = Color.White;
            release_id.CheckBoxes = false;
			release_id.ImageIndex = -1;
			release_id.Name = DatabaseObject.RELEASE_ID;
			release_id.Text = "Release ID";
			release_id.SortType = SortTypes.InsertionSort;

            GTLColumn investment_system_id = new GTLColumn();
            investment_system_id.BackColor = Color.White;
            investment_system_id.CheckBoxes = false;
			investment_system_id.ImageIndex = -1;
			investment_system_id.Name = DatabaseObject.INVESTMENT_SYSTEM_ID;
			investment_system_id.Text = "InvSys ID";
			investment_system_id.SortType = SortTypes.InsertionSort;

            GTLColumn instrument_id = new GTLColumn();
            instrument_id.BackColor = Color.White;
            instrument_id.CheckBoxes = false;
			instrument_id.ImageIndex = -1;
			instrument_id.Name = DatabaseObject.INSTRUMENT_ID;
			instrument_id.Text = "Instrument ID";
			instrument_id.SortType = SortTypes.InsertionSort;

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
			session_id.Name = DatabaseObject.COUNTERPARTY_ID;
			session_id.Text = "Session ID";
			session_id.SortType = SortTypes.InsertionSort;

            GTLColumn counterparty_execution_id = new GTLColumn();
            counterparty_execution_id.BackColor = Color.White;
            counterparty_execution_id.CheckBoxes = false;
			counterparty_execution_id.ImageIndex = -1;
			counterparty_execution_id.Name = DatabaseObject.COUNTERPARTY_EXECUTION_ID;
			counterparty_execution_id.Text = "Cpty Execution ID";
			counterparty_execution_id.SortType = SortTypes.InsertionSort;

            GTLColumn side_code = new GTLColumn();
            side_code.BackColor = Color.White;
            side_code.CheckBoxes = false;
			side_code.ImageIndex = -1;
			side_code.Name = DatabaseObject.SIDE_CODE;
			side_code.Text = "Side Code";
			side_code.SortType = SortTypes.InsertionSort;

            GTLColumn execution_ccy = new GTLColumn();
            execution_ccy.BackColor = Color.White;
            execution_ccy.CheckBoxes = false;
			execution_ccy.ImageIndex = -1;
			execution_ccy.Name = DatabaseObject.EXECUTION_CCY;
			execution_ccy.Text = "Execution Ccy";
			execution_ccy.SortType = SortTypes.InsertionSort;

            GTLColumn execution_status = new GTLColumn();
            execution_status.BackColor = Color.White;
            execution_status.CheckBoxes = false;
			execution_status.ImageIndex = -1;
			execution_status.Name = DatabaseObject.EXECUTION_STATUS;
			execution_status.Text = "Execution Status";
			execution_status.SortType = SortTypes.InsertionSort;

            GTLColumn execution_prc = new GTLColumn();
            execution_prc.CheckBoxes = false;
			execution_prc.ImageIndex = -1;
			execution_prc.Name = DatabaseObject.EXECUTION_PRC;
			execution_prc.Text = "Execution Prc";
			execution_prc.TextAlignment = ContentAlignment.MiddleRight;
			execution_prc.SortType = SortTypes.InsertionSort;

            GTLColumn execution_qty = new GTLColumn();
            execution_qty.CheckBoxes = false;
			execution_qty.ImageIndex = -1;
			execution_qty.Name = DatabaseObject.EXECUTION_QTY;
			execution_qty.Text = "Execution Qty";
			execution_qty.TextAlignment = ContentAlignment.MiddleRight;
			execution_qty.SortType = SortTypes.InsertionSort;

            GTLColumn execution_val = new GTLColumn();
            execution_val.CheckBoxes = false;
			execution_val.ImageIndex = -1;
			execution_val.Name = DatabaseObject.EXECUTION_VAL;
			execution_val.Text = "Execution Val";
			execution_val.TextAlignment = ContentAlignment.MiddleRight;
			execution_val.SortType = SortTypes.InsertionSort;

            GTLColumn leaves_qty = new GTLColumn();
            leaves_qty.CheckBoxes = false;
			leaves_qty.ImageIndex = -1;
			leaves_qty.Name = DatabaseObject.LEAVES_QTY;
			leaves_qty.Text = "Leaves Qty";
			leaves_qty.TextAlignment = ContentAlignment.MiddleRight;
			leaves_qty.SortType = SortTypes.InsertionSort;

            GTLColumn average_prc = new GTLColumn();
            average_prc.CheckBoxes = false;
			average_prc.ImageIndex = -1;
			average_prc.Name = DatabaseObject.AVERAGE_PRC;
			average_prc.Text = "Average Prc";
			average_prc.TextAlignment = ContentAlignment.MiddleRight;
			average_prc.SortType = SortTypes.InsertionSort;

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
			added_by.TextAlignment = ContentAlignment.MiddleRight;
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
			updated_by.TextAlignment = ContentAlignment.MiddleRight;
			updated_by.SortType = SortTypes.InsertionSort;

			data.Columns.AddRange(
			    new GTLColumn [] {
                    execution_id,
                    execution_status,
                    order_id,
                    release_id,
                    investment_system_id,
                    instrument_id,
                    account_name,
                    instrument_name,
                    symbol,
                    client_order_id,
                    session_id,
                    counterparty_execution_id,
                    side_code,
                    execution_ccy,
                    execution_prc,
                    execution_qty,
                    execution_val,
                    leaves_qty,
                    average_prc,
                    added_datetime,
                    added_by,
                    updated_datetime,
                    updated_by
			    } );
            
            //
			//  Context Menu for blotter creation
			//

			ContextMenu ctxMenu = new ContextMenu();

			MenuItem create = new MenuItem( "CREATE BLOTTER",
			    new EventHandler( FireCreateBlotter )  );
            
            ctxMenu.MenuItems.AddRange( new MenuItem []
			{
                create
            } );
            
            data.ContextMenu = ctxMenu;
            
			this.Controls.AddRange( new Control [] { data } );
        }
        
        private void FireCreateBlotter( object o, EventArgs e )
        {
            createBlotterForm.Show();
        }

        private void FireExecutionAdded( object o, DealingEventArgs e )
        {
            Execution execution = (Execution)e.Data;
            GTLTreeNode executionNode = CreateExecutionNode( execution );

            data.BeginUpdate();
            data.BeginNodeListChange();

            if( null != executionNode )
            {
                data.Nodes.Add( executionNode );
            }

            SortData( data.Columns[ DatabaseObject.EXECUTION_ID ] );

            data.EndNodeListChange();
            data.EndUpdate();

            for( int i=0; i<executionNode.SubItems.Count; i++ )
            {
                GTLSubItem sub = executionNode.SubItems[ i ];
                ColorSubItem csi =
                    new BlueColorSubItem(
                        Color.White, executionNode.SubItems[ i ],
                        executionNode.Tag + data.Columns[ i ].Text );
                colorNodes[ csi.Tag ] = csi;
            }
        }

        private void FireExecutionChanged( object o, DealingEventArgs e )
        {
            Execution execution = (Execution)e.Data;
            Execution oldExecution = (Execution)e.OldData;

            GTLTreeNode executionNode = null;
            foreach( GTLTreeNode node in data.Nodes )
            {
                if( node.Tag.Equals( execution.Id ) )
                {
                    executionNode = node;
                    break;
                }
            }

            if( null != executionNode )
            {
                //
                //  Update the columns that can change on an execution:
                //

                FireSubItem( executionNode, DatabaseObject.EXECUTION_STATUS, execution.ExecutionStatus() );
                FireSubItem( executionNode, DatabaseObject.UPDATED_DATETIME, execution.ToLocalTime( DatabaseObject.UPDATED_DATETIME ) );
                FireSubItem( executionNode, DatabaseObject.UPDATED_BY, execution[ DatabaseObject.UPDATED_BY ] );
            }
        }

        private void LoadExecutions( ArrayList executions )
        {
            data.BeginUpdate();
            data.BeginNodeListChange();

            foreach( Execution execution in executions )
            {
                GTLTreeNode executionNode = CreateExecutionNode( execution );

                if( null != executionNode )
                {
                    data.Nodes.Add( executionNode );
                }
            }

            SortData( data.Columns[ DatabaseObject.EXECUTION_ID ] );

            data.EndNodeListChange();
            data.EndUpdate();
        }

        private GTLTreeNode CreateExecutionNode( Execution execution )
        {
            //
            //  Get the instrument on the execution
            //

            Instrument instrument =
                controller.Cache.GetInstrument(
                    execution[ DatabaseObject.INSTRUMENT_ID ] );

            if( null == instrument )
            {
                log.Warn( "Unknown instrument: " +
                    execution[ DatabaseObject.INSTRUMENT_ID ] );

                return null;
            }

            //
            //  Get the account on the execution
            //

            Account account =
                controller.Cache.GetAccount(
                    execution[ DatabaseObject.ACCOUNT_ID ] );

            if( null == account )
            {
                log.Warn( "Unknown account: " +
                    execution[ DatabaseObject.ACCOUNT_ID ] );

                return null;
            }

            if(! controller.Cache.HasInvestmentSystem(
                 execution[ DatabaseObject.INVESTMENT_SYSTEM_ID ] ) )
            {
                log.Warn( "Unknown investment_system: " +
                    execution[ DatabaseObject.INVESTMENT_SYSTEM_ID ] );

                return null;
            }

            GTLTreeNode executionNode = new GTLTreeNode( execution.Id );
            executionNode.Checked = false;
            executionNode.Collapse();
            executionNode.Tag = execution.Id;

            executionNode.SubItems.AddRange( new GTLSubItem [] {
                new GTLSubItem( execution.ExecutionStatus( ) ),
                new GTLSubItem( execution[ DatabaseObject.ORDER_ID ] ),
                new GTLSubItem( execution[ DatabaseObject.RELEASE_ID ] ),
                new GTLSubItem( execution[ DatabaseObject.INVESTMENT_SYSTEM_ID ] ),
                new GTLSubItem( execution[ DatabaseObject.INSTRUMENT_ID ] ),
                new GTLSubItem( account[ DatabaseObject.NAME ] ), // account_name
                new GTLSubItem( instrument[ DatabaseObject.NAME ] ), // instrument_name
                new GTLSubItem( instrument[ DatabaseObject.SYMBOL ] ), // symbol
                new GTLSubItem( execution[ DatabaseObject.CLIENT_ORDER_ID ] ),
                new GTLSubItem( execution[ DatabaseObject.SESSION_ID ] ),
                new GTLSubItem( execution[ DatabaseObject.COUNTERPARTY_EXECUTION_ID ] ),
                new GTLSubItem( execution.SideCode( ) ),
                new GTLSubItem( execution[ DatabaseObject.EXECUTION_CCY ] ),
                new GTLSubItem( execution[ DatabaseObject.EXECUTION_PRC ] ),
                new GTLSubItem( execution[ DatabaseObject.EXECUTION_QTY ] ),
                new GTLSubItem( execution[ DatabaseObject.EXECUTION_VAL ] ),
                new GTLSubItem( execution[ DatabaseObject.LEAVES_QTY ] ),
                new GTLSubItem( execution[ DatabaseObject.AVERAGE_PRC ] ),
                new GTLSubItem( execution.ToLocalTime( DatabaseObject.ADDED_DATETIME ) ),
                new GTLSubItem( execution[ DatabaseObject.ADDED_BY ] ),
                new GTLSubItem( execution.ToLocalTime( DatabaseObject.UPDATED_DATETIME ) ),
                new GTLSubItem( execution[ DatabaseObject.UPDATED_BY ] )
            } );

            return executionNode;
        }


        //private void FireLoadExecutions( object sender, MessageEventArgs args )
		//{
        //    XmlNode node = args.Node;
        //
        //    this.SuspendLayout();
        //    data.Nodes.Clear();
        //
        //    foreach( XmlNode execution_node in node.SelectNodes( "Executions/Execution" ) )
        //    {
        //        AddExecution( execution_node );
        //    }
        //
        //    SortData( data.Columns[ DatabaseObject.EXECUTION_ID ] );
        //
        //    this.ResumeLayout( false );
		//	this.PerformLayout();
        //}
        //
        //private void FireCreateExecution( object sender, MessageEventArgs args )
		//{
		//    XmlNode node = args.Node;
        //
        //    this.SuspendLayout();
        //
        //    foreach( XmlNode execution_node in node.SelectNodes( "Execution" ) )
        //    {
        //        AddExecution( execution_node, true );
        //    }
        //
        //    SortData( data.Columns[ data.LastSortColumnIndex ] );
        //
		//	this.ResumeLayout( false );
		//	this.PerformLayout();
	    //}
        //
		//private void FireUpdateExecution( object sender, MessageEventArgs args )
		//{
		//    XmlNode node = args.Node;
        //
        //    this.SuspendLayout();
        //
        //    foreach( XmlNode execution_node in node.SelectNodes( "Execution" ) )
        //    {
        //        UpdateExecution( execution_node );
        //    }
        //
        //    SortData( data.Columns[ data.LastSortColumnIndex ] );
        //
		//	this.ResumeLayout( false );
		//	this.PerformLayout();
	    //}
        //
        //private void AddExecution( XmlNode execution_node )
        //{
        //    AddExecution( execution_node, false );
        //}

	    private void AddExecution( XmlNode execution_node, bool fireColor )
	    {
//	        ExecutionData executionData = new ExecutionData( execution_node );
//
//            if( log.IsDebugEnabled )
//            {
//                log.Debug( "ADDING EXECUTION: " + executionData.Id );
//            }
//
//            if( DealingEngineViewer.EXECUTIONS.ContainsKey( executionData.Id ) )
//            {
//                log.Warn( "DealingEngineViewer.EXECUTIONS already has key: " + executionData.Id );
//                return;
//            }
//            else
//            {
//                DealingEngineViewer.EXECUTIONS.Add( executionData.Id, executionData );
//            }
//
//            //
//            //
//            //
//
//            XmlDocument instrument = new XmlDocument();
//            XmlDocument investmentSystem = new XmlDocument();
//
//            InstrumentData instrumentData = null;
//            if( DealingEngineViewer.INSTRUMENTS.ContainsKey(
//                executionData[ DatabaseObject.INSTRUMENT_ID ] ) )
//            {
//                instrumentData = (InstrumentData)
//                    DealingEngineViewer.INSTRUMENTS[
//                        executionData[ DatabaseObject.INSTRUMENT_ID ] ];
//            }
//            else
//            {
//                string instrumentXml = controller.GetDirect(
//                    "instrument::instrument_id::" + executionData[
//                        DatabaseObject.INSTRUMENT_ID ] );
//
//                instrument.LoadXml( instrumentXml );
//                instrumentData = new InstrumentData(
//                    instrument.DocumentElement );
//
//                DealingEngineViewer.INSTRUMENTS.Add(
//                    instrumentData.Id, instrumentData );
//            }
//
//            InvestmentSystemData invSysData = null;
//            if( DealingEngineViewer.INVESTMENT_SYSTEMS.ContainsKey(
//                executionData[ DatabaseObject.INVESTMENT_SYSTEM_ID ] ) )
//            {
//                invSysData = (InvestmentSystemData)
//                    DealingEngineViewer.INVESTMENT_SYSTEMS[
//                        executionData[ DatabaseObject.INVESTMENT_SYSTEM_ID ] ];
//            }
//            else
//            {
//                string invSysXml = controller.GetDirect(
//                    "investment_system::investment_system_id::" + executionData[
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
//            GTLTreeNode executionRow = new GTLTreeNode( executionData.Id );
//            executionRow.Checked = false;
//            executionRow.Collapse();
//            executionRow.Tag = executionData.Id;
//
//            executionRow.SubItems.AddRange( new GTLSubItem [] {
//                new GTLSubItem( executionData.ExecutionStatus( ) ),
//                new GTLSubItem( executionData[ DatabaseObject.ORDER_ID ] ),
//                new GTLSubItem( executionData[ DatabaseObject.RELEASE_ID ] ),
//                new GTLSubItem( executionData[ DatabaseObject.INVESTMENT_SYSTEM_ID ] ),
//                new GTLSubItem( invSysData[ DatabaseObject.NAME ] ), // investment_system_name
//                new GTLSubItem( executionData[ DatabaseObject.INSTRUMENT_ID ] ),
//                new GTLSubItem( instrumentData[ DatabaseObject.NAME ] ), // instrument_name
//                new GTLSubItem( instrumentData[ DatabaseObject.SYMBOL ] ), // symbol
//                new GTLSubItem( executionData[ DatabaseObject.CLIENT_ORDER_ID ] ),
//                new GTLSubItem( executionData[ DatabaseObject.COUNTERPARTY_ID ] ),
//                new GTLSubItem( executionData[ DatabaseObject.EXECUTING_COUNTERPARTY_ID ] ),
//                new GTLSubItem( executionData[ DatabaseObject.COUNTERPARTY_EXECUTION_ID ] ),
//                new GTLSubItem( executionData.SideCode( ) ),
//                new GTLSubItem( executionData[ DatabaseObject.EXECUTION_CCY ] ),
//                new GTLSubItem( executionData[ DatabaseObject.EXECUTION_PRC ] ),
//                new GTLSubItem( executionData[ DatabaseObject.EXECUTION_QTY ] ),
//                new GTLSubItem( executionData[ DatabaseObject.EXECUTION_VAL ] ),
//                new GTLSubItem( executionData[ DatabaseObject.LEAVES_QTY ] ),
//                new GTLSubItem( executionData[ DatabaseObject.AVERAGE_PRC ] ),
//                new GTLSubItem( executionData.ToLocalTime( DatabaseObject.ADDED_DATETIME ) ),
//                new GTLSubItem( executionData[ DatabaseObject.ADDED_BY ] ),
//                new GTLSubItem( executionData.ToLocalTime( DatabaseObject.UPDATED_DATETIME ) ),
//                new GTLSubItem( executionData[ DatabaseObject.UPDATED_BY ] )
//            } );
//
//			data.BeginUpdate();
//            data.BeginNodeListChange();
//            data.Nodes.Add( executionRow );
//            data.EndNodeListChange();
//		    data.EndUpdate();
//
//		    if( fireColor )
//		    {
//		        for( int i=0; i<executionRow.SubItems.Count; i++ )
//                {
//                    GTLSubItem sub = executionRow.SubItems[ i ];
//                    ColorSubItem csi =
//                        new BlueColorSubItem(
//                            Color.White, executionRow.SubItems[ i ],
//                                executionRow.Tag + data.Columns[ i ].Text );
//                    colorNodes[ csi.Tag ] = csi;
//                }
//            }
	    }

	    private void UpdateExecution( XmlNode execution_node )
	    {
//	        ExecutionData executionData = new ExecutionData( execution_node );
//
//	        if(! DealingEngineViewer.EXECUTIONS.ContainsKey( executionData.Id ) )
//	        {
//	            AddExecution( execution_node, true );
//	            return;
//	        }
//
//	        ExecutionData oldExecutionData = (ExecutionData)DealingEngineViewer.EXECUTIONS[ executionData.Id ];
//
//	        if( string.Compare( executionData[ DatabaseObject.UPDATED_DATETIME ],
//	            oldExecutionData[ DatabaseObject.UPDATED_DATETIME ] ) > 0 )
//            {
//    	        DealingEngineViewer.EXECUTIONS[ executionData.Id ] = executionData;
//
//    	        //
//                //  Search the top level nodes for the execution
//                //
//
//                GTLTreeNode executionRow = null;
//                foreach( GTLTreeNode node in data.Nodes )
//                {
//                    if( node.Tag.Equals( executionData.Id ) )
//                    {
//                        executionRow = node;
//                        break;
//                    }
//                }
//
//                if( null != executionRow )
//                {
//                    //
//                    //  Update the columns that can change on an execution:
//                    //
//                    //      EXECUTION_STATUS
//                    //      UPDATED_BY
//                    //      UPDATED_DATETIME
//                    //
//
//                    if(! executionRow.SubItems[ DatabaseObject.EXECUTION_STATUS ].Text.Equals( executionData.ExecutionStatus( ) ) )
//                    {
//                        executionRow.SubItems[ DatabaseObject.EXECUTION_STATUS ].Text = executionData.ExecutionStatus( );
//                        ColorSubItem csi =
//                            new BlueColorSubItem(
//                                Color.White, executionRow.SubItems[ DatabaseObject.EXECUTION_STATUS ],
//                                   executionRow.Tag + data.Columns[ DatabaseObject.EXECUTION_STATUS ].Text );
//                        colorNodes[ csi.Tag ] = csi;
//                    }
//
//                    if(! executionRow.SubItems[ DatabaseObject.UPDATED_DATETIME ].Text.Equals( executionData[ DatabaseObject.UPDATED_DATETIME ] ) )
//                    {
//                        executionRow.SubItems[ DatabaseObject.UPDATED_DATETIME ].Text = executionData[ DatabaseObject.UPDATED_DATETIME ];
//                        ColorSubItem csi =
//                            new BlueColorSubItem(
//                                Color.White, executionRow.SubItems[ DatabaseObject.UPDATED_DATETIME ],
//                                   executionRow.Tag + data.Columns[ DatabaseObject.UPDATED_DATETIME ].Text );
//                        colorNodes[ csi.Tag ] = csi;
//                    }
//
//                    if(! executionRow.SubItems[ DatabaseObject.UPDATED_BY ].Text.Equals( executionData[ DatabaseObject.UPDATED_BY ] ) )
//                    {
//                        executionRow.SubItems[ DatabaseObject.UPDATED_BY ].Text = executionData[ DatabaseObject.UPDATED_BY ];
//                        ColorSubItem csi =
//                            new BlueColorSubItem(
//                                Color.White, executionRow.SubItems[ DatabaseObject.UPDATED_BY ],
//                                   executionRow.Tag + data.Columns[ DatabaseObject.UPDATED_BY ].Text );
//                        colorNodes[ csi.Tag ] = csi;
//                    }
//                }
//	        }
        }
    }
}
