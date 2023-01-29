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
using Client.Domain.ValueObjects;
using Microsoft.Extensions.Configuration;
using System.Reflection;
using Client.Application.Views;
using Client.Domain.ViewModels;
using Client.Application.ViewModels;
using Client.Domain.Helpers;
using Client.Infrastructure.Helpers;

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

            var application = AppHost.Services.GetRequiredService<Bot>();
            application.StartAsync();

            base.OnStartup(e);
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
                .AddJsonFile("experience.json", optional: false, reloadOnChange: true)
                .Build();

            services
                .AddSingleton(typeof(IConfiguration), config)
                .AddSingleton<MainWindow>()
                .AddSingleton(
                    typeof(Bot),
                    x => new Bot(
                            x.GetRequiredService<TransportInterface>(),
                            x.GetRequiredService<MessageParserInterface>(),
                            x.GetRequiredService<EntityHandlerFactoryInterface>(),
                            config.GetValue<string>("DLLName") ?? ""
                        )
                    )
                .AddSingleton(typeof(EntityHandlerFactoryInterface), typeof(EntityHandlerFactory))
                .AddSingleton(typeof(MessageParserInterface), typeof(JsonMessageParser))
                .AddSingleton(
                    typeof(TransportInterface),
                    x => new NamedPipeTransport(
                        config.GetValue<string>("ConnectionPipeName") ?? ""
                    )
                )
                .AddSingleton(typeof(ExperienceHelperInterface), typeof(ConfigurationExperienceHelper))

                .AddTransient(typeof(EntityFactoryInterface<Hero>), typeof(EntityFactory<Hero>))
                .AddTransient(typeof(EntityFactoryInterface<Drop>), typeof(EntityFactory<Drop>))
                .AddTransient(typeof(EntityFactoryInterface<NPC>), typeof(EntityFactory<NPC>))
                .AddTransient(typeof(EntityFactoryInterface<Player>), typeof(EntityFactory<Player>))
                .AddTransient(typeof(EntityFactoryInterface<ChatMessage>), typeof(EntityFactory<ChatMessage>))

                .AddSingleton<HeroHandler>()
                .AddSingleton<DropHandler>()
                .AddSingleton<NpcHandler>()
                .AddSingleton<PlayerHandler>()
                .AddSingleton<ChatMessageHandler>()
                
                .AddSingleton(typeof(MainViewModelInterface), typeof(MainViewModel));
        }
    }
}
