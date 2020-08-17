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
    public sealed class CurrencySubItem : GTLSubItem
    {
        bool color_flag = false;

        public CurrencySubItem( string value ) : base()
        {
            Update( Convert.ToDouble( value ) );
        }

        public CurrencySubItem( double value, bool flag ) : base()
        {
            this.color_flag = flag;
            Update( value );
        }

        public void Update( double value )
        {
            this.Text = value.ToString( "C" );

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
