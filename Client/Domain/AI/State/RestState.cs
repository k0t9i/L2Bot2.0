using Client.Domain.Entities;
using Client.Domain.Service;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.AI.State
{
    public class RestState : BaseState
    {
        public RestState(AI ai) : base(ai)
        {
        }

        protected override void DoOnEnter(WorldHandler worldHandler, Config config, Hero hero)
        {
            worldHandler.RequestAcquireTarget(hero.Id);
        }

        protected override void DoExecute(WorldHandler worldHandler, Config config, AsyncPathMoverInterface asyncPathMover, Hero hero)
        {
            if (!hero.IsStanding)
            {
                return;
            }

            worldHandler.RequestSit();
        }

        protected override void DoOnLeave(WorldHandler worldHandler, Config config, Hero hero)
        {
            if (hero.IsStanding)
            {
                return;
            }

            worldHandler.RequestStand();
        }
    }
}
