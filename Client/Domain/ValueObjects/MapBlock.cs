using Client.Domain.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.ValueObjects
{
    public class MapBlock : ObservableObject
    {
        private float deltaX;
        private float deltaY;
        private float size;

        public int Id => (IdWithOutLevel + Level) * (IdWithOutLevel + Level + 1) / 2 + IdWithOutLevel;
        public uint BlockX { get; set; }
        public uint BlockY { get; set; }
        public float DeltaX { get => deltaX; set { if (value != deltaX) { deltaX = value; OnPropertyChanged(); } } }
        public float DeltaY { get => deltaY; set { if (value != deltaY) { deltaY = value; OnPropertyChanged(); } } }
        public float Size { get => size; set { if (value != size) { size = value; OnPropertyChanged(); } } }
        public int Level { get; set; }

        private int IdWithOutLevel => (int) ((BlockX + BlockY) * (BlockX + BlockY + 1) / 2 + BlockX);

        public MapBlock(uint blockX, uint blockY, float deltaX, float deltaY, float size, int level)
        {
            BlockX = blockX;
            BlockY = blockY;
            DeltaX = deltaX;
            DeltaY = deltaY;
            Size = size;
            Level = level;
        }
    }
}
