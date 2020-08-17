using System;
using System.Drawing;

using GlacialComponents.Controls.GlacialTreeList;
using GlacialComponents.Controls.Common;
using GlacialComponents.Controls.GTLCommon;

namespace com.quantmodel.common.controls
{
	public sealed class ControlHelper
	{
		public static GlacialTreeList NewGlacialTreeList( string name )
		{
		    GlacialTreeList list = new GlacialTreeList( "0DAA8D90BB9ACC06C6FDDF11F7DD73E8" );
		    list.Name = name;
            list.BackColor = System.Drawing.SystemColors.Window;
            list.BackgroundStretchToFit = true;
            list.Dock = System.Windows.Forms.DockStyle.Fill;
            list.ItemHeight = 19;
            list.Location = new System.Drawing.Point( 0, 0 );
            list.MultiSelect = false;
            list.ShowPlusMinus = false;
            list.HotColumnTracking = false;
            list.HotNodeTracking = false;
            list.SnapLastColumn = true;
            list.AlternateBackground = Color.AliceBlue;
            list.UnfocusedSelectionColor = Color.LightGray;
            list.ShowRootLines = false;
            list.ShowFocusRect = true;
            list.FastFind = false;
            list.AlternatingColors = false;
            list.OptimizeForHighNodeCount = false;
            list.HotTrackingColor = SystemColors.HotTrack;
            list.SelectedTextColor = SystemColors.HighlightText;
            list.SelectionColor =  SystemColors.Highlight;
            list.ControlStyle = GTLControlStyles.XP;
            list.Font = new System.Drawing.Font( "Tahoma", 9.25F );

            return list;
        }
	}
}
