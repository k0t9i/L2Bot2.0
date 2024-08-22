using Client.Domain.AI.Combat;
using Client.Domain.Entities;
using Client.Domain.Service;
using Client.Domain.ValueObjects;
using System;

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

            var distanceToPrevPosition = targetPosition != null ? targetPosition.HorizontalDistance(target.Transform.Position) : 0;

            var routeNeedsToBeAdjusted = MathF.Abs(distanceToPrevPosition) > config.Combat.AttackDistanceMili;
            if (routeNeedsToBeAdjusted)
            {
                asyncPathMover.Unlock();
            }

            if (asyncPathMover.IsLocked)
            {
                return;
            }

            var distance = hero.Transform.Position.HorizontalDistance(target.Transform.Position);
            if (routeNeedsToBeAdjusted || distance >= Helper.GetAttackDistanceByConfig(worldHandler, config, hero, target) || !asyncPathMover.Pathfinder.HasLineOfSight(hero.Transform.Position, target.Transform.Position))
            {
                targetPosition = target.Transform.Position.Clone() as Vector3;
                asyncPathMover.MoveAsync(target.Transform.Position, config.Combat.MaxPassableHeight);
            }
        }

        protected override void DoOnLeave(WorldHandler worldHandler, Config config, Hero hero)
        {
            targetPosition = null;
        }

        private Vector3? targetPosition = null;
    }
}
