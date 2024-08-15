using Client.Domain.AI.Combat;
using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Application.ViewModels
{
    public class AICombatZoneMapViewModel : ObservableObject
    {
        public void MapUpdated(float scale, float viewportWidth, float viewportHeight)
        {
            Scale = scale;
            VieportSize = new Vector3(viewportWidth, viewportHeight, 0);
        }

        public AICombatZoneMapViewModel(CombatZone combatZone, Hero hero)
        {
            this.combatZone = combatZone;
            this.hero = hero;

            hero.Transform.Position.PropertyChanged += HeroPosition_PropertyChanged;
            combatZone.PropertyChanged += CombatZone_PropertyChanged;
            combatZone.Center.PropertyChanged += CombatZoneCenter_PropertyChanged;
        }

        private void CombatZoneCenter_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Center");
        }

        private void CombatZone_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Radius")
            {
                OnPropertyChanged("Radius");
            }
        }

        public Vector3 Center => new Vector3(
            (combatZone.Center.X - hero.Transform.Position.X) / scale + (VieportSize.X / 2),
            (combatZone.Center.Y - hero.Transform.Position.Y) / scale + (VieportSize.Y / 2),
            0
        );

        public float Radius => combatZone.Radius / scale;

        public float Scale
        {
            get => scale;
            set
            {
                if (scale != value)
                {
                    scale = value;
                    OnPropertyChanged("Center");
                    OnPropertyChanged("Radius");
                }
            }
        }

        public Vector3 VieportSize
        {
            get => vieportSize;
            set
            {
                if (vieportSize != value)
                {
                    vieportSize = value;
                    OnPropertyChanged("Center");
                }
            }
        }

        private void HeroPosition_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("Center");
        }

        private readonly CombatZone combatZone;
        private readonly Hero hero;
        private float scale = 1;
        private Vector3 vieportSize = new Vector3(0, 0, 0);
    }
}
