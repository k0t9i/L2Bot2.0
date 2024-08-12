using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Enums
{
    public enum RestartPointTypeEnum: short
    {
        Village = 0,
        ClanHall,
        Castle,
        Siege,
        Festival,
        Jail = 27
    }
}
