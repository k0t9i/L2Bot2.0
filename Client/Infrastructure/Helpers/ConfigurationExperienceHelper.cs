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
    public class ConfigurationExperienceHelper : ExperienceHelperInterface
    {
        public ulong GetExperienceToLevel(uint level)
        {
            if (experienceToLevel.Count == 0)
            {
                var items = configuration.GetSection("experience").Get<List<ExpInfo>>();
                if (items != null)
                {
                    foreach (var item in items)
                    {
                        experienceToLevel[item.level] = item.toLevel;
                    }
                }
            }
            if (experienceToLevel.TryGetValue(level, out ulong result))
            {
                return result;
            }
            return 0;
        }

        public ConfigurationExperienceHelper(IConfiguration configuration)
        {
            this.configuration = configuration;
        }

        private readonly IConfiguration configuration;
        private Dictionary<uint, ulong> experienceToLevel = new Dictionary<uint, ulong>();

        public class ExpInfo
        {
            public uint level { get; set; }
            public ulong toLevel { get; set; }

            /* public ExpInfo(uint level, ulong toLevel)
             {
                 this.level = level;
                 this.toLevel = toLevel;
             }*/
        }
    }
}
