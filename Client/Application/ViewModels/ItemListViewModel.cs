using Client.Application.Commands;
using Client.Application.Components;
using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.Service;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Client.Application.ViewModels
{
    public class ItemListViewModel : ObservableObject
    {
        public uint Id => item.Id;
        public string Name => item.Name;
        public string Description => item.FullDescription;
        public string IconName => "/Assets/icons/" + item.IconName + ".png";

        public ICommand MouseLeftClickCommand { get; }
        public ICommand MouseRightClickCommand { get; }
        private void OnLeftMouseClick(object? obj)
        {
            worldHandler.RequestUseItem(Id);
        }
        private void OnRightMouseClick(object? obj)
        {
            worldHandler.RequestToggleAutouseSoulshot(Id);
        }

        public ItemListViewModel(WorldHandler worldHandler, ItemInterface item)
        {
            this.worldHandler = worldHandler;
            this.item = item;

            //skill.PropertyChanged += Skill_PropertyChanged;
            MouseLeftClickCommand = new RelayCommand(OnLeftMouseClick);
            MouseRightClickCommand = new RelayCommand(OnRightMouseClick);
        }

        private readonly WorldHandler worldHandler;
        private readonly ItemInterface item;
    }
}
