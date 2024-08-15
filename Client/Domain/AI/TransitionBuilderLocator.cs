using System.Collections.Generic;
using CombatTransitionBuilder = Client.Domain.AI.Combat.TransitionBuilder;
using DelevelingTransitionBuilder = Client.Domain.AI.Deleveling.TransitionBuilder;

namespace Client.Domain.AI
{
    public class TransitionBuilderLocator
    {
        public TransitionBuilderInterface Get(TypeEnum type)
        {
            if (!builders.ContainsKey(type))
            {
                switch (type)
                {
                    case TypeEnum.Combat:
                        builders.Add(type, new CombatTransitionBuilder());
                        break;
                    case TypeEnum.Deleveling:
                        builders.Add(type, new DelevelingTransitionBuilder());
                        break;
                }
            }

            return builders[type];            
        }

        private Dictionary<TypeEnum, TransitionBuilderInterface> builders = new Dictionary<TypeEnum, TransitionBuilderInterface>();
    }
}
