using Client.Application.Components;
using Client.Domain.Entities;
using Client.Domain.Service;

namespace Client.Domain.AI.State
{
    public class AttackGuardState : BaseState
    {
        public AttackGuardState(AI ai) : base(ai)
        {
        }

        protected override void DoExecute(WorldHandler worldHandler, Config config, AsyncPathMoverInterface asyncPathMover, Hero hero)
        {
            if (hero.Target == null)
            {
                return;
            }

            var skill = worldHandler.GetSkillById(config.Deleveling.SkillId);

            if (skill != null && skill.IsReadyToUse && skill.Cost <= hero.VitalStats.Mp)
            {
                worldHandler.RequestUseSkill(skill.Id, true, false);
            }
        }
    }
}
