﻿using Client.Domain.AI.State;
using Client.Domain.Entities;
using Client.Domain.Service;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.AI.Deleveling
{
    public class TransitionBuilder : TransitionBuilderInterface
    {
     
        // todo add MoveToDropState, SweepState
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
                    new(new List<BaseState.Type>{BaseState.Type.FindGuard}, BaseState.Type.MoveToTarget, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }
                        return worldHandler.Hero.Target != null;
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.MoveToTarget}, BaseState.Type.Idle, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }
                        return worldHandler.Hero.Target == null;
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.MoveToTarget}, BaseState.Type.AttackGuard, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }
                        if (worldHandler.Hero.Target == null)
                        {
                            return false;
                        }

                        var distance = worldHandler.Hero.Transform.Position.HorizontalDistance(worldHandler.Hero.Target.Transform.Position);
                        var expectedDistance = config.Deleveling.AttackDistance;
                        return distance < expectedDistance;
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.AttackGuard}, BaseState.Type.FindGuard, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }
                        if (worldHandler.Hero.Target == null)
                        {
                            return true;
                        }

                        var distance = worldHandler.Hero.Transform.Position.HorizontalDistance(worldHandler.Hero.Target.Transform.Position);
                        var expectedDistance = config.Deleveling.AttackDistance;
                        return distance >= expectedDistance;
                    }),
                    new(new List<BaseState.Type>{BaseState.Type.Idle}, BaseState.Type.FindGuard, (state) => {
                        if (worldHandler.Hero == null) {
                            return false;
                        }

                        return worldHandler.Hero.ExperienceInfo.Level > config.Deleveling.TargetLevel;
                    }),
                };
            }

            return transitions;
        }

        private List<TransitionBuilderInterface.Transition> transitions = new List<TransitionBuilderInterface.Transition>();
    }
}
