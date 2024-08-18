using Client.Domain.AI.State;
using Client.Domain.Entities;
using Client.Domain.Service;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.AI.Combat
{
    public class TransitionBuilder : TransitionBuilderInterface
    {
        public List<TransitionBuilderInterface.Transition> Build(WorldHandler worldHandler, Config config, AsyncPathMoverInterface pathMover)
        {
            if (transitions.Count == 0)
            {
                transitions = new List<TransitionBuilderInterface.Transition>()
                {
                    new(new List<BaseState.Type>{BaseState.Type.Any}, BaseState.Type.Dead, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }
                        return worldHandler.Hero.VitalStats.IsDead;
                    }),
                     new(new List<BaseState.Type>{BaseState.Type.Dead}, BaseState.Type.Idle, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }
                        return !worldHandler.Hero.VitalStats.IsDead;
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.Idle, BaseState.Type.MoveToTarget, BaseState.Type.Rest, BaseState.Type.MoveToSpot}, BaseState.Type.FindTarget, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }

                        // TODO если нет цели, а тебя атаковали, то моб берется автоматом в таргет, из-за этого баг в Rest и MoveToSpot
                        // а без этой проверки зацикливается MoveToTarget->FindTarget->MoveToTarget
                        // один из вариантов решения, брать себя в таргет при входе в Rest и MoveToSpot 
                        if (worldHandler.Hero.Target != null && (worldHandler.Hero.AttackerIds.Contains(worldHandler.Hero.Target.Id) || worldHandler.Hero.Target.VitalStats.IsDead))
                        {
                            return false;
                        }

                        return worldHandler.Hero.AttackerIds.Count > 0;
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.FindTarget}, BaseState.Type.MoveToTarget, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }
                        return worldHandler.Hero.HasValidTarget;
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.FindTarget}, BaseState.Type.MoveToSpot, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }

                        return Helper.GetMobsToAttackByConfig(worldHandler, config, worldHandler.Hero).Count == 0
                            && !Helper.IsOnSpot(worldHandler, config, worldHandler.Hero);
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.MoveToSpot}, BaseState.Type.Idle, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }
                        if (Helper.GetMobsToAttackByConfig(worldHandler, config, worldHandler.Hero).Count > 0)
                        {
                            return true;
                        }

                        return Helper.IsOnSpot(worldHandler, config, worldHandler.Hero);
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.MoveToTarget}, BaseState.Type.Idle, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }
                        return !worldHandler.Hero.HasValidTarget;
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.Idle}, BaseState.Type.Rest, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        };
                        return worldHandler.Hero.AttackerIds.Count == 0 && (worldHandler.Hero.VitalStats.HpPercent < config.Combat.RestStartPercentHp
                            || worldHandler.Hero.VitalStats.MpPercent < config.Combat.RestStartPecentMp);
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.Rest}, BaseState.Type.Idle, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }
                        return worldHandler.Hero.VitalStats.HpPercent >= config.Combat.RestEndPecentHp
                            && worldHandler.Hero.VitalStats.MpPercent >= config.Combat.RestEndPecentMp;
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.MoveToTarget}, BaseState.Type.Attack, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }
                        if (worldHandler.Hero.Target == null)
                        {
                            return false;
                        }

                        if (!pathMover.Pathfinder.HasLineOfSight(worldHandler.Hero.Transform.Position, worldHandler.Hero.Target.Transform.Position))
                        {
                            return false;
                        }

                        if (config.Combat.SpoilIsPriority) {
                            var spoil = worldHandler.GetSkillById(config.Combat.SpoilSkillId);
                            if (spoil != null && !spoil.IsReadyToUse) {
                                return false;
                            }
                        }

                        var distance = worldHandler.Hero.Transform.Position.HorizontalDistance(worldHandler.Hero.Target.Transform.Position);
                        return distance < Helper.GetAttackDistanceByConfig(worldHandler, config, worldHandler.Hero, worldHandler.Hero.Target);
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.Attack}, BaseState.Type.Pickup, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }

                        return !worldHandler.Hero.HasValidTarget;
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.Attack}, BaseState.Type.FindTarget, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }

                        return worldHandler.Hero.HasValidTarget && worldHandler.Hero.AttackerIds.Count > 0 && !worldHandler.Hero.AttackerIds.Contains(worldHandler.Hero.TargetId);
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.Pickup}, BaseState.Type.Idle, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }
                        var currentState = (PickupState) state;

                        if (worldHandler.GetSkillById(config.Combat.SweeperSkillId) != null && currentState.IsSweeperMustBeUsed(worldHandler, config)) {
                            return false;
                        }

                        return currentState.GetDrops(worldHandler, config).Count == 0;
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.Idle}, BaseState.Type.FindTarget),
                };
            }

            return transitions;
        }

        private List<TransitionBuilderInterface.Transition> transitions = new List<TransitionBuilderInterface.Transition>();
    }
}
