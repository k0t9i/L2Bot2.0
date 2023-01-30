using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Client.Domain.Factories;
using Client.Domain.Parsers;
using Client.Domain.Transports;

namespace Client
{
    public class Bot
    {
        [DllImport("kernel32.dll", EntryPoint = "LoadLibrary", SetLastError = true)]
        static extern int LoadLibrary([MarshalAs(UnmanagedType.LPStr)] string lpLibFileName);

        private readonly TransportInterface transport;
        private readonly MessageParserInterface messageParser;
        private readonly EntityHandlerFactoryInterface entityHandlerFactory;
        private readonly string dllName;

        public Bot(TransportInterface transport, MessageParserInterface messageParser, EntityHandlerFactoryInterface entityHandlerFactory, string dllName)
        {
            this.transport = transport;
            this.messageParser = messageParser;
            this.entityHandlerFactory = entityHandlerFactory;
            this.dllName = dllName;
        }

        public async void StartAsync()
        {
            int hDll = LoadLibrary(dllName);

            if (hDll == 0)
            {
                throw new Exception("Unable to load library " + dllName + ": " + Marshal.GetLastWin32Error().ToString());
            }

            Debug.WriteLine(dllName + " loaded\n");
            transport.Message += OnMessage;

            await transport.ConnectAsync();
            await transport.SendAsync("invalidate");
            while (true)
            {
                await transport.StartReceiveAsync();
                await transport.ConnectAsync();
            }
        }

        private void OnMessage(string args)
        {
            try
            {
                var message = messageParser.Parse(args);
                try
                {
                    var handler = entityHandlerFactory.GetHandler(message.Type);
                    handler.Update(message.Operation, message.Content);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine("Exception: " + ex.Message);
                }
            }
            catch (Domain.Exception.ParserException)
            {
                Debug.WriteLine("Unable to parse message: " + args);
            }
        }
    }
}
