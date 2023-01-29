using Client.Application.ViewModels;
using Client.Domain.Entities;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.ViewModels
{
    public interface MainViewModelInterface
    {
        void AddChatMessage(ChatMessage chatMessage);
        void CreateHero(Hero hero);
        void DeleteHero();
        void AddNpc(NPC npc);
        void RemoveNpc(NPC npc);
        void AddPlayer(Player player);
        void RemovePlayer(Player player);
        void AddDrop(Drop drop);
        void RemoveDrop(Drop drop);
    }
}
