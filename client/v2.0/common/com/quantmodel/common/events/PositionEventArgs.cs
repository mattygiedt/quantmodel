using System;

namespace com.quantmodel.common.events
{
    public sealed class PositionEventArgs : EventArgs
    {
        private readonly object data;
        public object Data { get { return data; } }

        public PositionEventArgs( object data )
        {
            this.data = data;
        }
    }
}