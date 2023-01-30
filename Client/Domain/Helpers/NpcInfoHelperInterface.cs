using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Helpers
{
    public interface NpcInfoHelperInterface
    {
        uint GetLevel(uint id);
        uint GetAggroRadius(uint id);
    }
}
