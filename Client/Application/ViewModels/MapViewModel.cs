using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.Service;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Animation;

namespace Client.Application.ViewModels
{
    public class MapViewModel : ObservableObject
    {
        public Hero? Hero
        {
            get => hero;
            set
            {
                if (hero != value)
                {
                    if (hero != null)
                    {
                        hero.Transform.Position.PropertyChanged -= HeroPosition_PropertyChanged;
                    }
                    hero = value;
                    if (hero != null)
                    {
                        hero.Transform.Position.PropertyChanged += HeroPosition_PropertyChanged;
                    }
                    UpdateMap();
                }
            }
        }

        private void HeroPosition_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            UpdateMap();
        }

        private void UpdateMap()
        {
            foreach (var block in Blocks)
            {
                block.Visible = false;
            }

            if (hero != null)
            {
                var blocks = selector.SelectImages((float)ViewportWidth, (float)ViewportHeight, hero.Transform.Position, Scale);

                foreach (var block in blocks)
                {
                    if (this.blocks.ContainsKey(block.Id))
                    {
                        this.blocks[block.Id].MapBlock.DeltaX = block.DeltaX;
                        this.blocks[block.Id].MapBlock.DeltaY = block.DeltaY;
                        this.blocks[block.Id].MapBlock.Size = block.Size;

                    }
                    else
                    {
                        var model = new MapBlockViewModel(block);
                        this.blocks.Add(block.Id, model);
                        Blocks.Add(model);
                    }

                    this.blocks[block.Id].Visible = true;
                }
            }
        }

        public ObservableCollection<MapBlockViewModel> Blocks { get; } = new ObservableCollection<MapBlockViewModel>();
        public double ViewportWidth
        {
            get => viewportWidth;
            set
            {
                if (viewportWidth != value)
                {
                    viewportWidth = value;
                    UpdateMap();
                }
            }
        }
        public double ViewportHeight
        {
            get => viewportHeight;
            set
            {
                if (viewportHeight != value)
                {
                    viewportHeight = value;
                    UpdateMap();
                }
            }
        }

        public float Scale
        {
            get => scale;
            set
            {
                if (scale != value)
                {
                    scale = value;
                    UpdateMap();
                    OnPropertyChanged();
                }
            }
        }

        MapImageSelector selector = new MapImageSelector();
        Dictionary<uint, MapBlockViewModel> blocks = new Dictionary<uint, MapBlockViewModel>();
        private Hero? hero;
        private float scale = 8;
        private double viewportWidth = 50;
        private double viewportHeight = 50;
    }
}
