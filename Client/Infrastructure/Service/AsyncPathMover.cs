using Client.Application.ViewModels;
using Client.Domain.DTO;
using Client.Domain.Entities;
using Client.Domain.Enums;
using Client.Domain.Service;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace Client.Infrastructure.Service
{
    public class AsyncPathMover : AsyncPathMoverInterface
    {
        private readonly WorldHandler worldHandler;
        private readonly PathfinderInterface pathfinder;
        private readonly double nodeWaitingTime;
        private readonly int nodeDistanceTolerance;
        private readonly int nextNodeDistanceTolerance;
        private readonly ushort maxPassableHeight;
        private CancellationTokenSource? cancellationTokenSource;

        public PathfinderInterface Pathfinder => pathfinder;
        public ObservableCollection<PathSegment> Path { get; private set; } = new ObservableCollection<PathSegment>();
        public bool IsLocked { get; private set; } = false;

        public void Unlock()
        {
            IsLocked = false;
            if (cancellationTokenSource != null)
            {
                cancellationTokenSource.Cancel();
                cancellationTokenSource.Dispose();
                cancellationTokenSource = null;
                Path.Clear();
            }
        }

        public async Task<bool> MoveAsync(Vector3 location, ushort maxPassableHeight)
        {
            IsLocked = true;

            if (cancellationTokenSource != null)
            {
                cancellationTokenSource.Cancel();
                cancellationTokenSource.Dispose();
            }
            cancellationTokenSource = new CancellationTokenSource();
            var cancellationToken = cancellationTokenSource.Token;

            try
            {
                return await Task.Run(async () =>
                {
                    Debug.WriteLine("Find path started");
                    FindPath(location, maxPassableHeight);
                    Debug.WriteLine("Find path finished");


                    foreach (var node in Path.ToList())
                    {
                        worldHandler.RequestMoveToLocation(node.To);

                        var reached = await WaitForNodeReaching(cancellationToken, node);
                        if (!reached)
                        {
                            IsLocked = false;
                            return false;
                        }

                        Path.Remove(node);
                    }

                    IsLocked = false;
                    return true;
                }, cancellationToken);
            }
            catch (OperationCanceledException)
            {
                Debug.WriteLine("Path cancelled");
                IsLocked = false;
                return true;
            }
        }

        public async Task<bool> MoveAsync(Vector3 location)
        {
            return await MoveAsync(location, maxPassableHeight);
        }

        public AsyncPathMover(WorldHandler worldHandler, PathfinderInterface pathfinder, double nodeWaitingTime, int nodeDistanceTolerance, int nextNodeDistanceTolerance, ushort maxPassableHeight)
        {
            this.worldHandler = worldHandler;
            this.pathfinder = pathfinder;
            this.nodeWaitingTime = nodeWaitingTime;
            this.nodeDistanceTolerance = nodeDistanceTolerance;
            this.nextNodeDistanceTolerance = nextNodeDistanceTolerance;
            this.maxPassableHeight = maxPassableHeight;
        }

        private void FindPath(Vector3 location, ushort maxPassableHeight)
        {
            var hero = worldHandler.Hero;

            Path.Clear();
            if (hero == null)
            {
                return;
            }

            var path = pathfinder.FindPath(hero.Transform.Position, location, maxPassableHeight);
            foreach (var segment in path)
            {
                Path.Add(segment);
            }
        }

        private async Task<bool> WaitForNodeReaching(CancellationToken token, PathSegment node)
        {
            var hero = worldHandler.Hero;

            var start = DateTime.Now;
            while (!token.IsCancellationRequested && hero != null && !hero.Transform.Position.ApproximatelyEquals(node.To, nodeDistanceTolerance))
            {
                if (hero.Transform.Velocity.Equals(Vector3.Zero))
                {
                    var elapsedSeconds = (DateTime.Now - start).TotalSeconds;
                    if (hero.Transform.Position.ApproximatelyEquals(node.To, nextNodeDistanceTolerance))
                    {
                        break;
                    }
                    else if (elapsedSeconds >= nodeWaitingTime)
                    {
                        Path.Clear();
                        return false;
                    }
                }

                token.ThrowIfCancellationRequested();
                await Task.Delay(25, token);
            }

            return true;
        }
    }
}
