using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Client.Application.ViewModels;
using Client.Domain.Events;
using Client.Domain.Factories;
using Client.Domain.Parsers;
using Client.Domain.Transports;
using Microsoft.Extensions.DependencyInjection;

namespace Client
{
    public class Bot
    {
        [DllImport("kernel32.dll", EntryPoint = "LoadLibrary", SetLastError = true)]
        static extern int LoadLibrary([MarshalAs(UnmanagedType.LPStr)] string lpLibFileName);

        private readonly TransportInterface transport;
        private readonly MessageParserInterface messageParser;
        private readonly EntityHandlerFactoryInterface entityHandlerFactory;
        private readonly EventBusInterface eventBus;
        private readonly IServiceProvider serviceProvider;
        private readonly string dllName;

        public Bot(
            IServiceProvider serviceProvider,
            string dllName
        )
        {
            transport = serviceProvider.GetRequiredService<TransportInterface>();
            messageParser = serviceProvider.GetRequiredService<MessageParserInterface>();
            entityHandlerFactory = serviceProvider.GetRequiredService<EntityHandlerFactoryInterface>();
            eventBus = serviceProvider.GetRequiredService<EventBusInterface>();
            this.serviceProvider = serviceProvider;
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

            SubscribeAllHandlers();

            await transport.ConnectAsync();
            await transport.SendAsync("invalidate");
            while (true)
            {
                await transport.StartReceiveAsync();
                await transport.ConnectAsync();
            }
        }

        private void SubscribeAllHandlers()
        {
            var viewModel = serviceProvider.GetRequiredService<MainViewModel>();
            eventBus.Subscrbe((EventHandlerInterface<HeroCreatedEvent>)viewModel);
            eventBus.Subscrbe((EventHandlerInterface<HeroDeletedEvent>)viewModel);
            eventBus.Subscrbe((EventHandlerInterface<CreatureCreatedEvent>)viewModel);
            eventBus.Subscrbe((EventHandlerInterface<CreatureDeletedEvent>)viewModel);
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
