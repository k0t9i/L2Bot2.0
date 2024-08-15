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
    public class ConfigurationSkillInfoHelper : SkillInfoHelperInterface
    {

        public Dictionary<uint, SkillInfo> GetAllSkills()
        {
            LoadSkills();
            return skillsInfo;
        }

        public ConfigurationSkillInfoHelper(IConfiguration configuration)
        {
            this.configuration = configuration;
        }

        private void LoadSkills()
        {
            if (skillsInfo.Count == 0)
            {
                var items = configuration.GetRequiredSection("skillInfo").GetChildren();
                foreach (var item in items)
                {
                    var id = uint.Parse(item.Key);

                    var isActive = false;
                    if (item != null)
                    {
                        bool.TryParse(item.GetRequiredSection("IsActive").Value, out isActive);
                    }
                    skillsInfo[id]=(new SkillInfo
                    {
                        Id = id,
                        Name = string.Format("{0} [{1}]", item?.GetRequiredSection("name").Value ?? "", id),
                        IsActive = isActive
                    });
                }
            }
        }

        private readonly IConfiguration configuration;
        private Dictionary<uint, SkillInfo> skillsInfo = new Dictionary<uint, SkillInfo>();
    }
}
