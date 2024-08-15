using Client.Domain.Entities;
using Client.Domain.Service;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.AI.State
{
    public class DeadState : BaseState
    {
        public DeadState(AI ai) : base(ai)
        {
        }

        protected override void DoOnEnter(WorldHandler worldHandler, Config config, Hero hero)
        {
            worldHandler.RequestRestartPoint(Enums.RestartPointTypeEnum.Village);
        }
    }
}
