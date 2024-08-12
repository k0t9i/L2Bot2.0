using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.DTO
{
    public struct UseSkillParams
    {
        public uint skillId;
        public bool isForced;
        public bool isShiftPressed;
    }
}
