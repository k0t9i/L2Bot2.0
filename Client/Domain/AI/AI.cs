using Client.Domain.AI.State;
using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.Events;
using Client.Domain.Service;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Client.Domain.AI
{
    public class AI : ObservableObject, AIInterface
    {
        public AI(WorldHandler worldHandler, Config config, AsyncPathMoverInterface asyncPathMover, TransitionBuilderLocator locator)
        {
            this.worldHandler = worldHandler;
            this.config = config;
            this.asyncPathMover = asyncPathMover;
            this.locator = locator;
            states = StateBuilder.Build(this);
            ResetState();
        }

        public void Toggle()
        {
            IsEnabled = !IsEnabled;
            if (IsEnabled)
            {
                ResetState();
            }
        }

        public bool IsEnabled { get { return isEnabled; } private set { if (isEnabled != value) { isEnabled = value; OnPropertyChanged(); } } }

        public TypeEnum Type { get { return type; } set { if (type != value) { type = value; ResetState(); OnPropertyChanged(); } } }
        public BaseState.Type CurrentState { get { return currentState; } private set { if (currentState != value) { currentState = value; OnPropertyChanged(); } } }

        public async Task Update()
        {
            await Task.Delay((int) config.DelayBetweenTransitions);

            await Task.Run(() =>
            {
                if (IsEnabled && worldHandler.Hero != null)
                {
                    states[CurrentState].Execute();
                    foreach (var transition in locator.Get(Type).Build(worldHandler, config, asyncPathMover))
                    {
                        if (transition.fromStates.ContainsKey(BaseState.Type.Any) && transition.toState != CurrentState || transition.fromStates.ContainsKey(CurrentState))
                        {
                            if (transition.predicate(states[CurrentState]))
                            {
                                states[CurrentState].OnLeave();
                                CurrentState = transition.toState;
                                states[CurrentState].OnEnter();
                                break;
                            }
                        }
                    }
                }
                else
                {
                    ResetState();
                }
            });

        }

        public WorldHandler GetWorldHandler()
        {
            return worldHandler;
        }

        public Config GetConfig()
        {
            return config;
        }

        public AsyncPathMoverInterface GetAsyncPathMover()
        {
            return asyncPathMover;
        }

        private void ResetState()
        {
            CurrentState = BaseState.Type.Idle;
        }

        private readonly WorldHandler worldHandler;
        private readonly Config config;
        private readonly AsyncPathMoverInterface asyncPathMover;
        private readonly TransitionBuilderLocator locator;
        private BaseState.Type currentState;
        private Dictionary<BaseState.Type, BaseState> states = new Dictionary<BaseState.Type, BaseState>();
        private bool isEnabled = false;
        private TypeEnum type = TypeEnum.Combat;
    }
}
