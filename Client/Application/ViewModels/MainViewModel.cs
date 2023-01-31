using Client.Application.Extensions;
using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.Events;
using Client.Domain.ValueObjects;
using Client.Domain.ViewModels;
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
        NotifyPropertyChanged,
        MainViewModelInterface,
        EventHandlerInterface<HeroCreatedEvent>,
        EventHandlerInterface<HeroDeletedEvent>,
        EventHandlerInterface<CreatureCreatedEvent>,
        EventHandlerInterface<CreatureDeletedEvent>
    {

        public void AddChatMessage(ChatMessage chatMessage)
        {
            ChatMessages.Add(new ChatMessageViewModel(chatMessage));
        }

        public void AddDrop(Drop drop)
        {
            if (hero != null)
            {
                Drops.Add(new DropListViewModel(drop, hero));
            }
        }

        public void RemoveDrop(Drop drop)
        {
            Drops.RemoveAll(x => x.Id == drop.Id);
        }

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

        public ObservableCollection<ChatMessageViewModel> ChatMessages { get; } = new ObservableCollection<ChatMessageViewModel>();
        public ObservableCollection<CreatureListViewModel> Creatures { get; } = new ObservableCollection<CreatureListViewModel>();
        public ObservableCollection<DropListViewModel> Drops { get; } = new ObservableCollection<DropListViewModel>();
        public HeroSummaryInfoViewModel? Hero { get; private set; }
        public Hero? hero;
    }
}
