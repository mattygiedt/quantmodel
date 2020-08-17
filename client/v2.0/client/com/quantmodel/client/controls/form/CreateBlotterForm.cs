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

using quantmodel;
using com.quantmodel.common.data;
using com.quantmodel.common.events;
using com.quantmodel.common.controls;

using com.quantmodel.common.network.message;

namespace com.quantmodel.client.controls.form
{
	public class CreateBlotterForm : Form
	{
	    private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( CreateBlotterForm ) );

	    private Panel buttonPanel = null;
	    private Panel headerPanel = null;
	    private Panel mainPanel = null;
        
        private DateTimePicker dateTimePicker = null;

	    private readonly ClientController controller;
        private readonly Button execButton = new Button();
        
    	public CreateBlotterForm( ClientController controller ) : base()
		{
            this.controller = controller;
            this.Font = new System.Drawing.Font( "Tahoma", 9.25F );

		    InitializeComponents();
		    this.Visible = false;

		    this.VisibleChanged += new EventHandler( FireVisibleChanged );
		    this.FormClosing += new FormClosingEventHandler( FireFormClosing );
        }

        private void InitializeComponents()
    	{
    	    Label headerLabel = new Label();
			headerLabel.Location = new System.Drawing.Point( 10, 10 );
			headerLabel.Name = "headerLabel";
			headerLabel.Size = new System.Drawing.Size( 360, 20 );
			headerLabel.TabIndex = 3;
			headerLabel.Text = "Create Blotter:";
			headerLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			headerLabel.Font = new System.Drawing.Font(
			    "Tahoma", 10.25F, System.Drawing.FontStyle.Bold,
			        System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
            
            Label dateLabel = new Label();
            dateLabel.AutoSize = true;
            dateLabel.Location = new System.Drawing.Point( 50, 16 );
            dateLabel.Name = "dateLabel";
            dateLabel.Text = "Trade Date:";
            dateLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            
            dateTimePicker = new DateTimePicker();
            dateTimePicker.Size = new System.Drawing.Size( 260, 20 );
            dateTimePicker.Value = DateTime.Today;
            dateTimePicker.ShowUpDown = false;
            dateTimePicker.ShowCheckBox = false;
            dateTimePicker.MaxDate = DateTime.Today;
            //dateTimePicker.Format = DateTimePickerFormat.Short;            
            dateTimePicker.Location = new System.Drawing.Point( 50, 40 );
            
            execButton.Size = new System.Drawing.Size( 84, 26 );
            execButton.Text = "Execute";
            execButton.TabIndex = 1;
            execButton.TabStop = true;
            execButton.Location = new System.Drawing.Point( 70, 10 );
            execButton.Click += new EventHandler( FireExecuteButton );
            execButton.Anchor = AnchorStyles.Right;
            execButton.Enabled = false;

            Button cancel = new Button();
            cancel.Text = "Cancel";
            cancel.Size = new System.Drawing.Size( 84, 26 );
            cancel.Location = new System.Drawing.Point( 160, 10 );
            cancel.Click += new EventHandler( FireCancelButton );
            cancel.Anchor = AnchorStyles.Right;

			//
			//  Form panels
			//

            mainPanel = new Panel();
	        mainPanel.BackColor = System.Drawing.SystemColors.Window;
			mainPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			mainPanel.Name = "mainPanel";

			mainPanel.Controls.AddRange( new Control [] { dateLabel, dateTimePicker } );

			headerPanel = new Panel();
	        headerPanel.BackColor = System.Drawing.SystemColors.Window;
			headerPanel.Dock = System.Windows.Forms.DockStyle.Top;
			headerPanel.Name = "headerPanel";
			headerPanel.Size = new System.Drawing.Size( 260, 30 );
			headerPanel.TabIndex = 8;
			headerPanel.Controls.AddRange( new Control[] { headerLabel } );

			buttonPanel = new Panel();
	        buttonPanel.BackColor = System.Drawing.SystemColors.Control;
			buttonPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
			buttonPanel.Name = "buttonPanel";
			buttonPanel.Size = new System.Drawing.Size( 260, 50 );
			buttonPanel.Controls.AddRange( new Control[] { execButton, cancel } );

            this.Controls.AddRange( new Control [] {
                mainPanel,
                headerPanel,
                buttonPanel
            } );
	    }

	    private void FireVisibleChanged( object o, EventArgs args )
        {
            if( this.Visible )
            {
                execButton.Enabled = true;
                execButton.Select();
                
                //
                //  Load the active accounts
                //

                //foreach( Account account in controller.Cache.Accounts )
                //{
                //    if( "1".Equals( account[ DatabaseObject.IS_ACTIVE ] ) )
                //    {
                //        
    		    //    }
    		    //}
            }
            else
            {
                execButton.Enabled = false;
            }
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
            
        }

        private void FireExecuteButton( object sender, EventArgs args )
        {
            ArrayList rows = new ArrayList();
            DateTime EPOCH = new DateTime( 1970, 1, 1, 0, 0, 0, DateTimeKind.Utc );
            
            rows.Add( "Record Type,Investment,Portfolio,Strategy,Client Executing Broker,Prime Broker A/c Number,Transaction Type,Client Transaction ID,Client Asset Type,Trade Date,Contractual Settlement Date,Trade Execution Date,Broker Clearing Date,Price,Settlement Currency,Quantity,Commission Amount,Trader,Manager" );
            
            ClientAdapterResponse response = controller.SendRpc(
                new CreateTradeBlotter( dateTimePicker.Value ) );            
        
            foreach( DatabaseEngineResponse.Types.ResultSet rs in response.DatabaseResponse.ResultSetList )
            {
                foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row row in rs.RowList )
                {
                    if( controller.Cache.HasInstrument( row.ColumnList[ 1 ].Value ) == false )
                    {
                        MessageBox.Show( "Unknown instrument_id: " + row.ColumnList[ 1 ].Value );
                        return;                        
                    }
                    
                    Instrument instrument = controller.Cache.GetInstrument( row.ColumnList[ 1 ].Value );
                    TimeSpan ts = TimeSpan.FromSeconds( Convert.ToDouble( row.ColumnList[ 6 ].Value ) );
                    DateTime executionDate = EPOCH + ts;
                    
                    rows.Add( "N," + instrument[ DatabaseObject.NAME ].Substring( 10 ) + 
                              " Index,GCCG5,CCK-FT-S1,DORM,2000CAS2," + 
                              com.quantmodel.common.data.DatabaseObject.SideCode( row.ColumnList[ 3 ].Value ) +
                              ",CCKFT-" + row.ColumnList[ 0 ].Value +
                              ",FUT," + dateTimePicker.Value.ToString( "yyyyMMdd" ) + 
                              "," + dateTimePicker.Value.ToString( "yyyyMMdd" ) +
                              "," + executionDate.ToString( "yyyy-MM-ddTHH:mm:ss" ) +
                              "," + dateTimePicker.Value.ToString( "yyyyMMdd" ) +
                              "," + row.ColumnList[ 4 ].Value +
                              ",USD," + row.ColumnList[ 5 ].Value +
                              ",1,Mark,CCK" );
                }
            }
            
            if( rows.Count == 1 )
            {
                MessageBox.Show( "No records found for trade date: " + dateTimePicker.Value.ToShortDateString() );
            }
            else
            {
                Stream myStream;
                SaveFileDialog safeFileDialog = new SaveFileDialog();
                
                safeFileDialog.Filter = "csv files (*.csv)|*.csv|All files (*.*)|*.*";
                safeFileDialog.FilterIndex = 2 ;
                safeFileDialog.RestoreDirectory = true;
                
                if( safeFileDialog.ShowDialog() == DialogResult.OK )
                {
                    if( ( myStream = safeFileDialog.OpenFile() ) != null )
                    {
                        StreamWriter writer = new StreamWriter( myStream );
                        
                        foreach( string row in rows )
                        {
                            writer.Write( row + "\n" );
                        }
                        
                        writer.Close();
                    }
                    
                    this.Hide();
                    MessageBox.Show( "Blotter created for trade date: " + dateTimePicker.Value.ToShortDateString() );
                }
            }
        }
	}
}
