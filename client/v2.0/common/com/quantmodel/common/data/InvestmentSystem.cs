using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Collections;

using quantmodel;

namespace com.quantmodel.common.data
{
    public class InvestmentSystem : DatabaseObject, IComparable
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( InvestmentSystem ) );

		private readonly ArrayList strategies = new ArrayList();
        public ArrayList Strategies { get { return new ArrayList( strategies ); } }

        public InvestmentSystem( DatabaseEngineResponse.Types.ResultSet.Types.Row row )
        {
            foreach( DatabaseEngineResponse.Types.ResultSet.Types.Row.Types.Column col in row.ColumnList )
            {
                fields.Add( col.Name, col.Value );
            }

            this.id = this[ INVESTMENT_SYSTEM_ID ];
        }

		public InvestmentSystem( XmlNode node )
		{
		    foreach( XmlNode attr in node.Attributes )
		    {
                fields.Add( attr.Name, attr.Value );
		    }

		    this.id = this[ INVESTMENT_SYSTEM_ID ];
		}

		public void AddStrategy( Strategy strategy )
		{
		    strategies.Add( strategy );
		}

		public int CompareTo( object obj )
        {
            InvestmentSystem compare = (InvestmentSystem)obj;
            return this[ NAME ].CompareTo( compare[ NAME ] );
        }
	}
}
