using Client.Domain.AI.State;
using Client.Domain.Common;
using Client.Domain.Events;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.AI
{
    public interface AIInterface : ObservableObjectInterface
    {
        Task Update();

        void Toggle();

        bool IsEnabled { get; }

        TypeEnum Type { get; set; }
        BaseState.Type CurrentState { get; }
    }
}
