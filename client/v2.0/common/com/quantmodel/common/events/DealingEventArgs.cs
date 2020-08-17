using System;

namespace com.quantmodel.common.events
{
    public sealed class DealingEventArgs : EventArgs
    {
        private readonly object data;
        public object Data { get { return data; } }

        private readonly object old_data;
        public object OldData { get { return old_data; } }

        public DealingEventArgs( object data )
        {
            this.data = data;
        }

        public DealingEventArgs( object data, object old_data )
        {
            this.data = data;
            this.old_data = old_data;
        }
    }
}