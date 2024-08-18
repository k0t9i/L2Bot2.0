using Client.Domain.DTO;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Service
{
    public interface AsyncPathMoverInterface
    {
        public PathfinderInterface Pathfinder { get; }
        public ObservableCollection<PathSegment> Path { get; }
        public Task<bool> MoveAsync(Vector3 location);
        public Task MoveUntilReachedAsync(Vector3 location);
        public bool IsLocked { get; }

        public void Unlock();
    }
}
