using Client.Domain.Entities;
using Client.Domain.Factories;
using Client.Domain.Helpers;
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
            entity.ExperienceInfo.ExpToLevel = experienceHelper.GetExperienceToLevel(entity.ExperienceInfo.Level + 1);
        }

        public override void OnUpdate(Hero entity)
        {
            if (entity.ExperienceInfo != null)
            {
                entity.ExperienceInfo.ExpToLevel = experienceHelper.GetExperienceToLevel(entity.ExperienceInfo.Level + 1);
            }
        }

        public override void OnDelete(Hero entity)
        {
            mainViewModel.DeleteHero();
        }

        public HeroHandler(EntityFactoryInterface<Hero> factory, MainViewModelInterface mainViewModel, ExperienceHelperInterface experienceHelper) : base(factory)
        {
            this.mainViewModel = mainViewModel;
            this.experienceHelper = experienceHelper;
        }

        private readonly MainViewModelInterface mainViewModel;
        private readonly ExperienceHelperInterface experienceHelper;
    }
}
