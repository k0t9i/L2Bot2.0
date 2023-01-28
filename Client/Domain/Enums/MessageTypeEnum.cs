using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Client.Domain.Entities;

namespace Client.Domain.Enums
{
    public enum MessageTypeEnum
    {
        None,
        Hero,
        Drop,
        NPC,
        Player,
        Skill,
        Item,
        AbnormalEffect,
        Chat
    }
}
