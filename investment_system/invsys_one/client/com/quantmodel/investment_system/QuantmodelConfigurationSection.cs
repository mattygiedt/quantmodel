using System;
using System.Xml;
using System.Text;
using System.Collections;
using System.Configuration;

//
//  http://msdn.microsoft.com/en-us/library/2tw134k3.aspx#4
//  http://www.codeproject.com/KB/dotnet/mysteriesofconfiguration.aspx
//

namespace com.quantmodel.investment_system
{
    public class ConnectionElement : ConfigurationElement
    {
        [ConfigurationProperty( "name", IsRequired=true )]
        public string Name
        {
            get { return (string)this[ "name" ]; }
        }

        [ConfigurationProperty( "timeout", IsRequired=true )]
        public string Timeout
        {
            get { return (string)this[ "timeout" ]; }
        }

        [ConfigurationProperty( "broadcast", IsRequired=true )]
        public string Broadcast
        {
            get { return (string)this[ "broadcast" ]; }
        }

        [ConfigurationProperty( "callforward", IsRequired=true )]
        public string Callforward
        {
            get { return (string)this[ "callforward" ]; }
        }

        [ConfigurationProperty( "price_format", IsRequired=true )]
        public string PriceFormat
        {
            get { return (string)this[ "price_format" ]; }
        }
    }

    [ConfigurationCollection( typeof(ConnectionElement), AddItemName="connection", CollectionType=ConfigurationElementCollectionType.BasicMap )]
    public class ConnectionGroup : ConfigurationElementCollection
    {
        public override ConfigurationElementCollectionType CollectionType
        {
            get { return ConfigurationElementCollectionType.BasicMap; }
        }

        protected override string ElementName
        {
            get { return "connection"; }
        }

        public ConnectionElement this[ int index ]
        {
            get { return (ConnectionElement)base.BaseGet( index ); }
            set
            {
                if( base.BaseGet( index ) != null )
                {
                    base.BaseRemoveAt( index );
                }

                base.BaseAdd( index, value );
            }
        }

        public new ConnectionElement this[ string name ]
        {
            get { return (ConnectionElement)base.BaseGet( name ); }
        }

        protected override ConfigurationElement CreateNewElement()
        {
            return new ConnectionElement();
        }

        protected override object GetElementKey( ConfigurationElement element )
        {
            return (element as ConnectionElement).Name;
        }
    }

    public class QuantmodelConfiguration : ConfigurationSection
    {
        public QuantmodelConfiguration() { }

        /*

        */

        [ConfigurationProperty( "datetime_format", IsRequired=true )]
        public string DatetimeFormat
        {
            get { return (string)this[ "datetime_format" ]; }
        }

        [ConfigurationProperty( "fcm_id", IsRequired=true )]
        public string FcmId
        {
            get { return (string)this[ "fcm_id" ]; }
        }

        [ConfigurationProperty( "connectionGroup" )]
        public ConnectionGroup ConnectionGroup
        {
            get { return (ConnectionGroup)this[ "connectionGroup" ]; }
        }
    }
}