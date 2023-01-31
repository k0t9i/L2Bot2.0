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
    public class DropHandler : EntityHandler<Drop>
    {
        public override void OnCreate(Drop entity)
        {
            mainViewModel.AddDrop(entity);
        }
        public override void OnDelete(Drop entity)
        {
            mainViewModel.RemoveDrop(entity);
        }

        public DropHandler(EntityFactoryInterface<Drop> factory, MainViewModelInterface mainViewModel) : base(factory)
        {
            this.mainViewModel = mainViewModel;
        }

        private readonly MainViewModelInterface mainViewModel;
    }
}
