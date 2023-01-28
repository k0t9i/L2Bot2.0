using Client.Domain.Factories;
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
using BaseApp = System.Windows.Application;
using Client.Domain.ValueObjects;

namespace Client
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : BaseApp
    {
        public static IHost? AppHost { get; private set; }

        public App()
        {
            AppHost = Host.CreateDefaultBuilder()
                .ConfigureServices((hostContext, services) => {
                    services
                        .AddSingleton<MainWindow>()
                        .AddSingleton(
                            typeof(Application),
                            x => new Application(
                                x.GetRequiredService<TransportInterface>(),
                                x.GetRequiredService<MessageParserInterface>(),
                                x.GetRequiredService<EntityHandlerFactoryInterface>(),
                                "L2BotDll.dll"
                             )
                         )
                        .AddSingleton(typeof(EntityHandlerFactoryInterface), typeof(EntityHandlerFactory))
                        .AddSingleton(typeof(MessageParserInterface), typeof(JsonMessageParser))
                        .AddSingleton(typeof(TransportInterface), x => new NamedPipeTransport("PipeL2Bot"))

                        .AddTransient(typeof(EntityFactoryInterface<Hero>), typeof(EntityFactory<Hero>))
                        .AddTransient(typeof(EntityFactoryInterface<Drop>), typeof(EntityFactory<Drop>))
                        .AddTransient(typeof(EntityFactoryInterface<NPC>), typeof(EntityFactory<NPC>))
                        .AddTransient(typeof(EntityFactoryInterface<Player>), typeof(EntityFactory<Player>))
                        .AddTransient(typeof(EntityFactoryInterface<ChatMessage>), typeof(EntityFactory<ChatMessage>))

                        .AddSingleton<EntityHandler<Hero>>()
                        .AddSingleton<EntityHandler<Drop>>()
                        .AddSingleton<EntityHandler<NPC>>()
                        .AddSingleton<EntityHandler<Player>>()
                        .AddSingleton<ChatMessageHandler>();
                })
                .Build();
        }

        protected override async void OnStartup(StartupEventArgs e)
        {
            await AppHost!.StartAsync();

            var startupForm = AppHost.Services.GetRequiredService<MainWindow>();
            startupForm.Show();

            var application = AppHost.Services.GetRequiredService<Application>();
            application.StartAsync();

            base.OnStartup(e);
        }

        protected override async void OnExit(ExitEventArgs e)
        {
            await AppHost!.StopAsync();

            base.OnExit(e);
        }
    }
}
