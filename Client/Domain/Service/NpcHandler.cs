using Client.Domain.Entities;
using Client.Domain.Factories;
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
        }
        public override void OnDelete(NPC entity)
        {
            mainViewModel.RemoveNpc(entity);
        }

        public NpcHandler(EntityFactoryInterface<NPC> factory, MainViewModelInterface mainViewModel) : base(factory)
        {
            this.mainViewModel = mainViewModel;
        }

        private readonly MainViewModelInterface mainViewModel;
    }
}
