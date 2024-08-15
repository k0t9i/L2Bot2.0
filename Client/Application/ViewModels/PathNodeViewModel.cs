using Client.Application.Commands;
using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.Service;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Application.ViewModels
{
    public class PathNodeViewModel : ObservableObject
    {
        public void MapUpdated(float scale, float viewportWidth, float viewportHeight)
        {
            Scale = scale;
            VieportSize = new Vector3(viewportWidth, viewportHeight, 0);
        }

        public Vector3 From => new Vector3(
           (from.X - hero.Transform.Position.X) / scale + (VieportSize.X / 2),
           (from.Y - hero.Transform.Position.Y) / scale + (VieportSize.Y / 2),
           0
        );

        public Vector3 To => new Vector3(
           (to.X - hero.Transform.Position.X) / scale + (VieportSize.X / 2),
           (to.Y - hero.Transform.Position.Y) / scale + (VieportSize.Y / 2),
           0
        );

        public float Radius => MAX_RADIUS - (1 / MapViewModel.MIN_SCALE - 1 / scale) / (1 / MapViewModel.MIN_SCALE - 1 / MapViewModel.MAX_SCALE) * (MAX_RADIUS - MIN_RADIUS);

        public float Scale
        {
            get => scale;
            set
            {
                if (scale != value)
                {
                    scale = value;
                    OnPropertyChanged("From");
                    OnPropertyChanged("To");
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
                    OnPropertyChanged("From");
                    OnPropertyChanged("To");
                }
            }
        }

        public PathNodeViewModel(Vector3 from, Vector3 to, Hero hero)
        {
            this.from = from;
            this.to = to;
            this.hero = hero;
            hero.Transform.Position.PropertyChanged += HeroPosition_PropertyChanged;
        }

        private void HeroPosition_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            OnPropertyChanged("From");
            OnPropertyChanged("To");
        }

        private readonly Vector3 from;
        private readonly Vector3 to;
        private readonly Hero hero;
        private float scale = 1;
        private static readonly float MAX_RADIUS = 10;
        private static readonly float MIN_RADIUS = 2;
        private Vector3 vieportSize = new Vector3(0, 0, 0);
    }
}
