using System;

using quantmodel;

namespace com.quantmodel.investment_system
{
    public sealed class BroadcastEventArgs : EventArgs
    {
        private readonly InvestmentSystemBroadcast data;
        public InvestmentSystemBroadcast Data { get { return data; } }

        public BroadcastEventArgs( InvestmentSystemBroadcast data )
        {
            this.data = data;
        }
    }
}