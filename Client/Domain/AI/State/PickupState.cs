using Client.Domain.AI.Combat;
using Client.Domain.Entities;
using Client.Domain.Service;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace Client.Domain.AI.State
{
    public class PickupState : BaseState
    {
        public PickupState(AI ai) : base(ai)
        {
        }

        public List<Drop> GetDrops(WorldHandler worldHandler, Config config)
        {
            var hero = worldHandler.Hero;

            if (hero == null)
            {
                return new List<Drop>();
            }

            var drops = Helper.GetDropByConfig(worldHandler, config, hero);
            for (var i = drops.Count - 1; i >= 0; i--)
            {
                if (pickupAttempts.ContainsKey(drops[0].Id) && pickupAttempts[drops[0].Id] > config.Combat.PickupAttemptsCount)
                {
                    drops.RemoveAt(i);
                }
            }
            return drops;
        }

        public bool IsSweeperMustBeUsed(WorldHandler worldHandler, Config config)
        {
            return GetSweepableMobs(worldHandler, config).Count > 0;
        }

        protected override void DoExecute(WorldHandler worldHandler, Config config, AsyncPathMoverInterface asyncPathMover, Hero hero)
        {
            if (IsSweeperMustBeUsed(worldHandler, config))
            {
                var mob = GetSweepableMobs(worldHandler, config).First();
                var sweeper = worldHandler.GetSkillById(config.Combat.SweeperSkillId);
                if (sweeper != null && sweeper.IsReadyToUse && hero.VitalStats.Mp >= sweeper.Cost)
                {
                    worldHandler.RequestAcquireTarget(mob.Id);
                    worldHandler.RequestUseSkill(sweeper.Id, false, false);
                    if (!sweepAttempts.ContainsKey(mob.Id))
                    {
                        sweepAttempts[mob.Id] = 0;
                    }
                    sweepAttempts[mob.Id]++;
                }
            }

            if (!hero.Transform.IsMoving)
            {
                var drops = GetDrops(worldHandler, config);
                if (drops.Count > 0)
                {
                    worldHandler.RequestPickUp(drops[0].Id);
                    if (!pickupAttempts.ContainsKey(drops[0].Id))
                    {
                        pickupAttempts[drops[0].Id] = 0;
                    }
                    pickupAttempts[drops[0].Id]++;
                }
            }
        }

        protected override void DoOnLeave(WorldHandler worldHandler, Config config, Hero hero)
        {
            pickupAttempts.Clear();
            sweepAttempts.Clear();
        }

        private List<NPC> GetSweepableMobs(WorldHandler worldHandler, Config config)
        {
            return worldHandler.GetDeadMobsSortedByDistanceToHero(config.Combat.MobsMaxDeltaZ)
                .Where(x =>
                {
                    return x.SpoilState == Enums.SpoilStateEnum.Sweepable &&
                        (!sweepAttempts.ContainsKey(x.Id) || sweepAttempts[x.Id] <= config.Combat.SweepAttemptsCount);
                })
                .ToList();
        }

        private Dictionary<uint, short> pickupAttempts = new Dictionary<uint, short>();
        private Dictionary<uint, short> sweepAttempts = new Dictionary<uint, short>();
    }
}
