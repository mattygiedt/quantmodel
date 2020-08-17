using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Collections;

using quantmodel;

namespace com.quantmodel.common.data
{
    public class User : DatabaseObject
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( User ) );

        private readonly ArrayList applications = new ArrayList();

        public bool IsComplianceUser
        {
            get
            {
                foreach( string application in applications )
                {
                    if( "COMPLNCE".Equals( application ) ) return true;
                }
                return false;
            }
        }

        public bool IsClientUser
        {
            get
            {
                foreach( string application in applications )
                {
                    if( "CLIENT".Equals( application ) ) return true;
                }
                return false;
            }
        }

        public User( DatabaseEngineResponse.Types.ResultSet.Types.Row row )
        {
            foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row.Types.Column col in row.ColumnList )
            {
                fields.Add( col.Name, col.Value );
            }

            this.id = this[ USER_ID ];
        }

		public User( XmlNode node )
		{
		    foreach( XmlNode attr in node.Attributes )
		    {
                fields.Add( attr.Name, attr.Value );
		    }

		    this.id = this[ USER_ID ];
		}

		public void AddApplication( DatabaseEngineResponse.Types.ResultSet.Types.Row row )
		{
            foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row.Types.Column col in row.ColumnList )
            {
                applications.Add( col.Value );
            }
		}
	}
}
