using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Collections;

using quantmodel;
using Google.ProtocolBuffers.Descriptors;

namespace com.quantmodel.common.data
{
    public class OvernightPosition : DatabaseObject
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( OvernightPosition ) );

        public OvernightPosition( PositionOvernightData data )
        {
            MessageDescriptor d = PositionOvernightData.Descriptor;

            foreach( FieldDescriptor f in d.Fields )
            {
                if( f.Name.Equals( "position_type" ) )
                {
                    if( data.PositionType == PositionOvernightData.Types.PositionType.LONG )
                    {
                        fields.Add( "position_type", "1" );
                    }
                    else
                    {
                        fields.Add( "position_type", "2" );
                    }
                }
                else
                {
                    fields.Add( f.Name, data[ f ] );
                }
            }

            this.id = this[ POSITION_OVERNIGHT_ID ];
        }

        public OvernightPosition( DatabaseEngineResponse.Types.ResultSet.Types.Row row )
        {
            foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row.Types.Column col in row.ColumnList )
            {
                fields.Add( col.Name, col.Value );
            }

            this.id = this[ POSITION_OVERNIGHT_ID ];
        }

		public OvernightPosition( XmlNode node )
		{
		    foreach( XmlNode attr in node.Attributes )
		    {
                fields.Add( attr.Name, attr.Value );
		    }

		    this.id = this[ POSITION_OVERNIGHT_ID ];
		}
	}
}
