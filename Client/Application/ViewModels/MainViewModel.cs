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
        EventHandlerInterface<NpcCreatedEvent>,
        EventHandlerInterface<NpcDeletedEvent>
    {

        public void AddChatMessage(ChatMessage chatMessage)
        {
            ChatMessages.Add(new ChatMessageViewModel(chatMessage));
        }

        public void AddPlayer(Player player)
        {
            if (hero != null)
            {
                Creatures.Add(new PlayerListViewModel(player, hero));
            }
        }

        public void RemovePlayer(Player player)
        {
            Creatures.RemoveAll(x => x.Id == player.Id);
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

        public void Handle(NpcCreatedEvent @event)
        {
            if (hero != null)
            {
                Creatures.Add(new NpcListViewModel(@event.NPC, hero));
            }
        }

        public void Handle(NpcDeletedEvent @event)
        {
            Creatures.RemoveAll(x => x.Id == @event.Id);
        }

        public ObservableCollection<ChatMessageViewModel> ChatMessages { get; } = new ObservableCollection<ChatMessageViewModel>();
        public ObservableCollection<CreatureListViewModelInterface> Creatures { get; } = new ObservableCollection<CreatureListViewModelInterface>();
        public ObservableCollection<DropListViewModel> Drops { get; } = new ObservableCollection<DropListViewModel>();
        public HeroSummaryInfoViewModel? Hero { get; private set; }
        public Hero? hero;
    }
}
