using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using Client.Application.ViewModels;
using Client.Domain.AI;
using Client.Domain.Events;
using Client.Domain.Factories;
using Client.Domain.Parsers;
using Client.Domain.Service;
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
        private readonly AIInterface ai;

        public Bot(
            IServiceProvider serviceProvider,
            string dllName
        )
        {
            transport = serviceProvider.GetRequiredService<TransportInterface>();
            messageParser = serviceProvider.GetRequiredService<MessageParserInterface>();
            entityHandlerFactory = serviceProvider.GetRequiredService<EntityHandlerFactoryInterface>();
            eventBus = serviceProvider.GetRequiredService<EventBusInterface>();
            ai = serviceProvider.GetRequiredService<AIInterface>();
            this.serviceProvider = serviceProvider;
            this.dllName = dllName;
        }

        public async Task StartAsync()
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
            var aiTask = Task.Run(async () =>
            {
                while (true)
                {
                    await ai.Update();
                }
            });
            while (true)
            {
                await transport.ReceiveAsync();
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
            eventBus.Subscrbe((EventHandlerInterface<DropCreatedEvent>)viewModel);
            eventBus.Subscrbe((EventHandlerInterface<DropDeletedEvent>)viewModel);
            eventBus.Subscrbe((EventHandlerInterface<ChatMessageCreatedEvent>)viewModel);
            eventBus.Subscrbe((EventHandlerInterface<SkillCreatedEvent>)viewModel);
            eventBus.Subscrbe((EventHandlerInterface<SkillDeletedEvent>)viewModel);
            eventBus.Subscrbe((EventHandlerInterface<ItemCreatedEvent>)viewModel);
            eventBus.Subscrbe((EventHandlerInterface<ItemDeletedEvent>)viewModel);

            var worldHandler = serviceProvider.GetRequiredService<WorldHandler>();
            eventBus.Subscrbe((EventHandlerInterface<HeroCreatedEvent>)worldHandler);
            eventBus.Subscrbe((EventHandlerInterface<HeroDeletedEvent>)worldHandler);
            eventBus.Subscrbe((EventHandlerInterface<CreatureCreatedEvent>)worldHandler);
            eventBus.Subscrbe((EventHandlerInterface<CreatureDeletedEvent>)worldHandler);
            eventBus.Subscrbe((EventHandlerInterface<DropCreatedEvent>)worldHandler);
            eventBus.Subscrbe((EventHandlerInterface<DropDeletedEvent>)worldHandler);
            eventBus.Subscrbe((EventHandlerInterface<SkillCreatedEvent>)worldHandler);
            eventBus.Subscrbe((EventHandlerInterface<SkillDeletedEvent>)worldHandler);
            eventBus.Subscrbe((EventHandlerInterface<ItemCreatedEvent>)worldHandler);
            eventBus.Subscrbe((EventHandlerInterface<ItemDeletedEvent>)worldHandler);

            eventBus.Subscrbe(serviceProvider.GetRequiredService<HeroHandler>());
            eventBus.Subscrbe(serviceProvider.GetRequiredService<NpcHandler>());
            eventBus.Subscrbe(serviceProvider.GetRequiredService<PlayerHandler>());

            var configViewModel = serviceProvider.GetRequiredService<AIConfigViewModel>();
            eventBus.Subscrbe((EventHandlerInterface<HeroCreatedEvent>)configViewModel);
            eventBus.Subscrbe((EventHandlerInterface<HeroDeletedEvent>)configViewModel);
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
