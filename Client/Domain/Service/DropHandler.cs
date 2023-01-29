using Client.Domain.Entities;
using Client.Domain.Factories;
using Client.Domain.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Service
{
    public class PlayerHandler : EntityHandler<Player>
    {
        public override void OnCreate(Player entity)
        {
            mainViewModel.AddPlayer(entity);
        }
        public override void OnDelete(Player entity)
        {
            mainViewModel.RemovePlayer(entity);
        }

        public PlayerHandler(EntityFactoryInterface<Player> factory, MainViewModelInterface mainViewModel) : base(factory)
        {
            this.mainViewModel = mainViewModel;
        }

        private readonly MainViewModelInterface mainViewModel;
    }
}
