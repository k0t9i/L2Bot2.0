using Client.Domain.AI.Combat;
using Client.Domain.Entities;
using Client.Domain.Service;
using Client.Infrastructure.Service;

namespace Client.Domain.AI.State
{
    public class MoveToTargetState : BaseState
    {
        public MoveToTargetState(AI ai) : base(ai)
        {
        }

        protected override void DoExecute(WorldHandler worldHandler, Config config, AsyncPathMoverInterface asyncPathMover, Hero hero)
        {
            var target = hero.Target;
            if (target == null)
            {
                target = hero;
            }

            var distance = hero.Transform.Position.HorizontalDistance(target.Transform.Position);

            if (asyncPathMover.IsLocked)
            {
                return;
            }
            var hasLineOfSight = asyncPathMover.Pathfinder.HasLineOfSight(hero.Transform.Position, target.Transform.Position);
            if (distance >= Helper.GetAttackDistanceByConfig(worldHandler, config, hero, target) || !hasLineOfSight)
            {
                asyncPathMover.MoveAsync(target.Transform.Position);
            }
        }
    }
}
