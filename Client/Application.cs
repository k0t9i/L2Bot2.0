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
    public class Application
    {
        [DllImport("kernel32.dll", EntryPoint = "LoadLibrary", SetLastError = true)]
        static extern int LoadLibrary([MarshalAs(UnmanagedType.LPStr)] string lpLibFileName);

        private readonly TransportInterface transport;
        private readonly MessageParserInterface messageParser;
        private readonly EntityHandlerFactoryInterface entityHandlerFactory;
        private readonly string dllName;

        public Application(TransportInterface transport, MessageParserInterface messageParser, EntityHandlerFactoryInterface entityHandlerFactory, string dllName)
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

            while (true)
            {
                await transport.ConnectAsync();
                await transport.StartReceiveAsync();
            }
        }

        private void OnMessage(string args)
        {
            try
            {
                var message = messageParser.Parse(args);
                Debug.WriteLine(message);

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
