using Client.Domain.Entities;
using Client.Domain.Factories;
using Client.Domain.Helpers;
using Client.Domain.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client.Domain.Service
{
    public class NpcHandler : EntityHandler<NPC>
    {
        public override void OnCreate(NPC entity)
        {
            mainViewModel.AddNpc(entity);
            entity.Level = npcInfoHelper.GetLevel(entity.NpcId);
            entity.AggroRadius = npcInfoHelper.GetAggroRadius(entity.NpcId);
        }
        public override void OnDelete(NPC entity)
        {
            mainViewModel.RemoveNpc(entity);
        }

        public NpcHandler(EntityFactoryInterface<NPC> factory, MainViewModelInterface mainViewModel, NpcInfoHelperInterface npcInfoHelper) : base(factory)
        {
            this.mainViewModel = mainViewModel;
            this.npcInfoHelper = npcInfoHelper;
        }

        private readonly MainViewModelInterface mainViewModel;
        private readonly NpcInfoHelperInterface npcInfoHelper;
    }
}
