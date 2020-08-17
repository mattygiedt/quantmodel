using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Collections;

using quantmodel;

namespace com.quantmodel.common.data
{
    public class Instrument : DatabaseObject, IComparable
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( Instrument ) );

        public Instrument( DatabaseEngineResponse.Types.ResultSet.Types.Row row )
        {
            foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row.Types.Column col in row.ColumnList )
            {
                fields.Add( col.Name, col.Value );
            }

            this.id = this[ INSTRUMENT_ID ];
        }

		public Instrument( XmlNode node )
		{
		    foreach( XmlNode attr in node.Attributes )
		    {
                fields.Add( attr.Name, attr.Value );
		    }

		    this.id = this[ INSTRUMENT_ID ];
		}

		public int CompareTo( object obj )
        {
            Instrument compare = (Instrument)obj;
            return this[ NAME ].CompareTo( compare[ NAME ] );
        }
	}
}
