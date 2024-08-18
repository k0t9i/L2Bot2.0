using Client.Domain.DTO;
using Client.Domain.Service;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Client.Infrastructure.Service
{
    public class L2jGeoDataPathfinder : PathfinderInterface
    {
        [DllImport("L2JGeoDataPathFinder.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern uint FindPath(out IntPtr arrayPtr, string geoDataDirectory, float startX, float startY, float startZ, float endX, float endY, ushort maxPassableHeight);
        [DllImport("L2JGeoDataPathFinder.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern uint ReleasePath(IntPtr arrayPtr);

        [DllImport("L2JGeoDataPathFinder.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool HasLineOfSight(string geoDataDirectory, float startX, float startY, float startZ, float endX, float endY, ushort maxPassableHeight);

        public L2jGeoDataPathfinder(string geodataDirectory, ushort maxPassableHeight)
        {
            this.geodataDirectory = geodataDirectory;
            this.maxPassableHeight = maxPassableHeight;
        }

        public List<PathSegment> FindPath(Vector3 start, Vector3 end)
        {
            var arrayPtr = IntPtr.Zero;
            var size = FindPath(out arrayPtr, GetGeodataFullpath(), start.X, start.Y, start.Z, end.X, end.Y, maxPassableHeight);
            var originalArrayPtr = arrayPtr;

            var nodes = new List<PathNode>();
            if (size > 0)
            {
                var entrySize = Marshal.SizeOf(typeof(PathNode));
                for (var i = 0; i < size; i++)
                {
                    var node = Marshal.PtrToStructure(arrayPtr, typeof(PathNode));
                    if (node != null)
                    {
                        nodes.Add((PathNode)node);
                    }
                    arrayPtr = new IntPtr(arrayPtr.ToInt32() + entrySize);
                }
                ReleasePath(originalArrayPtr);
            }

            return BuildPath(nodes);
        }

        public bool HasLineOfSight(Vector3 start, Vector3 end)
        {
            return HasLineOfSight(GetGeodataFullpath(), start.X, start.Y, start.Z, end.X, end.Y, maxPassableHeight);
        }

        private List<PathSegment> BuildPath(List<PathNode> nodes)
        {
            var result = new List<PathSegment>();

            var points = new List<Vector3>();
            foreach (var node in nodes)
            {
                points.Add(NodeToVector(node));
            }

            for (var i = 0; i < points.Count - 1; i++)
            {
                var point = points[i];
                var nextPoint = points[i + 1];

                result.Add(new PathSegment
                {
                    From = point,
                    To = nextPoint
                });
            }

            return result;
        }

        private Vector3 NodeToVector(PathNode node)
        {
            var rnd = new Random();

            return new Vector3(
                rnd.Next((int)node.minX, (int)node.maxX + 1),
                rnd.Next((int)node.minY, (int)node.maxY + 1),
                node.height
            );
        }

        private string GetGeodataFullpath()
        {
            return System.IO.Directory.GetCurrentDirectory() + "/Assets/" + geodataDirectory + "/";
        }

        private readonly string geodataDirectory;
        private readonly ushort maxPassableHeight;
    }
}
