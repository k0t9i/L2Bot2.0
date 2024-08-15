using Client.Domain.AI.Combat;
using Client.Domain.Entities;
using Client.Domain.Service;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Client.Domain.AI.State
{
    public class AttackState : BaseState
    {
        public AttackState(AI ai) : base(ai)
        {
        }

        protected override void DoExecute(WorldHandler worldHandler, Config config, AsyncPathMoverInterface asyncPathMover, Hero hero)
        {
            if (hero.Target == null)
            {
                return;
            }

            if (!config.Combat.UseOnlySkills)
            {
                worldHandler.RequestAttackOrFollow(hero.Target.Id);
            }

            if (config.Combat.SpoilIfPossible)
            {
                NPC? npc = hero.Target as NPC;
                var spoil = worldHandler.GetSkillById(config.Combat.SpoilSkillId);
                if (spoil != null && npc != null && npc.SpoilState == Enums.SpoilStateEnum.None)
                {
                    var excluded = config.Combat.ExcludedSpoilMobIds;
                    var included = config.Combat.IncludedSpoilMobIds;
                    if (!excluded.ContainsKey(npc.NpcId) && (included.Count == 0 || included.ContainsKey(npc.NpcId)))
                    {
                        if (spoil.IsReadyToUse && hero.VitalStats.Mp >= spoil.Cost)
                        {
                            worldHandler.RequestUseSkill(spoil.Id, false, false);
                        }
                    }
                }
            }

            var skill = Helper.GetSkillByConfig(worldHandler, config, hero, hero.Target);
            if (skill != null)
            {
                worldHandler.RequestUseSkill(skill.Id, false, false);
            }
        }

        protected override void DoOnEnter(WorldHandler worldHandler, Config config, Hero hero)
        {
            if (config.Combat.AutoUseShots)
            {
                // todo use only appropriate grade
                foreach (var item in worldHandler.GetShotItems())
                {
                    if (!item.IsAutoused)
                    {
                        worldHandler.RequestToggleAutouseSoulshot(item.Id);
                    }
                }
            }
        }
    }
}
