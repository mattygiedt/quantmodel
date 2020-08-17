using System;
using System.IO;
using System.Xml;
using System.Windows;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using System.Drawing.Design;
using System.Configuration;
using System.Windows.Forms;
using System.ComponentModel;
using System.Globalization;

using GlacialComponents.Controls.GlacialTreeList;
using GlacialComponents.Controls.GTLCommon;

namespace com.quantmodel.common.controls
{
    public sealed class PercentSubItem : GTLSubItem
    {
        bool color_flag = false;
        public PercentSubItem( double value, bool flag ) : base()
        {
            this.color_flag = flag;
            Update( value );
        }

        public void Update( double value )
        {
            this.Text = string.Concat( value.ToString( "0.000" ), "%" );

            if( color_flag )
            {
                if( 0.0 > value )
                {
                    this.ForeColor = Color.Red;
                }
                else if( 0.0 < value )
                {
                    this.ForeColor = Color.Green;
                }
                else
                {
                    this.ForeColor = Color.Black;
                }
            }
        }
    }
}
