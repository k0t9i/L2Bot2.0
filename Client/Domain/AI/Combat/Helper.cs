using Client.Domain.Entities;
using Client.Domain.Service;
using Client.Domain.ValueObjects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.AI.Combat
{
    public static class Helper
    {
        public static Skill? GetSkillByConfig(WorldHandler worldHandler, Config config, Hero hero, CreatureInterface target)
        {
            var conditions = config.Combat.SkillConditions;
            var targetHp = target.VitalStats.HpPercent;
            var heroMp = hero.VitalStats.MpPercent;
            var heroHp = hero.VitalStats.HpPercent;

            foreach (var condition in conditions )
            {
                var skill = worldHandler.GetSkillById(condition.Id);
                if (skill != null)
                {
                    if (condition.MaxTargetPercentHp < targetHp || condition.MinPlayerPercentMp > heroMp || condition.MaxPlayerPercentHp < heroHp)
                    {
                        continue;
                    }
                    return skill;
                }
            }

            return null;
        }

        public static List<Drop> GetDropByConfig(WorldHandler worldHandler, Config config)
        {
            if (!config.Combat.PickupIfPossible)
            {
                return new List<Drop>();
            }

            var result = worldHandler.GetDropsSortedByDistanceToHero(config.Combat.PickupMaxDeltaZ)
                .Where(x => !config.Combat.ExcludedItemIdsToPickup.ContainsKey(x.ItemId));

            if (config.Combat.IncludedItemIdsToPickup.Count > 0)
            {
                result = result.Where(x => config.Combat.IncludedItemIdsToPickup.ContainsKey(x.ItemId));
            }

            return result.ToList();
        }

        public static List<NPC> GetMobsToAttackByConfig(WorldHandler worldHandler, Config config, Hero hero)
        {
            var result = worldHandler.GetAliveMobsSortedByDistanceToHero(config.Combat.MobsMaxDeltaZ)
                .Where(x => !config.Combat.ExcludedMobIds.ContainsKey(x.NpcId));

            result = result.Where(x => config.Combat.Zone.IsInside(x.Transform.Position));

            if (config.Combat.IncludedMobIds.Count > 0)
            {
                result = result.Where(x => config.Combat.IncludedMobIds.ContainsKey(x.NpcId));
            }

            if (config.Combat.MobLevelLowerLimit != null)
            {
                result = result.Where(x => (int) (hero.ExperienceInfo.Level - x.Level) <= config.Combat.MobLevelLowerLimit);
            }

            if (config.Combat.MobLevelUpperLimit != null)
            {
                result = result.Where(x => (int) (x.Level - hero.ExperienceInfo.Level) <= config.Combat.MobLevelUpperLimit);
            }

            return result.ToList();
        }

        public static bool IsOnSpot(WorldHandler worldHandler, Config config, Hero hero)
        {
            if (config.Combat.Zone == null)
            {
                return true;
            }

            var spot = new Vector3(config.Combat.Zone.Center.X, config.Combat.Zone.Center.Y, hero.Transform.Position.Z);
            return spot.Distance(hero.Transform.Position) <= 200;
        }

        public static uint GetAttackDistanceByConfig(WorldHandler worldHandler, Config config, Hero hero, CreatureInterface target)
        {
            Skill? skill = GetSkillByConfig(worldHandler, config, hero, target);

            var equippedWeapon = worldHandler.GetEquippedWeapon();
            var expectedDistance = equippedWeapon != null && equippedWeapon.WeaponType == Enums.WeaponTypeEnum.Bow
                ? config.Combat.AttackDistanceBow
                : config.Combat.AttackDistanceMili;

            if (skill != null)
            {
                expectedDistance = (uint)skill.Range;
            }

            return expectedDistance;
        }
    }
}
