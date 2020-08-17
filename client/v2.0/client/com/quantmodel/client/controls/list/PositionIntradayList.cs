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

namespace com.quantmodel.client.controls.list
{
    public sealed class PositionIntradayList : UserListControl
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( PositionIntradayList ) );

		public static readonly string double_fmt = "#,##0.0000;(#,##0.0000);0.0000";

        private readonly ClientController controller;

        public PositionIntradayList( ClientController controller )
		{
		    this.controller = controller;

		    this.SuspendLayout();
		    this.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Name = "PositionIntradayList";
			InitializeComponents();
			this.ResumeLayout( false );

            LoadPositions( controller.Position.AccountPositions );

            controller.Dispatcher.Rollover +=
                new EventHandler<DealingEventArgs>( FireRollover );
            controller.Dispatcher.PositionChanged +=
                new EventHandler<PositionEventArgs>( FirePositionChanged );
		}

		private void InitializeComponents()
        {
            //
            //  Tree List
            //

            data = ControlHelper.NewGlacialTreeList( "PositionIntradayListData" );
            data.ShowPlusMinus = true;

            //
            //  Tree List columns
            //

			GTLColumn accountId = new GTLColumn();
            accountId.CheckBoxes = false;
			accountId.ImageIndex = -1;
			accountId.Name = "accountId";
			accountId.Text = "Account ID";
			accountId.SortType = SortTypes.InsertionSort;
			accountId.Visible = false;

			GTLColumn investmentSystemId = new GTLColumn();
            investmentSystemId.CheckBoxes = false;
			investmentSystemId.ImageIndex = -1;
			investmentSystemId.Name = "investmentSystemId";
			investmentSystemId.Text = "Investment System ID";
			investmentSystemId.SortType = SortTypes.InsertionSort;
			investmentSystemId.Visible = false;

			GTLColumn instrumentId = new GTLColumn();
            instrumentId.CheckBoxes = false;
			instrumentId.ImageIndex = -1;
			instrumentId.Name = "instrumentId";
			instrumentId.Text = "Instrument ID";
			instrumentId.SortType = SortTypes.InsertionSort;
			instrumentId.Visible = false;

			GTLColumn accountName = new GTLColumn();
            accountName.CheckBoxes = false;
			accountName.ImageIndex = -1;
			accountName.Name = "accountName";
			accountName.Text = "Account Name --> Investment System";
			accountName.SortType = SortTypes.InsertionSort;
			accountName.Width = 240;

			GTLColumn instrumentName = new GTLColumn();
            instrumentName.CheckBoxes = false;
			instrumentName.ImageIndex = -1;
			instrumentName.Name = "instrumentName";
			instrumentName.Text = "Instrument Name";
			instrumentName.SortType = SortTypes.InsertionSort;
			instrumentName.Width = 220;

			GTLColumn totalPctPL = new GTLColumn();
            totalPctPL.CheckBoxes = false;
			totalPctPL.ImageIndex = -1;
			totalPctPL.Name = "totalPctPL";
			totalPctPL.Text = "Total Pct PL";
			totalPctPL.TextAlignment = ContentAlignment.MiddleRight;
			totalPctPL.SortType = SortTypes.InsertionSort;

			GTLColumn totalPL = new GTLColumn();
            totalPL.CheckBoxes = false;
			totalPL.ImageIndex = -1;
			totalPL.Name = "totalPL";
			totalPL.Text = "Total PL";
			totalPL.TextAlignment = ContentAlignment.MiddleRight;
			totalPL.SortType = SortTypes.InsertionSort;

			GTLColumn openPL = new GTLColumn();
            openPL.CheckBoxes = false;
			openPL.ImageIndex = -1;
			openPL.Name = "openPL";
			openPL.Text = "Open PL";
			openPL.TextAlignment = ContentAlignment.MiddleRight;
			openPL.SortType = SortTypes.InsertionSort;

			GTLColumn closedPL = new GTLColumn();
            closedPL.CheckBoxes = false;
			closedPL.ImageIndex = -1;
			closedPL.Name = "closedPL";
			closedPL.Text = "Closed PL";
			closedPL.TextAlignment = ContentAlignment.MiddleRight;
			closedPL.SortType = SortTypes.InsertionSort;

			GTLColumn longPos = new GTLColumn();
            longPos.CheckBoxes = false;
			longPos.ImageIndex = -1;
			longPos.Name = "longPos";
			longPos.Text = "Long Pos";
			longPos.TextAlignment = ContentAlignment.MiddleRight;
			longPos.SortType = SortTypes.InsertionSort;

			GTLColumn shortPos = new GTLColumn();
            shortPos.CheckBoxes = false;
			shortPos.ImageIndex = -1;
			shortPos.Name = "shortPos";
			shortPos.Text = "Short Pos";
			shortPos.TextAlignment = ContentAlignment.MiddleRight;
			shortPos.SortType = SortTypes.InsertionSort;

			GTLColumn closedPos = new GTLColumn();
            closedPos.CheckBoxes = false;
			closedPos.ImageIndex = -1;
			closedPos.Name = "closedPos";
			closedPos.Text = "Closed Pos";
			closedPos.TextAlignment = ContentAlignment.MiddleRight;
			closedPos.SortType = SortTypes.InsertionSort;

			GTLColumn avgOpenPrc = new GTLColumn();
            avgOpenPrc.CheckBoxes = false;
			avgOpenPrc.ImageIndex = -1;
			avgOpenPrc.Name = "avgOpenPrc";
			avgOpenPrc.Text = "Open Prc";
			avgOpenPrc.TextAlignment = ContentAlignment.MiddleRight;
			avgOpenPrc.SortType = SortTypes.InsertionSort;

			GTLColumn lastPrc = new GTLColumn();
            lastPrc.CheckBoxes = false;
			lastPrc.ImageIndex = -1;
			lastPrc.Name = "lastPrc";
			lastPrc.Text = "Last Prc";
			lastPrc.TextAlignment = ContentAlignment.MiddleRight;
			lastPrc.SortType = SortTypes.InsertionSort;

			data.Columns.AddRange(
			    new GTLColumn [] {
                    accountName,
                    accountId,
                    investmentSystemId,
                    instrumentId,
                    instrumentName,
                    totalPctPL,
                    totalPL,
                    openPL,
                    closedPL,
                    longPos,
                    shortPos,
                    closedPos,
                    avgOpenPrc,
                    lastPrc
			    } );

			this.Controls.AddRange( new Control [] { data } );
        }

        private void LoadPositions( ArrayList positions )
        {
            data.BeginUpdate();
            data.BeginNodeListChange();

            foreach( AccountPosition account_position in positions )
            {
                foreach( InvestmentSystemPosition investment_system_position in account_position.AccountPositions )
                {
                    GTLTreeNode positionNode = CreatePositionNode( investment_system_position );

                    if( null != positionNode )
                    {
                        data.Nodes.Add( positionNode );
                    }
                }
            }

            data.EndNodeListChange();
            data.EndUpdate();
        }

        private void FirePositionChanged( object o, PositionEventArgs e )
        {
            AccountPosition account_position = (AccountPosition)e.Data;

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
            //  Find this account + investment_system in our list?
            //

            foreach( InvestmentSystemPosition investment_system_position in account_position.AccountPositions )
            {
                if(! controller.Cache.HasInvestmentSystem( investment_system_position.InvestmentSystemId ) )
                {
                    log.Warn( "Unknown investment_system: " + investment_system_position.InvestmentSystemId );
                    continue;
                }

                bool account_found = false;
                double notional_pl, total_pl, open_pl, closed_pl;

                foreach( GTLTreeNode account_node in data.Nodes )
                {
                    notional_pl = total_pl = open_pl = closed_pl = 0.0;

                    if( account_node.SubItems[ "accountId" ].Text.Equals( investment_system_position.AccountId ) &&
                        account_node.SubItems[ "investmentSystemId" ].Text.Equals( investment_system_position.InvestmentSystemId ) )
                    {
                        account_found = true;

                        foreach( InstrumentPosition instrument_position in investment_system_position.InvestmentSystemPositions )
                        {
                            Instrument instrument =
                                controller.Cache.GetInstrument(
                                    instrument_position.InstrumentId );

                            if( null == instrument )
                            {
                                log.Warn( "Unknown instrument_id: " + instrument_position.InstrumentId );
                                continue;
                            }

                            //
                            //  Calculate the PL
                            //

                            instrument_position.ProfitLoss( account, instrument );
                            notional_pl += instrument_position.NotionalPL;
                            total_pl += instrument_position.TotalPL;
                            open_pl += instrument_position.OpenPL;
                            closed_pl += instrument_position.ClosedPL;

                            //
                            //  Try to find the instrument in the existing nodes
                            //

                            bool instrument_found = false;
                            foreach( GTLTreeNode instrument_node in account_node.Nodes )
                            {
                                if( instrument.Id.Equals( instrument_node.Tag ) )
                                {
                                    instrument_found = true;

                                    //
                                    //  Update the instrument row
                                    //

                                    ((PercentSubItem)instrument_node.SubItems[ "totalPctPL" ]).Update( instrument_position.NotionalPL );
                                    ((CurrencySubItem)instrument_node.SubItems[ "totalPL" ]).Update( instrument_position.TotalPL );
                                    ((CurrencySubItem)instrument_node.SubItems[ "openPL" ]).Update( instrument_position.OpenPL );
                                    ((CurrencySubItem)instrument_node.SubItems[ "closedPL" ]).Update( instrument_position.ClosedPL );
                                    instrument_node.SubItems[ "longPos" ].Text = instrument_position.LongPosition.ToString( double_fmt );
                                    instrument_node.SubItems[ "shortPos" ].Text = instrument_position.ShortPosition.ToString( double_fmt );
                                    instrument_node.SubItems[ "closedPos" ].Text = instrument_position.ClosedPosition.ToString( double_fmt );
                                    instrument_node.SubItems[ "avgOpenPrc" ].Text = instrument_position.OpenPrice.ToString( double_fmt );
                                    instrument_node.SubItems[ "lastPrc" ].Text = instrument[ DatabaseObject.LAST_PRC ];

                                    break;
                                }
                            }

                            if(! instrument_found )
                            {
                                //
                                //  Create the instrument node
                                //

                                GTLTreeNode instrument_node = new GTLTreeNode( );
                                instrument_node.SubItems.AddRange( new GTLSubItem [] {
                                    new GTLSubItem( ), //  account_name
                                    new GTLSubItem( account[ DatabaseObject.ACCOUNT_ID ] ), //  account_id
                                    new GTLSubItem( investment_system_position.InvestmentSystemId ), // investment_system_id
                                    new GTLSubItem( instrument[ DatabaseObject.INSTRUMENT_ID ] ), // instrument_id
                                    new GTLSubItem( instrument[ DatabaseObject.NAME ] ), //  instrument_name
                                    new PercentSubItem( instrument_position.NotionalPL, true ), //  pctPL
                                    new CurrencySubItem( instrument_position.TotalPL, true ), //  totalPL
                                    new CurrencySubItem( instrument_position.OpenPL, true ), //  openPL
                                    new CurrencySubItem( instrument_position.ClosedPL, true ), //  closedPL
                                    new GTLSubItem( instrument_position.LongPosition.ToString( double_fmt ) ), //  longPos
                                    new GTLSubItem( instrument_position.ShortPosition.ToString( double_fmt ) ), //  shortPos
                                    new GTLSubItem( instrument_position.ClosedPosition.ToString( double_fmt ) ), //  closedPos
                                    new GTLSubItem( instrument_position.OpenPrice.ToString( ) ), //  avgOpenPrc
                                    new GTLSubItem( instrument[ DatabaseObject.LAST_PRC ] ) //  lastPrc
                                } );

                                instrument_node.Tag = instrument.Id;
                                account_node.Nodes.Add( instrument_node );
                                account_node.SubItems[ 0 ].Span = 2;
                            }
                        }

                        //
                        //  Update the aggregated account_node PL
                        //

                        ((PercentSubItem)account_node.SubItems[ "totalPctPL" ]).Update( notional_pl );
                        ((CurrencySubItem)account_node.SubItems[ "totalPL" ]).Update( total_pl );
                        ((CurrencySubItem)account_node.SubItems[ "openPL" ]).Update( open_pl );
                        ((CurrencySubItem)account_node.SubItems[ "closedPL" ]).Update( closed_pl );

                        break;
                    }
                }

                if(! account_found )
                {
                    //
                    //  We couldn't find this account + investment_system, so
                    //  create a new one.
                    //

                    data.BeginUpdate();
                    data.BeginNodeListChange();

                    GTLTreeNode positionNode = CreatePositionNode( investment_system_position );

                    if( null != positionNode )
                    {
                        data.Nodes.Add( positionNode );
                    }

                    data.EndNodeListChange();
                    data.EndUpdate();
                }
            }
        }

        private GTLTreeNode CreatePositionNode( InvestmentSystemPosition position )
        {
            //
            //  Get the account on the position
            //

            Account account = controller.Cache.GetAccount( position.AccountId );

            if( null == account )
            {
                log.Warn( "Unknown account: " + position.AccountId );
                return null;
            }

            if(! controller.Cache.HasInvestmentSystem( position.InvestmentSystemId ) )
            {
                log.Warn( "Unknown investment_system: " + position.InvestmentSystemId );
                return null;
            }

            GTLTreeNode account_node = new GTLTreeNode( " " +
                account[ DatabaseObject.NAME ] + " --> " +
                position.InvestmentSystemId );

            double notional_pl, total_pl, open_pl, closed_pl;
            notional_pl = total_pl = open_pl = closed_pl = 0.0;

            foreach( InstrumentPosition instrument_position in position.InvestmentSystemPositions )
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

                //
                //  Create the instrument node
                //

                GTLTreeNode instrument_node = new GTLTreeNode( );
                instrument_node.SubItems.AddRange( new GTLSubItem [] {
                    new GTLSubItem( ), //  account_name
                    new GTLSubItem( account[ DatabaseObject.ACCOUNT_ID ] ), //  account_id
                    new GTLSubItem( position.InvestmentSystemId ), // investment_system_id
                    new GTLSubItem( instrument[ DatabaseObject.INSTRUMENT_ID ] ), // instrument_id
                    new GTLSubItem( instrument[ DatabaseObject.NAME ] ), //  instrument_name
                    new PercentSubItem( instrument_position.NotionalPL, true ), //  pctPL
                    new CurrencySubItem( instrument_position.TotalPL, true ), //  totalPL
                    new CurrencySubItem( instrument_position.OpenPL, true ), //  openPL
                    new CurrencySubItem( instrument_position.ClosedPL, true ), //  closedPL
                    new GTLSubItem( instrument_position.LongPosition.ToString( double_fmt ) ), //  longPos
                    new GTLSubItem( instrument_position.ShortPosition.ToString( double_fmt ) ), //  shortPos
                    new GTLSubItem( instrument_position.ClosedPosition.ToString( double_fmt ) ), //  closedPos
                    new GTLSubItem( instrument_position.OpenPrice.ToString( ) ), //  avgOpenPrc
                    new GTLSubItem( instrument[ DatabaseObject.LAST_PRC ] ) //  lastPrc
                } );

                instrument_node.Tag = instrument.Id;
                account_node.Nodes.Add( instrument_node );
                account_node.SubItems[ 0 ].Span = 2;

                notional_pl += instrument_position.NotionalPL;
                total_pl += instrument_position.TotalPL;
                open_pl += instrument_position.OpenPL;
                closed_pl += instrument_position.ClosedPL;
            }

            account_node.SubItems.AddRange( new GTLSubItem [] {
                new GTLSubItem( account[ DatabaseObject.ACCOUNT_ID ] ), // account_id
                new GTLSubItem( position.InvestmentSystemId ), // investment_system_id
                new GTLSubItem(  ), //  instrument_id
                new GTLSubItem(  ), //  instrument_name
                new PercentSubItem( notional_pl, true ), //  totalPctPL
                new CurrencySubItem( total_pl, true ), //  totalPL
                new CurrencySubItem( open_pl, true ), //  openPL
                new CurrencySubItem( closed_pl, true ), //  closedPL
                new GTLSubItem(  ), //  longPos
                new GTLSubItem(  ), //  shortPos
                new GTLSubItem(  ), //  closedPos
                new GTLSubItem(  ), //  avgOpenPrc
                new GTLSubItem(  ) //  lastPrc
            } );

            account_node.Tag = account[ DatabaseObject.ACCOUNT_ID ];
            return account_node;
        }
    }
}
               