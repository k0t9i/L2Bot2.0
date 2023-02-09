using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.Service;
using System.Windows.Input;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client.Application.Commands;

namespace Client.Application.ViewModels
{
    public class SkillListViewModel : ObservableObject
    {
        public uint Id => skill.Id;
        public string Name => skill.Name;
        public uint Level => skill.Level;
        public string Description=> skill.Description;
        public string IconName => "/Assets/icons/" + skill.IconName + ".png";
        public bool IsActive => skill.IsActive;
        public bool IsReadyToUse => skill.IsReadyToUse;
        public bool IsBusy => !skill.IsReadyToUse;
        public uint? Cost => skill.Cost == 0 ? null : skill.Cost;
        public int? Range => skill.Range <= 0 ? null : skill.Range;

        public ICommand MouseLeftClickCommand { get; }
        private void OnLeftMouseClick(object? obj)
        {
            worldHandler.RequestUseSkill(Id, false, false);
        }

        public SkillListViewModel(WorldHandler worldHandler, Skill skill)
        {
            this.worldHandler = worldHandler;
            this.skill = skill;

            skill.PropertyChanged += Skill_PropertyChanged;
            MouseLeftClickCommand = new RelayCommand(OnLeftMouseClick);
        }

        private void Skill_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Description")
            {
                OnPropertyChanged("Description");
            }
            if (e.PropertyName == "IsReadyToUse")
            {
                OnPropertyChanged("IsBusy");
                OnPropertyChanged("IsReadyToUse");
            }
            if (e.PropertyName == "Level")
            {
                OnPropertyChanged("Level");
            }
            if (e.PropertyName == "Cost")
            {
                OnPropertyChanged("Cost");
            }
            if (e.PropertyName == "Range")
            {
                OnPropertyChanged("Range");
            }
            if (e.PropertyName == "Name")
            {
                OnPropertyChanged("Name");
            }
            if (e.PropertyName == "IconName")
            {
                OnPropertyChanged("IconName");
            }
        }

        private readonly WorldHandler worldHandler;
        private readonly Skill skill;
    }
}
