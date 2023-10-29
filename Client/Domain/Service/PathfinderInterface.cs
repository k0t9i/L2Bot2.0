using Client.Domain.DTO;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Service
{
    public interface PathfinderInterface
    {
        public List<PathSegment> FindPath(Vector3 start, Vector3 end);
    }
}
