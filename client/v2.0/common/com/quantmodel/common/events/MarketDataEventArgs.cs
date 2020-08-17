using System;

namespace com.quantmodel.common.events
{
    public sealed class MarketDataEventArgs : EventArgs
    {
        private readonly object data;
        public object Data { get { return data; } }

        public MarketDataEventArgs( object data )
        {
            this.data = data;
        }
    }
}