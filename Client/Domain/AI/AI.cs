using Client.Domain.AI.State;
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
    public class AI : AIInterface
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
            isEnabled = !isEnabled;
            if (isEnabled)
            {
                ResetState();
            }
        }

        public bool IsEnabled => isEnabled;

        public TypeEnum Type { get { return type; } set { if (type != value) { type = value; ResetState(); } } }

        public async Task Update()
        {
            await Task.Delay((int) config.DelayBetweenTransitions);

            await Task.Run(() =>
            {
                if (isEnabled && worldHandler.Hero != null)
                {
                    states[currentState].Execute();
                    foreach (var transition in locator.Get(Type).Build())
                    {
                        if (transition.fromStates.ContainsKey(BaseState.Type.Any) && transition.toState != currentState || transition.fromStates.ContainsKey(currentState))
                        {
                            if (transition.predicate(worldHandler, config, states[currentState]))
                            {
                                states[currentState].OnLeave();
                                currentState = transition.toState;
                                Debug.WriteLine(currentState.ToString());
                                states[currentState].OnEnter();
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
            currentState = BaseState.Type.Idle;
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
