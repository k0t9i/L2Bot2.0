using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Client.Domain.Service
{
    public class MapImageSelector
    {
        private static readonly uint BLOCK_SIZE = 32768;
        private static readonly uint DELTA_X = 20;
        private static readonly uint DELTA_Y = 18;

        public List<MapBlock> SelectImages(float viewportWidth, float viewportHeight, Vector3 heroPosition, float scale, int level)
        {
            var viewportCenter = new Tuple<float, float>(viewportWidth / 2, viewportHeight / 2);

            var topLeft = new Tuple<float, float>(
                heroPosition.X - viewportCenter.Item1 * scale,
                heroPosition.Y - viewportCenter.Item2 * scale
            );
            var bottomRight = new Tuple<float, float>(
                heroPosition.X + viewportCenter.Item1 * scale,
                heroPosition.Y + viewportCenter.Item2 * scale
            );

            uint top = (uint)(MathF.Floor(topLeft.Item2 / BLOCK_SIZE) + DELTA_Y);
            uint bottom = (uint)(MathF.Floor(bottomRight.Item2 / BLOCK_SIZE) + DELTA_Y);
            uint left = (uint)(MathF.Floor(topLeft.Item1 / BLOCK_SIZE) + DELTA_X);
            uint right = (uint)(MathF.Floor(bottomRight.Item1 / BLOCK_SIZE) + DELTA_X);

            List<MapBlock> result = new List<MapBlock>();
            for (uint x = left; x <= right; x++)
            {
                for (uint y = top; y <= bottom; y++)
                {
                    Tuple<int, int> blockTopLeft = new Tuple<int, int>(
                        (int)((x - DELTA_X) * BLOCK_SIZE),
                        (int)((y - DELTA_Y) * BLOCK_SIZE)
                    );
                    Tuple<float, float> delta = new Tuple<float, float>(
                        (blockTopLeft.Item1 - topLeft.Item1) / scale,
                        (blockTopLeft.Item2 - topLeft.Item2) / scale
                    );

                    result.Add(new MapBlock(x, y, delta.Item1, delta.Item2, BLOCK_SIZE / scale, level));
                }
            }

            return result;
        }
    }
}
