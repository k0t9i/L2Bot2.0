using Client.Domain.AI.Combat;
using Client.Domain.ValueObjects;
using System.Collections.Generic;

namespace Client.Domain.AI
{
    public class Config
    {
        public struct SkillCondition
        {
            public uint Id { get; set; }
            public byte MaxTargetPercentHp { get; set; }
            public byte MinPlayerPercentMp { get; set; }
            public byte MaxPlayerPercentHp { get; set; }
        }

        public class CombatSection
        {
            public uint MobsMaxDeltaZ { get; set; } = 500;
            public Dictionary<uint, bool> ExcludedMobIds { get; set; } = new Dictionary<uint, bool>();
            public Dictionary<uint, bool> IncludedMobIds { get; set; } = new Dictionary<uint, bool>();
            public byte? MobLevelLowerLimit { get; set; } = null;
            public byte? MobLevelUpperLimit { get; set; } = null;

            public byte RestStartPercentHp { get; set; } = 30;
            public byte RestEndPecentHp { get; set; } = 100;
            public byte RestStartPecentMp { get; set; } = 10;
            public byte RestEndPecentMp { get; set; } = 100;

            public CombatZone Zone { get; set; } = new CombatZone(new Vector3(0, 0, 0), 0);
            public bool AutoUseShots { get; set; } = true;
            public uint AttackDistanceMili { get; set; } = 80;
            public uint AttackDistanceBow { get; set; } = 500;
            public bool UseOnlySkills { get; set; } = false;
            public List<SkillCondition> SkillConditions { get; set; } = new List<SkillCondition>();

            public bool SpoilIfPossible { get; set; } = true;
            public bool SpoilIsPriority { get; set; } = false;
            public Dictionary<uint, bool> ExcludedSpoilMobIds { get; set; } = new Dictionary<uint, bool>();
            public Dictionary<uint, bool> IncludedSpoilMobIds { get; set; } = new Dictionary<uint, bool>();
            public uint SpoilSkillId { get; set; } = 254;
            public uint SweeperSkillId { get; set; } = 42;
            public byte SweepAttemptsCount { get; set; } = 10;

            public bool PickupIfPossible { get; set; } = true;
            public uint PickupMaxDeltaZ { get; set; } = 500;
            public byte PickupAttemptsCount { get; set; } = 10;
            public Dictionary<uint, bool> ExcludedItemIdsToPickup { get; set; } = new Dictionary<uint, bool>();
            public Dictionary<uint, bool> IncludedItemIdsToPickup { get; set; } = new Dictionary<uint, bool>();
        }

        public class DelevelingSection
        {
            public byte TargetLevel { get; set; } = 20;
            public uint AttackDistance { get; set; } = 80;
            public uint SkillId { get; set; } = 0;
        }

        public uint DelayBetweenTransitions { get; set; } = 250;

        public CombatSection Combat { get; } = new CombatSection();

        public DelevelingSection Deleveling { get; } = new DelevelingSection();
    }
}
