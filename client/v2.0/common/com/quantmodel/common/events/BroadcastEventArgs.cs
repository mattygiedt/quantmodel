using System;

using quantmodel;

namespace com.quantmodel.common.events
{
    public sealed class BroadcastEventArgs : EventArgs
    {
        private readonly ClientAdapterBroadcast data;
        public ClientAdapterBroadcast Data { get { return data; } }

        public BroadcastEventArgs( ClientAdapterBroadcast data )
        {
            this.data = data;
        }
    }
}