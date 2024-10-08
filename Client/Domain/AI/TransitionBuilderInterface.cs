﻿using Client.Domain.AI.State;
using Client.Domain.Service;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.AI
{
    public interface TransitionBuilderInterface
    {
        public struct Transition
        {
            public readonly Dictionary<BaseState.Type, BaseState.Type> fromStates;
            public readonly BaseState.Type toState;
            public readonly Func<BaseState, bool> predicate;

            public Transition(List<BaseState.Type> fromStates, BaseState.Type toState, Func<BaseState, bool>? predicate = null)
            {
                this.fromStates = fromStates.ToDictionary(x => x, x => x);
                this.toState = toState;
                this.predicate = predicate != null ? predicate : (state) => { return true; };
            }
        }

        List<Transition> Build(WorldHandler worldHandler, Config config, AsyncPathMoverInterface pathMover);
    }
}
