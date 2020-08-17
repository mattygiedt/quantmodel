using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Collections;

using quantmodel;
using Google.ProtocolBuffers.Descriptors;

namespace com.quantmodel.common.data
{
    public class Release : DatabaseObject
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( Release ) );

        public Release( ReleaseData data )
        {
            MessageDescriptor d = ReleaseData.Descriptor;

            foreach( FieldDescriptor f in d.Fields )
            {
                fields.Add( f.Name, data[ f ] );
            }

            this.id = this[ RELEASE_ID ];
        }

        public Release( DatabaseEngineResponse.Types.ResultSet.Types.Row row )
        {
            foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row.Types.Column col in row.ColumnList )
            {
                fields.Add( col.Name, col.Value );
            }

            this.id = this[ RELEASE_ID ];
        }

		public Release( XmlNode node )
		{
		    foreach( XmlNode attr in node.Attributes )
		    {
                fields.Add( attr.Name, attr.Value );
		    }

		    this.id = this[ RELEASE_ID ];
		}
	}
}
