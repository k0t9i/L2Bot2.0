using Client.Domain.AI.State;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.AI
{
    public static class StateBuilder
    {
        public static Dictionary<BaseState.Type, BaseState> Build(AI ai)
        {
            return new Dictionary<BaseState.Type, BaseState>
            {
                { BaseState.Type.Any, new AnyState(ai) },
                { BaseState.Type.Attack, new AttackState(ai) },
                { BaseState.Type.Dead, new DeadState(ai) },
                { BaseState.Type.FindTarget, new FindTargetState(ai) },
                { BaseState.Type.Idle, new IdleState(ai) },
                { BaseState.Type.MoveToTarget, new MoveToTargetState(ai) },
                { BaseState.Type.Pickup, new PickupState(ai) },
                { BaseState.Type.Rest, new RestState(ai) },
                { BaseState.Type.MoveToSpot, new MoveToSpotState(ai) },
                { BaseState.Type.AttackGuard, new AttackGuardState(ai) },
                { BaseState.Type.FindGuard, new FindGuardState(ai) }
            };
        }
    }
}
