using System;
using System.Collections;
using System.Collections.Specialized;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Design;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Globalization;
using System.Reflection;
using System.Threading;
using System.Windows.Forms;
using System.Windows.Forms.Design;

using GlacialComponents.Controls.GlacialTreeList;
using GlacialComponents.Controls.GTLCommon;

namespace com.quantmodel.common.controls
{
	public class CompareById : IComparerHelper, IComparer
	{
	    private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( CompareById ) );

	    private string id_prefix;
	    private int columnIndex = -1;
        private GSortDirectionTypes sortDirection;

        public int ColumnIndex
		{
			get { return columnIndex; }
			set { columnIndex = value; }
		}

		public GSortDirectionTypes SortDirection
		{
			get { return sortDirection; }
			set	{ sortDirection = value; }
		}

		public IComparer Comparer
		{
			get { return this; }
		}

		public CompareById( string id_prefix )
		{
		    this.id_prefix = id_prefix;
		}

		public int Compare( object x, object y )
		{
			int nSubIndex = columnIndex;

			string a = ((GTLTreeNode)x).SubItems[ nSubIndex ].Text;
			string b = ((GTLTreeNode)y).SubItems[ nSubIndex ].Text;

            if( null == a || a.Equals( "" ) )
            {
                a = id_prefix + "0";
            }

            if( null == b || b.Equals( "" ) )
            {
                b = id_prefix + "0";
            }

            //
            //  Parse out the order_id
            //

            if( a.StartsWith( id_prefix ) && b.StartsWith( id_prefix ) )
            {
                int order_id_a = Convert.ToInt32( a.Substring( 1 ) );
                int order_id_b = Convert.ToInt32( b.Substring( 1 ) );

                if ( sortDirection == GSortDirectionTypes.Ascending )
    			{
    				if ( order_id_a > order_id_b )
    					return 1;
    				if ( order_id_a < order_id_b )
    					return -1;
    			}
    			else
    			{
    				if ( order_id_a < order_id_b )
    					return 1;
    				if ( order_id_a > order_id_b )
    					return -1;
    			}

    			return 0;	// even
            }
            else
            {
                //
                //  Invalid ID!!!
                //

                return 0;
            }
		}
	}
}
