using Client.Application.Components;
using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.Events;
using Client.Domain.Service;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace Client.Application.ViewModels
{
    public class MainViewModel :
        ObservableObject,
        EventHandlerInterface<HeroCreatedEvent>,
        EventHandlerInterface<HeroDeletedEvent>,
        EventHandlerInterface<CreatureCreatedEvent>,
        EventHandlerInterface<CreatureDeletedEvent>,
        EventHandlerInterface<DropCreatedEvent>,
        EventHandlerInterface<DropDeletedEvent>,
        EventHandlerInterface<ChatMessageCreatedEvent>,
        EventHandlerInterface<SkillCreatedEvent>,
        EventHandlerInterface<SkillDeletedEvent>,
        EventHandlerInterface<ItemCreatedEvent>,
        EventHandlerInterface<ItemDeletedEvent>
    {
        public void Handle(HeroCreatedEvent @event)
        {
            Hero = new HeroSummaryInfoViewModel(@event.Hero);
            hero = @event.Hero;
            Map.Hero = hero;
            AddCreature(hero);
            OnPropertyChanged("Hero");
            OnPropertyChanged("Map");
        }

        public void Handle(HeroDeletedEvent @event)
        {
            if (hero != null)
            {
                RemoveCreature(hero.Id);
            }
            Hero = null;
            hero = null;
            Map.Hero = null;
            OnPropertyChanged("Hero");
            OnPropertyChanged("Map");
        }

        public void Handle(CreatureCreatedEvent @event)
        {
            if (hero != null)
            {
                Creatures.Add(new CreatureListViewModel(@event.Creature, hero, worldHandler));
                AddCreature(@event.Creature);
            }
        }

        public void Handle(CreatureDeletedEvent @event)
        {
            Creatures.RemoveAll(x => x.Id == @event.Id);
            RemoveCreature(@event.Id);
        }

        public void Handle(DropCreatedEvent @event)
        {
            if (hero != null)
            {
                Drops.Add(new DropListViewModel(@event.Drop, hero, worldHandler));
                Map.Drops.Add(new DropMapViewModel(@event.Drop, hero, worldHandler));
            }
        }

        public void Handle(DropDeletedEvent @event)
        {
            Drops.RemoveAll(x => x.Id == @event.Id);
            Map.Drops.RemoveAll(x => x.Id == @event.Id);
        }

        public void Handle(ChatMessageCreatedEvent @event)
        {
            ChatMessages.Add(new ChatMessageViewModel(@event.Message));
        }

        public void Handle(SkillCreatedEvent @event)
        {
            if (hero != null)
            {
                if (@event.Skill.IsActive)
                {
                    ActiveSkills.Add(new SkillListViewModel(worldHandler, @event.Skill));
                }
                else
                {
                    PassiveSkills.Add(new SkillListViewModel(worldHandler, @event.Skill));
                }
            }
        }

        public void Handle(SkillDeletedEvent @event)
        {
            ActiveSkills.RemoveAll(x => x.Id == @event.Id);
            PassiveSkills.RemoveAll(x => x.Id == @event.Id);
        }

        public void Handle(ItemCreatedEvent @event)
        {
            if (hero != null)
            {
                Items.Add(@event.Item);
            }
        }

        public void Handle(ItemDeletedEvent @event)
        {
            Items.RemoveAll(x => x.Id == @event.Id);
        }

        private void AddCreature(CreatureInterface creature)
        {
            if (hero != null)
            {
                Map.Creatures.Add(new CreatureMapViewModel(creature, hero, worldHandler));
            }
        }

        private void RemoveCreature(uint id)
        {
            Map.Creatures.RemoveAll(x => x.Id == id);
        }

        public MainViewModel(WorldHandler worldHandler)
        {
            this.worldHandler = worldHandler;
            Map = new MapViewModel(worldHandler);
        }

        public ObservableCollection<ChatMessageViewModel> ChatMessages { get; } = new ObservableCollection<ChatMessageViewModel>();
        public ObservableCollection<CreatureListViewModel> Creatures { get; } = new ObservableCollection<CreatureListViewModel>();
        public ObservableCollection<DropListViewModel> Drops { get; } = new ObservableCollection<DropListViewModel>();
        public ObservableCollection<SkillListViewModel> ActiveSkills { get; } = new ObservableCollection<SkillListViewModel>();
        public ObservableCollection<SkillListViewModel> PassiveSkills { get; } = new ObservableCollection<SkillListViewModel>();
        public ObservableCollection<BaseItem> Items { get; } = new ObservableCollection<BaseItem>();
        public HeroSummaryInfoViewModel? Hero { get; private set; }
        public MapViewModel Map { get; private set; }
        public Hero? hero;
        private readonly WorldHandler worldHandler;
    }
}
