using Client.Domain.AI.Combat;
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
    public class FindTargetState : BaseState
    {
        public FindTargetState(AI ai) : base(ai)
        {
        }

        protected override void DoExecute(WorldHandler worldHandler, Config config, AsyncPathMoverInterface asyncPathMover, Hero hero)
        {
            uint? targetId = hero.AttackerIds.Count > 0 ? hero.AttackerIds.First() : null;

            if (targetId == null)
            {
                targetId = Helper.GetMobsToAttackByConfig(worldHandler, config, hero).FirstOrDefault()?.Id;
            }

            if (targetId != null)
            {
                worldHandler.RequestAcquireTarget((uint)targetId);
            }
        }
    }
}
