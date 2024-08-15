using Client.Domain.Entities;
using Client.Domain.Service;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.AI.State
{
    public class FindGuardState : BaseState
    {
        public FindGuardState(AI ai) : base(ai)
        {
        }

        protected override void DoExecute(WorldHandler worldHandler, Config config, AsyncPathMoverInterface asyncPathMover, Hero hero)
        {
            var targetId = worldHandler.GetGuards().FirstOrDefault()?.Id;

            if (targetId != null)
            {
                worldHandler.RequestAcquireTarget((uint)targetId);
            }
        }
    }
}
