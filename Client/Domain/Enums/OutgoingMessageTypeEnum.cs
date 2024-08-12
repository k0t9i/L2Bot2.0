using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Enums
{
    public enum OutgoingMessageTypeEnum
    {
        Invalidate,
        Move,
        AcquireTarget,
        Attack,
        Pickup,
        UseSkill,
        UseItem,
        ToggleSoulshot,
        Sit,
        Stand,
        RestartPoint
    }
}
