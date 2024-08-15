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
    public class ConfigurationNpcInfoHelper : NpcInfoHelperInterface
    {
        public uint GetLevel(uint id)
        {
            LoadNpc();

            if (npcInfo.ContainsKey(id))
            {
                return npcInfo[id].Level;
            }
            return 0;
        }

        public uint GetAggroRadius(uint id)
        {
            LoadNpc();

            if (npcInfo.ContainsKey(id))
            {
                return npcInfo[id].AggroRadius;
            }
            return 0;
        }

        public List<NpcInfo> GetAllNpc()
        {
            LoadNpc();
            return npcInfo.Select(x => x.Value).ToList();
        }

        public ConfigurationNpcInfoHelper(IConfiguration configuration)
        {
            this.configuration = configuration;
        }

        private void LoadNpc()
        {
            if (npcInfo.Count == 0)
            {
                var items = configuration.GetRequiredSection("npcInfo").GetChildren();
                foreach (var item in items)
                {
                    var id = uint.Parse(item.Key);

                    uint level = 0;
                    uint aggroRadius = 0;
                    bool isGuard = false;
                    if (item != null)
                    {
                        uint.TryParse(item.GetRequiredSection("level").Value, out level);
                        uint.TryParse(item.GetRequiredSection("aggroRadius").Value, out aggroRadius);
                        bool.TryParse(item.GetRequiredSection("isGuard").Value, out isGuard);
                    }
                    npcInfo[id] = new NpcInfo
                    {
                        Id = id,
                        Level = level,
                        AggroRadius = aggroRadius,
                        Name = string.Format("{0} [{1}]", item?.GetRequiredSection("name").Value ?? "", id),
                        IsGuard = isGuard
                    };
                }
            }
        }

        private readonly IConfiguration configuration;
        private Dictionary<uint, NpcInfo> npcInfo = new Dictionary<uint, NpcInfo>();
    }
}
