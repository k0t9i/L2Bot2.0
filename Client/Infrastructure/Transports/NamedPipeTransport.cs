using System;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static Client.Domain.Transports.TransportInterface;

namespace Client.Infrastructure.Transports
{
    public class NamedPipeTransport : Domain.Transports.TransportInterface, IDisposable
    {
        public event DelegateMessage? Message;

        public async Task ConnectAsync()
        {
            if (!IsConnected())
            {
                Disconnect();

                connectionPipe = new NamedPipeClientStream(this.pipeName);
                await connectionPipe.ConnectAsync();
                connectionPipe.ReadMode = PipeTransmissionMode.Message;
                Debug.WriteLine("Connected to connection pipe");

                byte[] buffer = new byte[16384 * 2];
                int read = connectionPipe.Read(buffer, 0, buffer.Length);

                string pipeName = Encoding.Unicode.GetString(buffer).TrimEnd('\0');

                if (pipeName == "")
                {
                    return;
                }

                Debug.WriteLine("Received connection pipe name " + pipeName);

                mainPipe = new NamedPipeClientStream(".", pipeName, PipeDirection.InOut, PipeOptions.Asynchronous);
                await mainPipe.ConnectAsync();
                mainPipe.ReadMode = PipeTransmissionMode.Message;
                Debug.WriteLine("Connected to main pipe\n");
            }
        }
        public async Task StartReceiveAsync()
        {
            while (IsConnected())
            {
                byte[] buffer = new byte[16384 * 2];
                int readBytes = await mainPipe!.ReadAsync(buffer, 0, buffer.Length);

                if (readBytes != 0)
                {
                    string text = Encoding.Unicode.GetString(buffer).TrimEnd('\0');
                    Message?.Invoke(text);
                }
            }
        }

        public async Task SendAsync(string data)
        {
            if (IsConnected())
            {
                var buffer = Encoding.Unicode.GetBytes(data);

                await mainPipe!.WriteAsync(buffer, 0, buffer.Length);
            }
        }

        public bool IsConnected()
        {
            return connectionPipe != null && connectionPipe.IsConnected && mainPipe != null && mainPipe.IsConnected;
        }

        public void Dispose()
        {
            Disconnect();
        }

        public NamedPipeTransport(string pipeName)
        {
            this.pipeName = pipeName;
        }

        private void Disconnect()
        {
            if (mainPipe != null)
            {
                Debug.WriteLine("Disconnected from main pipe");
                mainPipe.Close();
                mainPipe.Dispose();
                mainPipe = null;
            }
            if (connectionPipe != null)
            {
                Debug.WriteLine("Disconnected from connection pipe");
                connectionPipe.Close();
                connectionPipe.Dispose();
                connectionPipe = null;
            }
        }

        private string pipeName;
        private NamedPipeClientStream? connectionPipe;
        private NamedPipeClientStream? mainPipe;
    }
}
