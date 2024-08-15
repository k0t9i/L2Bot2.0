using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Helpers
{
    public class NpcInfo : ObjectInfo
    {
        public uint Level { get; internal set; }
        public uint AggroRadius { get; internal set; }
        public bool IsGuard { get; internal set; }
    }
}
