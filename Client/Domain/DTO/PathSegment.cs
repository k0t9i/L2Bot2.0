using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.DTO
{
    public class PathSegment
    {
        public Vector3 From = new Vector3(0, 0, 0);
        public Vector3 To = new Vector3(0, 0, 0);
    }
}
