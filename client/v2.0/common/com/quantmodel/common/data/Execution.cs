using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Collections;

using quantmodel;
using Google.ProtocolBuffers.Descriptors;

namespace com.quantmodel.common.data
{
    public class Execution : DatabaseObject
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( Execution ) );

        public Execution( ExecutionData data )
        {
            MessageDescriptor d = ExecutionData.Descriptor;

            foreach( FieldDescriptor f in d.Fields )
            {
                fields.Add( f.Name, data[ f ] );
            }

            this.id = this[ EXECUTION_ID ];
        }

        public Execution( DatabaseEngineResponse.Types.ResultSet.Types.Row row )
        {
            foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row.Types.Column col in row.ColumnList )
            {
                fields.Add( col.Name, col.Value );
            }

            this.id = this[ EXECUTION_ID ];
        }

		public Execution( XmlNode node )
		{
		    foreach( XmlNode attr in node.Attributes )
		    {
                fields.Add( attr.Name, attr.Value );
		    }

		    this.id = this[ EXECUTION_ID ];
		}

		public bool IsSpreadExecution { get { return this[ LEG_TYPE ] == "3"; } }
	}
}
