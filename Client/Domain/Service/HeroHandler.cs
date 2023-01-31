using Client.Domain.Entities;
using Client.Domain.Events;
using Client.Domain.Factories;
using Client.Domain.Helpers;
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
            entity.ExperienceInfo.ExpToLevel = experienceHelper.GetExperienceToLevel(entity.ExperienceInfo.Level + 1);
            entity.ExperienceInfo.ExpToPrevLevel = experienceHelper.GetExperienceToLevel(entity.ExperienceInfo.Level);
            eventBus.Publish(new HeroCreatedEvent(entity));
        }

        public override void OnUpdate(Hero entity)
        {
            if (entity.ExperienceInfo != null)
            {
                entity.ExperienceInfo.ExpToLevel = experienceHelper.GetExperienceToLevel(entity.ExperienceInfo.Level + 1);
                entity.ExperienceInfo.ExpToPrevLevel = experienceHelper.GetExperienceToLevel(entity.ExperienceInfo.Level);
            }
        }

        public override void OnDelete(Hero entity)
        {
            eventBus.Publish(new HeroDeletedEvent());
        }

        public HeroHandler(EntityFactoryInterface<Hero> factory, EventBusInterface eventBus, ExperienceHelperInterface experienceHelper) : base(factory)
        {
            this.eventBus = eventBus;
            this.experienceHelper = experienceHelper;
        }

        private readonly EventBusInterface eventBus;
        private readonly ExperienceHelperInterface experienceHelper;
    }
}
