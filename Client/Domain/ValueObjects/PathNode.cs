using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.ValueObjects
{
    public struct PathNode
    {
        public readonly uint minX;
        public readonly uint minY;
        public readonly uint maxX;
        public readonly uint maxY;
        public readonly short height;
    }
}
