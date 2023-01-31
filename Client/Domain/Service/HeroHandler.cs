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
    public class HeroHandler : EntityHandler<Hero>, EventHandlerInterface<TargetChangedEvent>
    {
        public override void OnCreate(Hero entity)
        {
            entity.ExperienceInfo.ExpToLevel = experienceHelper.GetExperienceToLevel(entity.ExperienceInfo.Level + 1);
            entity.ExperienceInfo.ExpToPrevLevel = experienceHelper.GetExperienceToLevel(entity.ExperienceInfo.Level);
            entity.PropertyChanged += Hero_PropertyChanged;
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

        private void Hero_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (sender == null)
            {
                return;
            }

            var hero = (Hero)sender;
            if (e.PropertyName == "TargetId")
            {
                hero.Target = null;
                eventBus.Publish(new TargetChangedEvent(hero));
            }
        }

        public void Handle(TargetChangedEvent @event)
        {
            var target = GetEntity(@event.Hero.TargetId);
            if (target == null)
            {
                return;
            }
            @event.Hero.Target = target;
        }

        private readonly EventBusInterface eventBus;
        private readonly ExperienceHelperInterface experienceHelper;
    }
}
