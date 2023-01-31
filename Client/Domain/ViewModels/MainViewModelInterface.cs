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
        void AddDrop(Drop drop);
        void RemoveDrop(Drop drop);
    }
}
