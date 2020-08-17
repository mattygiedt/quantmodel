using System;

namespace com.quantmodel.common.events
{
    public sealed class StringEventArgs : EventArgs
    {
        private readonly string data;
        public string Data { get { return data; } }

        public StringEventArgs( string data )
        {
            this.data = data;
        }
    }
}