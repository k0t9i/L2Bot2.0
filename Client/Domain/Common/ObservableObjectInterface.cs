using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Common
{
    public interface ObservableObjectInterface : INotifyPropertyChanged
    {
        void OnPropertyChanged([CallerMemberName] string prop = "");
    }
}
