using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Entities
{
    public interface CreatureInterface : INotifyPropertyChanged
    {
        uint Id { get; set; }
        Transform Transform { get; set; }
        string Name { get; }
        string BriefInfo { get; }

    }
}
