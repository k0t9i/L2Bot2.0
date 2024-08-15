using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Common
{
    public static class ObservableCollectionExtensions
    {
        public static void RemoveAll<T>(this ObservableCollection<T> collection,
                                                           Func<T, bool>? condition = null)
        {
            for (int i = collection.Count - 1; i >= 0; i--)
            {
                if (condition == null || condition(collection[i]))
                {
                    collection.RemoveAt(i);
                }
            }
        }
    }
}
