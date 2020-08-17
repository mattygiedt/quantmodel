using System;
using System.Drawing;

using GlacialComponents.Controls.GlacialTreeList;
using GlacialComponents.Controls.GTLCommon;

namespace com.quantmodel.common.controls
{
    public abstract class ColorSubItem
    {
        protected Color [] colors;
        protected GTLSubItem item;
        protected int step = 0;
        protected object tag;

        public object Tag { get { return tag; } }
        public bool Remaining { get { return ( step < colors.Length ); } }

        public void FireColor()
        {
            if( Remaining )
                item.BackColor = colors[ step++ ];
        }

        public override bool Equals( System.Object obj )
        {
            if( obj == null )
            {
                return false;
            }

            ColorSubItem csi = obj as ColorSubItem;

            if( (System.Object)csi == null )
            {
                return false;
            }

            return csi.Tag.Equals( Tag );
        }

        public bool Equals( ColorSubItem csi )
        {
            if( null == csi )
            {
                return false;
            }

            return csi.Tag.Equals( Tag );
        }

        public override int GetHashCode()
        {
            return Tag.GetHashCode();
        }
    }

    public sealed class OrangeColorSubItem : ColorSubItem
    {
        private static readonly Color O1 = Color.FromArgb( 0x78FF6600 );
        private static readonly Color O2 = Color.FromArgb( 0x68FF6600 );
        private static readonly Color O3 = Color.FromArgb( 0x58FF6600 );
        private static readonly Color O4 = Color.FromArgb( 0x48FF6600 );
        private static readonly Color O5 = Color.FromArgb( 0x38FF6600 );
        private static readonly Color O6 = Color.FromArgb( 0x28FF6600 );
        private static readonly Color O7 = Color.FromArgb( 0x18FF6600 );

        public OrangeColorSubItem( Color final, GTLSubItem item, object tag )
        {
            this.item = item;
            this.tag = tag;
            this.colors = new Color [ ] { O1,O2,O3,O4,O5,O6,O7,final };
        }
    }

    public sealed class BlueColorSubItem : ColorSubItem
    {
        private static readonly Color O1 = Color.FromArgb( 0x780000FF );
        private static readonly Color O2 = Color.FromArgb( 0x680000FF );
        private static readonly Color O3 = Color.FromArgb( 0x580000FF );
        private static readonly Color O4 = Color.FromArgb( 0x480000FF );
        private static readonly Color O5 = Color.FromArgb( 0x380000FF );
        private static readonly Color O6 = Color.FromArgb( 0x280000FF );
        private static readonly Color O7 = Color.FromArgb( 0x180000FF );

        public BlueColorSubItem( Color final, GTLSubItem item, object tag )
        {
            this.item = item;
            this.tag = tag;
            this.colors = new Color [ ] { O1,O2,O3,O4,O5,O6,O7,final };
        }
    }

    public sealed class GreenColorSubItem : ColorSubItem
    {
        private static readonly Color G1 = Color.FromArgb( 0x7800FF00 );
        private static readonly Color G2 = Color.FromArgb( 0x6800FF00 );
        private static readonly Color G3 = Color.FromArgb( 0x5800FF00 );
        private static readonly Color G4 = Color.FromArgb( 0x4800FF00 );
        private static readonly Color G5 = Color.FromArgb( 0x3800FF00 );
        private static readonly Color G6 = Color.FromArgb( 0x2800FF00 );
        private static readonly Color G7 = Color.FromArgb( 0x1800FF00 );

        public GreenColorSubItem( Color final, GTLSubItem item, object tag )
        {
            this.item = item;
            this.tag = tag;
            this.colors = new Color [ ] { G1,G2,G3,G4,G5,G6,G7,final };
        }
    }

    public sealed class RedColorSubItem : ColorSubItem
    {
        private static readonly Color R1 = Color.FromArgb( 0x78FF0000 );
        private static readonly Color R2 = Color.FromArgb( 0x68FF0000 );
        private static readonly Color R3 = Color.FromArgb( 0x58FF0000 );
        private static readonly Color R4 = Color.FromArgb( 0x48FF0000 );
        private static readonly Color R5 = Color.FromArgb( 0x38FF0000 );
        private static readonly Color R6 = Color.FromArgb( 0x28FF0000 );
        private static readonly Color R7 = Color.FromArgb( 0x18FF0000 );

        public RedColorSubItem( Color final, GTLSubItem item, object tag )
        {
            this.item = item;
            this.tag = tag;
            this.colors = new Color [ ] { R1,R2,R3,R4,R5,R6,R7,final };
        }
    }
}
