using Client.Domain.Helpers;
using Microsoft.Extensions.Configuration;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web;

namespace Client.Infrastructure.Helpers
{
    public class ConfigurationItemInfoHelper : ItemInfoHelperInterface
    {

        public List<ItemInfo> GetAllItems()
        {
            LoadItems();
            return itemsInfo;
        }

        public ConfigurationItemInfoHelper(IConfiguration configuration)
        {
            this.configuration = configuration;
        }

        private void LoadItems()
        {
            if (itemsInfo.Count == 0)
            {
                var items = configuration.GetRequiredSection("itemInfo").GetChildren();
                foreach (var item in items)
                {
                    var id = uint.Parse(item.Key);

                    var isShot = false;
                    if (item != null)
                    {
                        bool.TryParse(item.GetSection("isShot").Value, out isShot);
                    }

                    itemsInfo.Add(new ItemInfo
                    {
                        Id = id,
                        Name = string.Format("{0} [{1}]", item?.GetRequiredSection("name").Value ?? "", id),
                        IsShot = isShot
                    });
                }
            }
        }

        private readonly IConfiguration configuration;
        private List<ItemInfo> itemsInfo = new List<ItemInfo>();
    }
}
