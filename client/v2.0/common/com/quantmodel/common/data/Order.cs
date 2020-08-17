using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Collections;

using quantmodel;
using Google.ProtocolBuffers.Descriptors;

namespace com.quantmodel.common.data
{
    public class Order : DatabaseObject
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( Order ) );

        private readonly ArrayList allocations = new ArrayList();
        public ArrayList Allocations { get { return new ArrayList( allocations ); } }

        public Order( OrderData data )
        {
            MessageDescriptor d = OrderData.Descriptor;

            foreach( FieldDescriptor f in d.Fields )
            {
                fields.Add( f.Name, data[ f ] );
            }

            this.id = this[ ORDER_ID ];
        }

        public Order( DatabaseEngineResponse.Types.ResultSet.Types.Row row )
        {
            foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row.Types.Column col in row.ColumnList )
            {
                fields.Add( col.Name, col.Value );
            }

            this.id = this[ ORDER_ID ];
        }

		public Order( XmlNode node )
		{
		    foreach( XmlNode attr in node.Attributes )
		    {
                fields.Add( attr.Name, attr.Value );
		    }

		    foreach( XmlNode alloc_node in node.ChildNodes )
		    {
		        allocations.Add( new Allocation( alloc_node ) );
		    }

		    this.id = this[ ORDER_ID ];
		}

		public void AddAllocation( Allocation alloc )
		{
		    allocations.Add( alloc );
		}
	}
}
