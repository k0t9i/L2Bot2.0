﻿using Client.Domain.Factories;
using Client.Infrastructure.Factories;
using Client.Domain.Parsers;
using Client.Infrastructure.Parsers;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using System.Windows;
using Client.Domain.Transports;
using Client.Infrastructure.Transports;
using Client.Domain.Entities;
using Client.Domain.Service;
using Client.Domain.ValueObjects;
using Microsoft.Extensions.Configuration;
using System.Reflection;
using Client.Application.Views;
using Client.Application.ViewModels;
using Client.Domain.Helpers;
using Client.Infrastructure.Helpers;
using Client.Domain.Events;
using Client.Infrastructure.Events;
using System;
using Client.Infrastructure.Service;
using System.Collections.Generic;
using System.Linq;
using Client.Domain.AI;
using Client.Infrastructure.AI.IO;
using Client.Domain.AI.IO;

namespace Client
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : System.Windows.Application
    {
        public static IHost? AppHost { get; private set; }

        public App()
        {
            AppHost = Host.CreateDefaultBuilder()
                .ConfigureServices(
                    (hostContext, services) => ConfigureServices(services)
                )
                .Build();
        }

        protected override async void OnStartup(StartupEventArgs e)
        {
            await AppHost!.StartAsync();

            var startupForm = AppHost.Services.GetRequiredService<MainWindow>();
            startupForm.Show();

            base.OnStartup(e);

            var application = AppHost.Services.GetRequiredService<Bot>();
            await application.StartAsync();
        }

        protected override async void OnExit(ExitEventArgs e)
        {
            await AppHost!.StopAsync();

            base.OnExit(e);
        }

        private void ConfigureServices(IServiceCollection services)
        {
            IConfiguration config = new ConfigurationBuilder()
                .AddJsonFile("config.json", optional: false, reloadOnChange: true)
                .AddJsonFile("Assets/data/experience.json", optional: false, reloadOnChange: true)
                .AddJsonFile("Assets/data/npcInfo.json", optional: false, reloadOnChange: true)
                .AddJsonFile("Assets/data/itemInfo.json", optional: false, reloadOnChange: true)
                .AddJsonFile("Assets/data/skillInfo.json", optional: false, reloadOnChange: true)
                .Build();

            services
                .AddSingleton(typeof(IConfiguration), config)
                .AddSingleton<MainWindow>()
                .AddSingleton(
                    typeof(Bot),
                    x => new Bot(
                            x.GetRequiredService<IServiceProvider>(),
                            config.GetValue<string>("DLLName") ?? ""
                        )
                    )
                .AddSingleton(typeof(EntityHandlerFactoryInterface), typeof(EntityHandlerFactory))
                .AddSingleton(typeof(MessageParserInterface), typeof(JsonMessageParser))
                .AddSingleton(typeof(OutgoingMessageBuilderInterface), typeof(JsonOutgoingMessageBuilder))
                .AddSingleton(
                    typeof(TransportInterface),
                    x => new NamedPipeTransport(
                        config.GetValue<string>("ConnectionPipeName") ?? ""
                    )
                )
                .AddSingleton(typeof(ExperienceHelperInterface), typeof(ConfigurationExperienceHelper))
                .AddSingleton(typeof(NpcInfoHelperInterface), typeof(ConfigurationNpcInfoHelper))
                .AddSingleton(typeof(ItemInfoHelperInterface), typeof(ConfigurationItemInfoHelper))
                .AddSingleton(typeof(SkillInfoHelperInterface), typeof(ConfigurationSkillInfoHelper))
                .AddSingleton(typeof(EventBusInterface), typeof(InMemoryEventBus))

                .AddTransient(typeof(EntityFactoryInterface<Entity>), typeof(EntityFactory<Entity>))
                .AddTransient(typeof(EntityFactoryInterface<Hero>), typeof(EntityFactory<Hero>))
                .AddTransient(typeof(EntityFactoryInterface<Drop>), typeof(EntityFactory<Drop>))
                .AddTransient(typeof(EntityFactoryInterface<NPC>), typeof(EntityFactory<NPC>))
                .AddTransient(typeof(EntityFactoryInterface<Player>), typeof(EntityFactory<Player>))
                .AddTransient(typeof(EntityFactoryInterface<ChatMessage>), typeof(EntityFactory<ChatMessage>))
                .AddTransient(typeof(EntityFactoryInterface<Skill>), typeof(EntityFactory<Skill>))
                .AddTransient(typeof(EntityFactoryInterface<ItemInterface>), typeof(ItemFactory))

                .AddSingleton<HeroHandler>()
                .AddSingleton<DropHandler>()
                .AddSingleton<NpcHandler>()
                .AddSingleton<PlayerHandler>()
                .AddSingleton<ChatMessageHandler>()
                .AddSingleton<SkillHandler>()
                .AddSingleton<ItemHander>()
                .AddSingleton<WorldHandler>()

                .AddSingleton(
                    typeof(PathfinderInterface),
                    x => new L2jGeoDataPathfinder(
                        config.GetValue<string>("GeoDataDirectory") ?? "",
                        config.GetValue<ushort>("MaxPassableHeight")
                    )
                 )
                .AddSingleton(
                    typeof(AsyncPathMoverInterface),
                    x => new AsyncPathMover(
                        x.GetRequiredService<WorldHandler>(),
                        x.GetRequiredService<PathfinderInterface>(),
                        config.GetValue<int>("PathNumberOfAttempts"),
                        config.GetValue<double>("NodeWaitingTime"),
                        config.GetValue<int>("NodeDistanceTolerance"),
                        config.GetValue<int>("NextNodeDistanceTolerance")
                    )
                )

                .AddSingleton<Config>()
                .AddSingleton<TransitionBuilderLocator>()
                .AddSingleton(
                    typeof(AIInterface), 
                    x => new AI(
                        x.GetRequiredService<WorldHandler>(),
                        x.GetRequiredService<Config>(),
                        x.GetRequiredService<AsyncPathMoverInterface>(),
                        x.GetRequiredService<TransitionBuilderLocator>()
                    )
                )

                .AddTransient(typeof(ConfigSerializerInterface), typeof(JsonConfigSerializer))
                .AddTransient(typeof(ConfigDeserializerInterface), typeof(JsonConfigDeserializer))

                .AddSingleton<MainViewModel>()
                .AddSingleton<AIConfigViewModel>();
        }
    }
}
