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
        private readonly int pathNumberOfAttempts;
        private readonly double nodeWaitingTime;
        private readonly int nodeDistanceTolerance;
        private readonly int nextNodeDistanceTolerance;
        private CancellationTokenSource? cancellationTokenSource;

        public ObservableCollection<PathSegment> Path { get; private set; } = new ObservableCollection<PathSegment>();
        public bool IsBusy { get; private set; } = false;

        public async Task MoveUntilReachedAsync(Vector3 location)
        {
            var remainingAttempts = pathNumberOfAttempts;
            while (!await MoveAsync(location) && remainingAttempts > 0)
            {
                remainingAttempts--;
            }
        }

        public async Task<bool> MoveAsync(Vector3 location)
        {
            IsBusy = true;

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
                    cancellationToken.ThrowIfCancellationRequested();

                    Debug.WriteLine("Find path start");
                    FindPath(location);
                    Debug.WriteLine("Find path finish");


                    foreach (var node in Path.ToList())
                    {
                        worldHandler.RequestMoveToLocation(node.To);

                        var reached = await WaitForNodeReaching(cancellationToken, node);
                        if (!reached)
                        {
                            IsBusy = false;
                            return false;
                        }
                        Path.Remove(node);
                    }

                    IsBusy = false;
                    return true;
                }, cancellationToken);
            }
            catch (OperationCanceledException)
            {
                IsBusy = false;
                return true;
            }
        }

        public AsyncPathMover(WorldHandler worldHandler, PathfinderInterface pathfinder, int pathNumberOfAttempts, double nodeWaitingTime, int nodeDistanceTolerance, int nextNodeDistanceTolerance)
        {
            this.worldHandler = worldHandler;
            this.pathfinder = pathfinder;
            this.pathNumberOfAttempts = pathNumberOfAttempts;
            this.nodeWaitingTime = nodeWaitingTime;
            this.nodeDistanceTolerance = nodeDistanceTolerance;
            this.nextNodeDistanceTolerance = nextNodeDistanceTolerance;
        }

        private void FindPath(Vector3 location)
        {
            var hero = worldHandler.Hero;

            Path.Clear();
            if (hero == null)
            {
                return;
            }

            var path = pathfinder.FindPath(hero.Transform.Position, location);
            foreach (var segment in path)
            {
                Path.Add(segment);
            }
        }

        private async Task<bool> WaitForNodeReaching(CancellationToken token, PathSegment node)
        {
            var hero = worldHandler.Hero;

            var start = DateTime.Now;
            while (hero != null && !hero.Transform.Position.ApproximatelyEquals(node.To, nodeDistanceTolerance))
            {
                if (token.IsCancellationRequested)
                {
                    token.ThrowIfCancellationRequested();
                }

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
                await Task.Delay(25);
            }

            return true;
        }
    }
}
