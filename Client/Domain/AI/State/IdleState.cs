using Client.Domain.Entities;
using Client.Domain.Service;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.AI.State
{
    public class IdleState : BaseState
    {
        public IdleState(AI ai) : base(ai)
        {
        }
    }
}
