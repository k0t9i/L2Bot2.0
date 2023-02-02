using Client.Domain.Common;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace Client.Application.ViewModels
{
    public class MapBlockViewModel : ObservableObject
    {
        public string ImageSource => "/Assets/maps/" + mapBlock.BlockX + "_" + mapBlock.BlockY + ".jpg";
        public float DeltaX => mapBlock.DeltaX;
        public float DeltaY => mapBlock.DeltaY;
        public float Size => mapBlock.Size;
        public bool Visible
        {
            get => visible;
            set
            {
                if (visible != value)
                {
                    visible = value;
                    OnPropertyChanged();
                }
            }
        }
        public MapBlock MapBlock => mapBlock;

        public MapBlockViewModel(MapBlock mapBlock)
        {
            this.mapBlock = mapBlock;

            mapBlock.PropertyChanged += MapBlock_PropertyChanged;
        }

        private void MapBlock_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "DeltaX")
            {
                OnPropertyChanged("DeltaX");
            }
            if (e.PropertyName == "DeltaY")
            {
                OnPropertyChanged("DeltaY");
            }
            if (e.PropertyName == "Size")
            {
                OnPropertyChanged("Size");
            }
        }

        private readonly MapBlock mapBlock;
        private bool visible = true;
    }
}
