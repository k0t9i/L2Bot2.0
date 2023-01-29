using Client.Domain.Common;
using Client.Domain.Entities;
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

namespace Client.Application.ViewModels
{
    public class MainViewModel : NotifyPropertyChanged, MainViewModelInterface
    {

        public void AddChatMessage(ChatMessage chatMessage)
        {
            ChatMessages.Add(new ChatMessageViewModel(chatMessage));
        }

        public void CreateHero(Hero hero)
        {
            Hero = new HeroSummaryInfoViewModel(hero);
            this.hero = hero;
            OnPropertyChanged("Hero");
        }
        public void DeleteHero()
        {
            Hero = null;
            hero = null;
            OnPropertyChanged("Hero");
        }

        public void AddNpc(NPC npc)
        {
            if (hero != null)
            {
                Creatures.Add(new NpcListViewModel(npc, hero));
            }
        }

        public void RemoveNpc(NPC npc)
        {
            foreach (var item in Creatures)
            {
                if (item.Id == npc.Id)
                {
                    Creatures.Remove(item);
                }
            }
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
            foreach (var item in Creatures)
            {
                if (item.Id == player.Id)
                {
                    Creatures.Remove(item);
                }
            }
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
            foreach (var item in Drops)
            {
                if (item.Id == drop.Id)
                {
                    Drops.Remove(item);
                }
            }
        }

        public ObservableCollection<ChatMessageViewModel> ChatMessages { get; } = new ObservableCollection<ChatMessageViewModel>();
        public ObservableCollection<CreatureListViewModelInterface> Creatures { get; } = new ObservableCollection<CreatureListViewModelInterface>();
        public ObservableCollection<DropListViewModel> Drops { get; } = new ObservableCollection<DropListViewModel>();
        public HeroSummaryInfoViewModel? Hero { get; private set; }
        public Hero? hero;
    }
}
