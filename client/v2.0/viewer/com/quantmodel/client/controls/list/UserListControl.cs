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

namespace com.quantmodel.common.controls
{
    public abstract class UserListControl : UserControl
    {
        protected readonly System.Windows.Forms.Timer timer =
            new System.Windows.Forms.Timer();

		protected readonly Hashtable colorNodes = new Hashtable();
        protected GlacialTreeList data = null;

        protected void FireTimerEvent( object o, EventArgs e )
        {
            if( colorNodes.Keys.Count > 0 )
            {
                this.SuspendLayout();
			    data.BeginUpdate();

                ArrayList items = new ArrayList( colorNodes.Values );
                foreach( ColorSubItem item in items )
                {
                    item.FireColor();
                    if(! item.Remaining )
                    {
                        colorNodes.Remove( item.Tag );
                    }
                }

                items.Clear();
    		    data.EndUpdate();
    			this.ResumeLayout( false );
    			this.PerformLayout();
            }
        }

        protected void FireShowColumn( object o, EventArgs e )
        {
            MenuItem item = o as MenuItem;

            foreach( GTLColumn col in data.Columns )
            {
                if( col.Text.Equals( item.Text ) )
                {
                    item.Checked = !item.Checked;
                    col.Visible = item.Checked;
                    break;
                }
            }
        }

        protected void FireRollover( object o, DealingEventArgs e )
        {
            data.BeginUpdate();
            data.BeginNodeListChange();

            data.Nodes.Clear();

            data.EndNodeListChange();
            data.EndUpdate();
        }

        protected void FireViewBy( object o, EventArgs e )
        {

        }

        protected void FireSubItem( GTLTreeNode node, string key, string value )
        {
            if(! node.SubItems[ key ].Text.Equals( value ) )
            {
                node.SubItems[ key ].Text = value;
                ColorSubItem csi = new BlueColorSubItem(
                    Color.White, node.SubItems[ key ],
                    node.Tag + data.Columns[ key ].Text );
                colorNodes[ csi.Tag ] = csi;
            }
        }

	    protected void SortData( GTLColumn col )
	    {
            GSortDirectionTypes sortdir = col.CurrentSortDirection;
            data.BeginUpdate();
            data.BeginNodeListChange();
            col.Sort( data.Nodes );
            data.EndNodeListChange();
		    data.EndUpdate();
            col.CurrentSortDirection = sortdir;
	    }

	    protected string BlankIfNull( string input )
	    {
	        if( input == null )
	        {
	            return "";
	        }
	        else if( input.Equals( "NULL" ) )
	        {
	            return "";
	        }
	        else
	        {
	            return input;
	        }
	    }
    }
}
