using Client.Domain.Helpers;
using Microsoft.Extensions.Configuration;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web;
using static Client.Infrastructure.Helpers.ConfigurationExperienceHelper;

namespace Client.Infrastructure.Helpers
{
    public class ConfigurationNpcInfoHelper : NpcInfoHelperInterface
    {
        public uint GetLevel(uint id)
        {
            return GetNpcInfo(id).level;
        }

        public uint GetAggroRadius(uint id)
        {
            return GetNpcInfo(id).aggroRadius;
        }

        public ConfigurationNpcInfoHelper(IConfiguration configuration)
        {
            this.configuration = configuration;
        }

        private NpcInfo GetNpcInfo(uint id)
        {
            if (!npcInfo.ContainsKey(id))
            {
                var item = configuration.GetRequiredSection("npcInfo").GetChildren()
                    .Where(x => x.Key == id.ToString())
                    .FirstOrDefault();
                uint level = 0;
                uint aggroRadius = 0;
                if (item != null)
                {
                    uint.TryParse(item.GetRequiredSection("level").Value, out level);
                    uint.TryParse(item.GetRequiredSection("aggroRadius").Value, out aggroRadius);
                }
                npcInfo[id] = new NpcInfo
                {
                    level = level,
                    aggroRadius = aggroRadius
                };
            }
            return npcInfo[id];
        }

        private readonly IConfiguration configuration;
        private Dictionary<uint, NpcInfo> npcInfo = new Dictionary<uint, NpcInfo>();

        private class NpcInfo
        {
            public uint level { get; set; }
            public uint aggroRadius { get; set; }
        }
    }
}
