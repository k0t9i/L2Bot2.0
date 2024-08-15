using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Transports
{
    public interface TransportInterface
    {
        public event DelegateMessage Message;

        bool IsConnected();
        Task ConnectAsync();
        Task SendAsync(string data);
        Task ReceiveAsync();

        public delegate void DelegateMessage(string args);
    }
}
