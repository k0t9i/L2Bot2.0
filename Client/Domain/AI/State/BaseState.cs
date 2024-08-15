using Client.Domain.Entities;
using Client.Domain.Service;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.AI.State
{
    public abstract class BaseState
    {
        public enum Type
        {
            Any,
            Attack,
            Dead,
            FindTarget,
            Idle,
            MoveToTarget,
            Pickup,
            Rest,
            MoveToSpot,
            AttackGuard,
            FindGuard
        }

        public BaseState(AI ai)
        {
            this.ai = ai;
        }

        public void Execute()
        {
            var hero = ai.GetWorldHandler().Hero;
            if (hero == null)
            {
                return;
            }

            DoExecute(ai.GetWorldHandler(), ai.GetConfig(), ai.GetAsyncPathMover(), hero);
        }

        public void OnEnter()
        {
            var hero = ai.GetWorldHandler().Hero;
            if (hero == null)
            {
                return;
            }

            DoOnEnter(ai.GetWorldHandler(), ai.GetConfig(), hero);
        }

        public void OnLeave()
        {
            var hero = ai.GetWorldHandler().Hero;
            if (hero == null)
            {
                return;
            }


            ai.GetAsyncPathMover().Unlock();
            DoOnLeave(ai.GetWorldHandler(), ai.GetConfig(), hero);
        }

        protected virtual void DoExecute(WorldHandler worldHandler, Config config, AsyncPathMoverInterface asyncPathMover, Hero hero)
        {

        }

        protected virtual void DoOnEnter(WorldHandler worldHandler, Config config, Hero hero)
        {

        }

        protected virtual void DoOnLeave(WorldHandler worldHandler, Config config, Hero hero)
        {

        }

        private readonly AI ai;
    }
}
