using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.Events;
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
        EventHandlerInterface<ChatMessageCreatedEvent>
    {
        public void Handle(HeroCreatedEvent @event)
        {
            Hero = new HeroSummaryInfoViewModel(@event.Hero);
            hero = @event.Hero;
            OnPropertyChanged("Hero");
        }

        public void Handle(HeroDeletedEvent @event)
        {
            Hero = null;
            hero = null;
            OnPropertyChanged("Hero");
        }

        public void Handle(CreatureCreatedEvent @event)
        {
            if (hero != null)
            {
                Creatures.Add(new CreatureListViewModel(@event.Creature, hero));
            }
        }

        public void Handle(CreatureDeletedEvent @event)
        {
            Creatures.RemoveAll(x => x.Id == @event.Id);
        }

        public void Handle(DropCreatedEvent @event)
        {
            if (hero != null)
            {
                Drops.Add(new DropListViewModel(@event.Drop, hero));
            }
        }

        public void Handle(DropDeletedEvent @event)
        {
            Drops.RemoveAll(x => x.Id == @event.Id);
        }

        public void Handle(ChatMessageCreatedEvent @event)
        {
            ChatMessages.Add(new ChatMessageViewModel(@event.Message));
        }

        public ObservableCollection<ChatMessageViewModel> ChatMessages { get; } = new ObservableCollection<ChatMessageViewModel>();
        public ObservableCollection<CreatureListViewModel> Creatures { get; } = new ObservableCollection<CreatureListViewModel>();
        public ObservableCollection<DropListViewModel> Drops { get; } = new ObservableCollection<DropListViewModel>();
        public HeroSummaryInfoViewModel? Hero { get; private set; }
        public Hero? hero;
    }
}
