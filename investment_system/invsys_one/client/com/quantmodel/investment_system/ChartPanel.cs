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
using System.Globalization;
using System.ComponentModel;

using System.Windows.Forms.DataVisualization.Charting;

using TD.Eyefinder;
using GlacialComponents.Controls.GlacialTreeList;
using GlacialComponents.Controls.GTLCommon;

using quantmodel;
using investment_system.invsys_one;

namespace com.quantmodel.investment_system
{
    public sealed class ChartPanel : UserControl
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( ChartPanel ) );

        private DateTime currXAxis;
        private Trend currTrend;

        private readonly CultureInfo provider = CultureInfo.InvariantCulture;
        private readonly int UTC_OFFSET = DateTime.UtcNow.Hour - DateTime.Now.Hour;
        private readonly DateTime EPOCH = new DateTime( 1970, 1, 1, 0, 0, 0, DateTimeKind.Utc );

        private ArrayList orderExecutionList = new ArrayList();
        private Series series1, series2, series3, series4, series5, series6,
                       series7, series8, series9, series10;
        private StripLine bidAskStripLine;
        private ChartArea chartArea1, chartArea2;
        private Chart chart1;


        private double aggregate_qty = 0.0D;
        private double aggregate_val = 0.0D;
        private bool deleteLastTimebar = false;

        public ChartPanel( ) : base()
		{
		    this.SuspendLayout();
		    this.AutoScroll = true;
		    this.AutoSize = true;
		    this.Size = new System.Drawing.Size( 550, 550 );
		    this.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Name = "ChartPanel";
			this.Tag = "ChartPanel";
			this.BackColor = System.Drawing.SystemColors.Window;
			InitializeComponents();
			this.ResumeLayout( false );
		}

		private void InitializeComponents()
		{
		    series1 = new Series();
            series1.BorderColor = System.Drawing.Color.FromArgb(((System.Byte)(180)), ((System.Byte)(26)), ((System.Byte)(59)), ((System.Byte)(105)));
            series1.ChartArea = "ChartArea";
            series1.Name = "Timebar";
            series1.ChartType = SeriesChartType.Candlestick;
            series1.YValuesPerPoint = 4;
            series1.IsVisibleInLegend = false;
            series1.IsXValueIndexed = false;
            series1[ "ShowOpenClose" ] = "Both";
            series1[ "OpenCloseStyle" ] = "Line";
            series1[ "PriceUpColor" ] = "#66FF33";
            series1[ "PriceDownColor" ] = "#FF0000";
            series1[ "PointWidth" ] = "0.8";
            series1[ "MaxPixelPointWidth" ] = "12";
            series1.XValueType = ChartValueType.Time;
            series1.YValueType = ChartValueType.Double;

            series2 = new Series();
            series2.BorderColor = System.Drawing.Color.FromArgb(((System.Byte)(180)), ((System.Byte)(26)), ((System.Byte)(59)), ((System.Byte)(105)));
			series2.ChartArea = "ChartArea";
			series2.ChartType = SeriesChartType.Spline;
			series2[ "LineTension" ] = "0.4";
			//series2.Color = System.Drawing.Color.FromArgb(((System.Byte)(51)), ((System.Byte)(102)), ((System.Byte)(255)));
			series2.Color = System.Drawing.Color.DarkGray;
			series2.ShadowColor = System.Drawing.Color.Black;
			series2.Name = "TREND";
			series2.IsVisibleInLegend = false;
			series2.IsXValueIndexed = false;
			series2.XValueType = ChartValueType.Time;
			series2.YValueType = ChartValueType.Auto;

			series3 = new Series();
            series3.BorderColor = System.Drawing.Color.FromArgb(((System.Byte)(180)), ((System.Byte)(26)), ((System.Byte)(59)), ((System.Byte)(105)));
			series3.ChartArea = "VolumeArea";
			series3.ChartType = SeriesChartType.Column;
			series3.Color = System.Drawing.Color.FromArgb(((System.Byte)(102)), ((System.Byte)(255)), ((System.Byte)(51))); //, ((System.Byte)(105)));
			series3.ShadowColor = System.Drawing.Color.Transparent;
			series3.Name = "UP_VOLUME";
			series3.IsVisibleInLegend = false;
			series3.IsXValueIndexed = false;
			series3[ "MaxPixelPointWidth" ] = "20";
			series3.XValueType = ChartValueType.Time;
			series3.YValueType = ChartValueType.Double;

			series4 = new Series();
            series4.BorderColor = System.Drawing.Color.FromArgb(((System.Byte)(180)), ((System.Byte)(26)), ((System.Byte)(59)), ((System.Byte)(105)));
			series4.ChartArea = "VolumeArea";
			series4.ChartType = SeriesChartType.Column;
			series4.Color = System.Drawing.Color.FromArgb(((System.Byte)(255)), ((System.Byte)(0)), ((System.Byte)(0)));
			series4.ShadowColor = System.Drawing.Color.Transparent;
			series4.Name = "DOWN_VOLUME";
			series4.IsVisibleInLegend = false;
			series4.IsXValueIndexed = false;
			series4[ "MaxPixelPointWidth" ] = "20";
			series4.XValueType = ChartValueType.Time;
			series4.YValueType = ChartValueType.Double;

			series5 = new Series();
            //series5.BorderColor = System.Drawing.Color.Transparent;
			series5.ChartArea = "VolumeArea";
			series5.ChartType = SeriesChartType.Column;
			series5.Color = System.Drawing.Color.FromArgb(((System.Byte)(51)), ((System.Byte)(102)), ((System.Byte)(255)));
			series5.ShadowColor = System.Drawing.Color.Transparent;
			series5.Name = "PLACEHOLDER_VOLUME";
			series5.IsVisibleInLegend = false;
			series5.IsXValueIndexed = false;
			series5[ "MaxPixelPointWidth" ] = "20";
			series5.XValueType = ChartValueType.Time;
			series5.YValueType = ChartValueType.Double;

			series6 = new Series();
            series6.ChartArea = "ChartArea";
			series6.ChartType = SeriesChartType.Point;
			series6.Name = "UP_PIVOT";
			series6.IsVisibleInLegend = false;
			series6.IsXValueIndexed = false;
			series6.XValueType = ChartValueType.Time;
			series6.YValueType = ChartValueType.Double;
			series6.MarkerColor = System.Drawing.ColorTranslator.FromHtml( "#66FF33" );
			series6.MarkerSize = 10;
			series6.MarkerStyle = MarkerStyle.Circle;
			series6.MarkerBorderColor = System.Drawing.Color.FromArgb(((System.Byte)(180)), ((System.Byte)(26)), ((System.Byte)(59)), ((System.Byte)(105)));
			series6.MarkerBorderWidth = 4;

			series7 = new Series();
            series7.ChartArea = "ChartArea";
			series7.ChartType = SeriesChartType.Point;
			series7.Name = "DOWN_PIVOT";
			series7.IsVisibleInLegend = false;
			series7.IsXValueIndexed = false;
			series7.XValueType = ChartValueType.Time;
			series7.YValueType = ChartValueType.Double;
			series7.MarkerColor = System.Drawing.ColorTranslator.FromHtml( "#FF0000" );
			series7.MarkerSize = 10;
			series7.MarkerStyle = MarkerStyle.Circle;
			series7.MarkerBorderColor = System.Drawing.Color.FromArgb(((System.Byte)(180)), ((System.Byte)(26)), ((System.Byte)(59)), ((System.Byte)(105)));
			series7.MarkerBorderWidth = 4;

			series8 = new Series();
            series8.ChartArea = "ChartArea";
			series8.ChartType = SeriesChartType.Point;
			series8.Name = "BUY_EXECUTION";
			series8.IsVisibleInLegend = false;
			series8.IsXValueIndexed = false;
			series8.XValueType = ChartValueType.Time;
			series8.YValueType = ChartValueType.Double;
			series8.MarkerColor = System.Drawing.ColorTranslator.FromHtml( "#66FF33" );
			series8.MarkerSize = 10;
			series8.MarkerStyle = MarkerStyle.Square;
			series8.MarkerBorderColor = System.Drawing.Color.FromArgb(((System.Byte)(180)), ((System.Byte)(26)), ((System.Byte)(59)), ((System.Byte)(105)));
			series8.MarkerBorderWidth = 4;

			series9 = new Series();
            series9.ChartArea = "ChartArea";
			series9.ChartType = SeriesChartType.Point;
			series9.Name = "SELL_EXECUTION";
			series9.IsVisibleInLegend = false;
			series9.IsXValueIndexed = false;
			series9.XValueType = ChartValueType.Time;
			series9.YValueType = ChartValueType.Double;
			series9.MarkerColor = System.Drawing.ColorTranslator.FromHtml( "#FF0000" );
			series9.MarkerSize = 10;
			series9.MarkerStyle = MarkerStyle.Square;
			series9.MarkerBorderColor = System.Drawing.Color.FromArgb(((System.Byte)(180)), ((System.Byte)(26)), ((System.Byte)(59)), ((System.Byte)(105)));
			series9.MarkerBorderWidth = 4;

			series10 = new Series();
            series10.BorderColor = System.Drawing.Color.FromArgb(((System.Byte)(180)), ((System.Byte)(26)), ((System.Byte)(59)), ((System.Byte)(105)));
			series10.ChartArea = "ChartArea";
			series10.ChartType = SeriesChartType.Spline;
			series10[ "LineTension" ] = "0.2";
			series10.Color = System.Drawing.Color.FromArgb(((System.Byte)(51)), ((System.Byte)(102)), ((System.Byte)(255)));
			series10.ShadowColor = System.Drawing.Color.Black;
			series10.Name = "VWAP";
			series10.IsVisibleInLegend = false;
			series10.IsXValueIndexed = false;
			series10.XValueType = ChartValueType.Time;
			series10.YValueType = ChartValueType.Double;

            bidAskStripLine = new StripLine();
            bidAskStripLine.Interval = 0;
            bidAskStripLine.BackColor = System.Drawing.Color.FromArgb(((System.Byte)(125)), ((System.Byte)(125)), ((System.Byte)(125)), ((System.Byte)(125)));

			chartArea1 = new ChartArea();
            chartArea1.BackColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(165)), ((System.Byte)(191)), ((System.Byte)(228)));
			chartArea1.BackSecondaryColor = System.Drawing.Color.White;
			chartArea1.BackGradientStyle = System.Windows.Forms.DataVisualization.Charting.GradientStyle.TopBottom;
			chartArea1.BorderColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)));
			chartArea1.BorderDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;
			chartArea1.Name = "ChartArea";
			chartArea1.ShadowColor = System.Drawing.Color.Transparent;
			chartArea1.CursorX.IsUserEnabled = false;

            chartArea1.AxisX.Enabled = AxisEnabled.True;
            //chartArea1.AxisX.IntervalAutoMode = IntervalAutoMode.VariableCount;
            chartArea1.AxisX.LabelStyle.IntervalType = DateTimeIntervalType.Minutes;
			chartArea1.AxisX.MajorGrid.IntervalType = DateTimeIntervalType.Minutes;
			chartArea1.AxisX.MajorTickMark.IntervalType = DateTimeIntervalType.Minutes;
			chartArea1.AxisX.Interval = 15;
			//chartArea1.AxisX.LabelStyle.Enabled = false;
            chartArea1.AxisX.IsInterlaced = true;
            chartArea1.AxisX.IsLabelAutoFit = true;
            chartArea1.AxisX.LabelStyle.Font = new System.Drawing.Font("Trebuchet MS", 8.25F, System.Drawing.FontStyle.Bold);
            chartArea1.AxisX.LineColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)));
			chartArea1.AxisX.MajorGrid.LineColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)));

            chartArea1.AxisY.Enabled = AxisEnabled.True;
            chartArea1.AxisY.IntervalAutoMode = IntervalAutoMode.VariableCount;
    	    chartArea1.AxisY.IsStartedFromZero = false;
            chartArea1.AxisY.IsLabelAutoFit = false;
			chartArea1.AxisY.LabelStyle.Font = new System.Drawing.Font("Trebuchet MS", 8.25F, System.Drawing.FontStyle.Bold);
			chartArea1.AxisY.LineColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)));
			chartArea1.AxisY.MajorGrid.LineColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)));
			chartArea1.AxisY.StripLines.Add( bidAskStripLine );

            chartArea1.AxisY2.Enabled = AxisEnabled.True;
            chartArea1.AxisY2.IntervalAutoMode = IntervalAutoMode.VariableCount;
    	    chartArea1.AxisY2.IsStartedFromZero = false;
            chartArea1.AxisY2.IsLabelAutoFit = false;
			chartArea1.AxisY2.LabelStyle.Font = new System.Drawing.Font("Trebuchet MS", 8.25F, System.Drawing.FontStyle.Bold);
			chartArea1.AxisY2.LineColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)));
			chartArea1.AxisY2.MajorGrid.LineColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)));

            chartArea2 = new ChartArea();
            chartArea2.AlignWithChartArea = "ChartArea";
            chartArea2.BackColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(165)), ((System.Byte)(191)), ((System.Byte)(228)));
			chartArea2.BackSecondaryColor = System.Drawing.Color.White;
			chartArea2.BackGradientStyle = System.Windows.Forms.DataVisualization.Charting.GradientStyle.TopBottom;
			chartArea2.BorderColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)));
			chartArea2.BorderDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;
			chartArea2.Name = "VolumeArea";
			chartArea2.ShadowColor = System.Drawing.Color.Transparent;

            chartArea2.AxisX.Enabled = AxisEnabled.True;
            //chartArea2.AxisX.IntervalAutoMode = IntervalAutoMode.VariableCount;
			chartArea2.AxisX.LabelStyle.IntervalType = DateTimeIntervalType.Minutes;
			chartArea2.AxisX.MajorGrid.IntervalType = DateTimeIntervalType.Minutes;
			chartArea2.AxisX.MajorTickMark.IntervalType = DateTimeIntervalType.Minutes;
			chartArea2.AxisX.Interval = 15;
			chartArea2.AxisX.IsLabelAutoFit = true;
            chartArea2.AxisX.IsInterlaced = true;
            chartArea2.AxisX.LabelStyle.Font = new System.Drawing.Font("Trebuchet MS", 8.25F, System.Drawing.FontStyle.Bold);
            chartArea2.AxisX.LineColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)));
			chartArea2.AxisX.MajorGrid.LineColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)));

			chartArea2.AxisY.IsStartedFromZero = true;
            chartArea2.AxisY.IsLabelAutoFit = false;
            chartArea2.AxisY.IntervalAutoMode = IntervalAutoMode.VariableCount;
			chartArea2.AxisY.LabelStyle.Font = new System.Drawing.Font("Trebuchet MS", 8.25F, System.Drawing.FontStyle.Bold);
			chartArea2.AxisY.LineColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)));
			chartArea2.AxisY.MajorGrid.LineColor = System.Drawing.Color.FromArgb(((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)), ((System.Byte)(64)));

			chartArea1.Position.X = 0F;
            chartArea1.Position.Y = 0F;
            chartArea1.Position.Width = 100F;
            chartArea1.Position.Height = 80F;

            chartArea2.Position.X = 0F;
            chartArea2.Position.Y = 82F;
            chartArea2.Position.Width = 100F;
            chartArea2.Position.Height = 18F;

            chart1 = new Chart();
            chart1.Palette = ChartColorPalette.BrightPastel;
            chart1.AntiAliasing = AntiAliasingStyles.All;
            chart1.BackColor = System.Drawing.Color.White;
            chart1.Name = "chart1";

            chart1.Series.Add( series1 );
            chart1.Series.Add( series2 );
            chart1.Series.Add( series3 );
            chart1.Series.Add( series4 );
            chart1.Series.Add( series5 );
            chart1.Series.Add( series6 );
            chart1.Series.Add( series7 );
            chart1.Series.Add( series8 );
            chart1.Series.Add( series9 );
            chart1.Series.Add( series10 );

            chart1.ChartAreas.Add( chartArea1 );
            chart1.ChartAreas.Add( chartArea2 );

            chart1.Dock = DockStyle.Fill;
            chart1.TabIndex = 0;

            this.Controls.Add( chart1 );
		}

	    public void AddTrendData( Trend trend )
		{
            DateTime x_axis = EPOCH.AddSeconds( trend.Timestamp ).ToLocalTime();

            if( trend.Type == Trend.Types.TrendType.UP_TREND )
            {
                series2.Points.AddXY( x_axis, trend.High );

                if( series7.Points.Count == 0 ||
                    currTrend.Type == Trend.Types.TrendType.DOWN_TREND )
                {
                    series7.Points.AddXY( x_axis, trend.High );
                }
                else if( series7.Points[ series7.Points.Count - 1 ].YValues[ 0 ] <= trend.High )
                {
                    series7.Points.RemoveAt( series7.Points.Count - 1 );
                    series7.Points.AddXY( x_axis, trend.High );
                }
                else
                {
                    //  NOP -- previous up trend is higher than this value
                }
            }
            else
            {
                series2.Points.AddXY( x_axis, trend.Low );

                if( series6.Points.Count == 0 ||
                    currTrend.Type == Trend.Types.TrendType.UP_TREND )
                {
                    series6.Points.AddXY( x_axis, trend.Low );
                }
                else if( series6.Points[ series6.Points.Count - 1 ].YValues[ 0 ] >= trend.Low )
                {
                    series6.Points.RemoveAt( series6.Points.Count - 1 );
                    series6.Points.AddXY( x_axis, trend.Low );
                }
                else
                {
                    //  NOP -- previous down trend is lower than this value
                }
            }

            currTrend = trend;
		}

		private void AddVolumeData( Timebar timebar )
        {
            DateTime x_axis = EPOCH.AddSeconds( timebar.Timestamp ).ToLocalTime();

            if( timebar.Open <= timebar.Close )
            {
                series3.Points.AddXY( x_axis, ( timebar.BuyQty + timebar.SellQty + timebar.UnknQty ) );
            }
            else
            {
                series4.Points.AddXY( x_axis, ( timebar.BuyQty + timebar.SellQty + timebar.UnknQty ) );
            }

            //
            //  Add VWAP data
            //

            aggregate_qty += ( timebar.BuyQty + timebar.SellQty + timebar.UnknQty );
            aggregate_val += ( timebar.BuyVal + timebar.SellVal + timebar.UnknVal );
            series10.Points.AddXY( x_axis, ( aggregate_val / aggregate_qty ) );

            double totalVolume = 0;
            ArrayList volumeList = new ArrayList();
            foreach( DataPoint point in series3.Points )
            {
                totalVolume += point.YValues[ 0 ];
                volumeList.Add( point.YValues[ 0 ] );

                if( point.ToolTip == null || point.ToolTip == "" )
                {
                    point.ToolTip = "BUY: " + timebar.BuyQty + "\nSELL: " + timebar.SellQty + "\nTOTAL: " + point.YValues[ 0 ];
                }
            }

            foreach( DataPoint point in series4.Points )
            {
                totalVolume += point.YValues[ 0 ];
                volumeList.Add( point.YValues[ 0 ] );

                if( point.ToolTip == null || point.ToolTip == "" )
                {
                    point.ToolTip = "BUY: " + timebar.BuyQty + "\nSELL: " + timebar.SellQty + "\nTOTAL: " + point.YValues[ 0 ];
                }
            }

/*
            //
            //  Calculate the 70% median
            //

            volumeList.Sort( );
            double median = (double)volumeList[ (int)( (volumeList.Count-1) * .7) ];
*/
        }

        private void AddPlaceholderVolumeData( Timebar timebar )
        {
            DateTime x_axis = EPOCH.AddSeconds( timebar.Timestamp ).ToLocalTime();
            series5.Points.AddXY( x_axis, ( timebar.BuyQty + timebar.SellQty + timebar.UnknQty ) );
        }

        private void RemovePlaceholderVolumeData( )
        {
            series5.Points.Clear();
        }

		private void AddTimebarData( Timebar timebar )
		{
		    DateTime x_axis = EPOCH.AddSeconds( timebar.Timestamp ).ToLocalTime();
            series1.Points.AddXY( x_axis, new object[] {
                timebar.High,
                timebar.Low,
                timebar.Open,
                timebar.Close } );

            foreach( DataPoint point in series1.Points )
            {
                if( point.ToolTip == null || point.ToolTip == "" )
                {
                    point.ToolTip = "OPEN: " + point.YValues[ 2 ] +
                                    "\nHIGH: " + point.YValues[ 0 ] +
                                    "\nLOW: " + point.YValues[ 1 ] +
                                    "\nCLOSE: " + point.YValues[ 3 ];
                }
            }

            currXAxis = x_axis;
        }

        public bool AddOrderExecutionData( OrderExecution execution, bool insert )
        {
            DateTime x_axis = EPOCH.AddSeconds( execution.Timestamp ).ToLocalTime();

            if( currXAxis == null )
            {
                if( insert == true )
                {
                    orderExecutionList.Add( execution );
                }
            }
            else if( currXAxis.Ticks < x_axis.Ticks )
            {
                if( insert == true )
                {
                    orderExecutionList.Add( execution );
                }
            }
            else
            {
                if( execution.Type == OrderExecution.Types.ExecutionType.BUY )
                {
                    series8.Points.AddXY( x_axis, execution.Price );
                }
                else
                {
                    series9.Points.AddXY( x_axis, execution.Price );
                }

                return true;
            }

            return false;
        }

        public void UpdateTimebarData( Timebar timebar, bool finalTimebar )
		{
		    DateTime x_axis = EPOCH.AddSeconds( timebar.Timestamp ).ToLocalTime();

		    if( deleteLastTimebar == true )
		    {
		        series1.Points.RemoveAt( series1.Points.Count - 1 );
		        deleteLastTimebar = false;
		    }

            //
            //  Determine if there are any executions to add to the timebar chart that
            //  were created prior to the current timebar.
            //

            if( orderExecutionList.Count > 0 )
            {
                //
                //  First, create copy of execution list
                //

                ArrayList skip_list = new ArrayList( orderExecutionList );

                foreach( OrderExecution orderExecution in skip_list )
                {
                    if( AddOrderExecutionData( orderExecution, false ) )
                    {
                        //
                        //  We were able to insert the execution. Remove it from
                        //  the order execution list.
                        //

                        orderExecutionList.Remove( orderExecution );
                    }
                }

                skip_list.Clear();
            }

		    if( finalTimebar == false && series1.Points.Count > 0 )
		    {
		        if( x_axis == currXAxis )
		        {
		            DataPoint candlestick = series1.Points[ series1.Points.Count - 1 ];
		            DataPoint volume = series5.Points[ 0 ];

		            candlestick.YValues[ 0 ] = timebar.High;
    		        candlestick.YValues[ 1 ] = timebar.Low;
    		        candlestick.YValues[ 2 ] = timebar.Open;
    		        candlestick.YValues[ 3 ] = timebar.Close;

    		        candlestick.ToolTip = "OPEN: " + candlestick.YValues[ 2 ] +
                                          "\nHIGH: " + candlestick.YValues[ 0 ] +
                                          "\nLOW: " + candlestick.YValues[ 1 ] +
                                          "\nCLOSE: " + candlestick.YValues[ 3 ];

    		        if( chartArea1.AxisY.Maximum != Double.NaN &&
                        chartArea1.AxisY.Maximum <= timebar.High )
                    {
                        //log.Warn( " MAX: " + chartArea1.AxisY.Maximum + " <= " + timebar.High );

                        deleteLastTimebar = true;
                        series1.Points.AddXY( EPOCH.AddSeconds( timebar.Timestamp ).ToLocalTime(),
                            new object[] {
                                timebar.High,
                                timebar.Low,
                                timebar.Open,
                                timebar.Close } );
                    }

                    if( chartArea1.AxisY.Minimum != Double.NaN &&
                        chartArea1.AxisY.Minimum >= timebar.Low )
                    {
                        //log.Info( " MIN: " + chartArea1.AxisY.Minimum + " >= " + timebar.Low );

                        deleteLastTimebar = true;
                        series1.Points.AddXY( EPOCH.AddSeconds( timebar.Timestamp ).ToLocalTime(),
                            new object[] {
                                timebar.High,
                                timebar.Low,
                                timebar.Open,
                                timebar.Close } );
                    }

    		        volume.YValues[ 0 ] = ( timebar.BuyQty + timebar.SellQty + timebar.UnknQty );

    		        if( chartArea2.AxisY.Maximum != Double.NaN &&
                        chartArea2.AxisY.Maximum <= volume.YValues[ 0 ] )
                    {
                        //log.Warn( " VOL: " + chartArea2.AxisY.Maximum + " <= " + volume.YValues[ 0 ] );
                        RemovePlaceholderVolumeData( );
                        AddPlaceholderVolumeData( timebar );
                    }
		        }
		        else if( timebar.Open > 0 )
		        {
		            AddTimebarData( timebar );
		            AddPlaceholderVolumeData( timebar );
		        }
		    }
		    else if( finalTimebar == true && series1.Points.Count > 0 )
		    {
		        if( x_axis == currXAxis )
		        {
		            RemovePlaceholderVolumeData( );
    		        AddVolumeData( timebar );

    		        DataPoint candlestick = series1.Points[ series1.Points.Count - 1 ];
    	            candlestick.YValues[ 0 ] = timebar.High;
    		        candlestick.YValues[ 1 ] = timebar.Low;
    		        candlestick.YValues[ 2 ] = timebar.Open;
    		        candlestick.YValues[ 3 ] = timebar.Close;

                    candlestick.ToolTip = "OPEN: " + candlestick.YValues[ 2 ] +
                                          "\nHIGH: " + candlestick.YValues[ 0 ] +
                                          "\nLOW: " + candlestick.YValues[ 1 ] +
                                          "\nCLOSE: " + candlestick.YValues[ 3 ] +
                                          "\nVWAP: " + ( aggregate_val / aggregate_qty ).ToString( "0.000" ) ;
		        }
		        else
		        {
		            AddTimebarData( timebar );
		            AddVolumeData( timebar );
		        }
		    }
		    else if( finalTimebar == true && series1.Points.Count == 0 )
		    {
		        AddTimebarData( timebar );
		        AddVolumeData( timebar );
		    }
        }

        public void UpdateBidAsk( double bid, double ask )
        {
            bidAskStripLine.StripWidth = ask - bid;
            bidAskStripLine.IntervalOffset = bid;
        }

        public void Redraw()
        {
            chart1.Invalidate( true );
            chart1.Update();
        }
    }
}
