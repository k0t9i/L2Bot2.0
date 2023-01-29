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
    public class HeroHandler : EntityHandler<Hero>
    {
        public override void OnCreate(Hero entity)
        {
            mainViewModel.CreateHero(entity);
        }
        public override void OnDelete(Hero entity)
        {
            mainViewModel.DeleteHero();
        }

        public HeroHandler(EntityFactoryInterface<Hero> factory, MainViewModelInterface mainViewModel) : base(factory)
        {
            this.mainViewModel = mainViewModel;
        }

        private readonly MainViewModelInterface mainViewModel;
    }
}
