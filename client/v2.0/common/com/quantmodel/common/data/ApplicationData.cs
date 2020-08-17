using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Collections;

namespace com.quantmodel.common.data
{
    public class ApplicationData : DatabaseObject
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( ApplicationData ) );

		public ApplicationData( XmlNode node )
		{
		    foreach( XmlNode attr in node.Attributes )
		    {
                fields.Add( attr.Name, attr.Value );
		    }

		    this.id = this[ APPLICATION_ID ];
		}
	}
}
