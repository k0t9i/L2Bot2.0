using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Application.ViewModels
{
    public interface CreatureListViewModelInterface
    {
        uint Id { get;  }
        string Title { get;  }
        string Info { get; }
        float Distance { get; } 
        float DeltaZ { get; }
    }
}
