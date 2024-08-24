using Client.Application.Commands;
using Client.Application.Components;
using Client.Application.Views;
using Client.Domain.AI;
using Client.Domain.AI.IO;
using Client.Domain.Common;
using Client.Domain.Entities;
using Client.Domain.Events;
using Client.Domain.Helpers;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows.Input;

namespace Client.Application.ViewModels
{
    public class AIConfigViewModel :
        ObservableObject,
        EventHandlerInterface<HeroCreatedEvent>,
        EventHandlerInterface<HeroDeletedEvent>
    {
        public void Handle(HeroCreatedEvent @event)
        {
            hero = @event.Hero;
        }

        public void Handle(HeroDeletedEvent @event)
        {
            hero = null;
        }

        public class SkillCondition : ObservableObject
        {
            private uint id;
            private byte maxTargetPercentHp = 100;
            private byte minPlayerPercentMp = 0;
            private byte maxPlayerPercentHp = 100;

            public uint Id { get => id; set { if (value != id) { id = value; OnPropertyChanged(); } } }
            public byte MaxTargetPercentHp { get => maxTargetPercentHp; set { if (value != maxTargetPercentHp) { maxTargetPercentHp = value; OnPropertyChanged(); } } }
            public byte MinPlayerPercentMp { get => minPlayerPercentMp; set { if (value != minPlayerPercentMp) { minPlayerPercentMp = value; OnPropertyChanged(); } } }
            public byte MaxPlayerPercentHp { get => maxPlayerPercentHp; set { if (value != maxPlayerPercentHp) { maxPlayerPercentHp = value; OnPropertyChanged(); } } }
        }

        public class CombatZone : ObservableObject
        {
            private float x = 0;
            private float y = 0;
            private float radius = 0;

            public float X { get => x; set { if (value != x) { x = value; OnPropertyChanged(); } } }
            public float Y { get => y; set { if (value != y) { y = value; OnPropertyChanged(); } } }
            public float Radius { get => radius; set { if (value != radius) { radius = value; OnPropertyChanged(); } } }
        }

        public AIConfigViewModel(NpcInfoHelperInterface npcInfoHelper, ItemInfoHelperInterface itemInfoHelper, SkillInfoHelperInterface skillInfoHelper, Config config, ConfigSerializerInterface configSerializer, ConfigDeserializerInterface configDeserializer)
        {
            this.npcInfoHelper = npcInfoHelper;
            this.itemInfoHelper = itemInfoHelper;
            this.skillInfoHelper = skillInfoHelper;
            this.config = config;
            this.configSerializer = configSerializer;
            this.configDeserializer = configDeserializer;
            SaveDialogCommand = new RelayCommand(OnSaveDialog);
            OpenDialogCommand = new RelayCommand(OnOpenDialog);
            SaveCommand = new RelayCommand(OnSave);
            ResetCommand = new RelayCommand(OnReset);
            GetHeroPosition = new RelayCommand(OnGetHeroPosition);
            Skills = new ObservableCollection<ObjectInfo>(skillInfoHelper.GetAllSkills().Select(x => x.Value).Where(x => x.IsActive).ToList());
        }

        public ICommand SaveDialogCommand { get; }
        public ICommand OpenDialogCommand { get; }
        public ICommand SaveCommand { get; }
        public ICommand ResetCommand { get; }
        public ICommand GetHeroPosition { get; }
        public Action? Close {  get; set; }
        public Action<string>? OpenSaveDialog {  get; set; }
        public Func<string?>? OpenOpenDialog {  get; set; }

        public ObservableCollection<ObjectInfo> Skills { get; set; }

        public uint MobsMaxDeltaZ { get => mobsMaxDeltaZ; set { if (value != mobsMaxDeltaZ) { mobsMaxDeltaZ = value; OnPropertyChanged(); } } }
        public ObservableCollection<ObjectInfo> ExcludedMobs { get; set; } = new ObservableCollection<ObjectInfo>();
        public ObservableCollection<ObjectInfo> SelectedExcludedMobs { get; set; } = new ObservableCollection<ObjectInfo>();
        public ObservableCollection<ObjectInfo> IncludedMobs { get; set; } = new ObservableCollection<ObjectInfo>();
        public ObservableCollection<ObjectInfo> SelectedIncludedMobs { get; set; } = new ObservableCollection<ObjectInfo>();
        public byte? MobLevelLowerLimit { get => mobLevelLowerLimit; set { if (value != mobLevelLowerLimit) { mobLevelLowerLimit = value; OnPropertyChanged(); } } }
        public byte? MobLevelUpperLimit { get => mobLevelUpperLimit; set { if (value != mobLevelUpperLimit) { mobLevelUpperLimit = value; OnPropertyChanged(); } } }
        public ObservableCollection<ObjectInfo> ExcludedSpoilMobs { get; set; } = new ObservableCollection<ObjectInfo>();
        public ObservableCollection<ObjectInfo> SelectedExcludedSpoilMobs { get; set; } = new ObservableCollection<ObjectInfo>();
        public ObservableCollection<ObjectInfo> IncludedSpoilMobs { get; set; } = new ObservableCollection<ObjectInfo>();
        public ObservableCollection<ObjectInfo> SelectedIncludedSpoilMobs { get; set; } = new ObservableCollection<ObjectInfo>();
        public bool SpoilIfPossible { get => spoilIfPossible; set { if (value != spoilIfPossible) { spoilIfPossible = value; OnPropertyChanged(); } } }
        public bool SpoilIsPriority { get => spoilIsPriority; set { if (value != spoilIsPriority) { spoilIsPriority = value; OnPropertyChanged(); } } }
        public uint SpoilSkillId { get => spoilSkillId; set { if (value != spoilSkillId) { spoilSkillId = value; OnPropertyChanged(); } } }
        public uint SweeperSkillId { get => sweeperSkillId; set { if (value != sweeperSkillId) { sweeperSkillId = value; OnPropertyChanged(); } } }
        public byte SweepAttemptsCount { get => sweepAttemptsCount; set { if (value != sweepAttemptsCount) { sweepAttemptsCount = value; OnPropertyChanged(); } } }
        public ObservableCollection<ObjectInfo> ExcludedItems { get; set; } = new ObservableCollection<ObjectInfo>();
        public ObservableCollection<ObjectInfo> SelectedExcludedItems { get; set; } = new ObservableCollection<ObjectInfo>();
        public ObservableCollection<ObjectInfo> IncludedItems { get; set; } = new ObservableCollection<ObjectInfo>();
        public ObservableCollection<ObjectInfo> SelectedIncludedItems { get; set; } = new ObservableCollection<ObjectInfo>();
        public bool PickupIfPossible { get => pickupIfPossible; set { if (value != pickupIfPossible) { pickupIfPossible = value; OnPropertyChanged(); } } }
        public uint PickupMaxDeltaZ { get => pickupMaxDeltaZ; set { if (value != pickupMaxDeltaZ) { pickupMaxDeltaZ = value; OnPropertyChanged(); } } }
        public byte PickupAttemptsCount { get => pickupAttemptsCount; set { if (value != pickupAttemptsCount) { pickupAttemptsCount = value; OnPropertyChanged(); } } }
        public byte RestStartPercentHp { get => restStartPercentHp; set { if (value != restStartPercentHp) { restStartPercentHp = value; OnPropertyChanged(); } } }
        public byte RestEndPercentHp { get => restEndPercentHp; set { if (value != restEndPercentHp) { restEndPercentHp = value; OnPropertyChanged(); } } }
        public byte RestStartPercentMp { get => restStartPercentMp; set { if (value != restStartPercentMp) { restStartPercentMp = value; OnPropertyChanged(); } } }
        public byte RestEndPercentMp { get => restEndPercentMp; set { if (value != restEndPercentMp) { restEndPercentMp = value; OnPropertyChanged(); } } }
        public ObservableCollection<SkillCondition> CombatSkills { get; set; } = new ObservableCollection<SkillCondition>();
        public bool AutoUseShots { get => autoUseShots; set { if (value != autoUseShots) { autoUseShots = value; OnPropertyChanged(); } } }
        public uint AttackDistanceMili { get => attackDistanceMili; set { if (value != attackDistanceMili) { attackDistanceMili = value; OnPropertyChanged(); } } }
        public uint AttackDistanceBow { get => attackDistanceBow; set { if (value != attackDistanceBow) { attackDistanceBow = value; OnPropertyChanged(); } } }
        public bool UseOnlySkills { get => useOnlySkills; set { if (value != useOnlySkills) { useOnlySkills = value; OnPropertyChanged(); } } }
        public CombatZone Zone { get => combatZone; set { if (value != combatZone) { combatZone = value; OnPropertyChanged(); } } }
        public byte DelevelingTargetLevel { get => delevelingTargetLevel; set { if (value != delevelingTargetLevel) { delevelingTargetLevel = value; OnPropertyChanged(); } } }
        public uint DelevelingAttackDistance { get => delevelingAttackDistance; set { if (value != delevelingAttackDistance) { delevelingAttackDistance = value; OnPropertyChanged(); } } }
        public uint DelevelingSkillId { get => delevelingSkillId; set { if (value != delevelingSkillId) { delevelingSkillId = value; OnPropertyChanged(); } } }
        public byte MaxPassableHeight { get => maxPassableHeight; set { if (value != maxPassableHeight) { maxPassableHeight = value; OnPropertyChanged(); } } }
        public short PickupRadius { get => pickupRadius; set { if (value != pickupRadius) { pickupRadius = value; OnPropertyChanged(); } } }

        public void LoadConfig()
        {
            LoadConfigFrom(config);
        }

        private void LoadConfigFrom(Config config)
        {
            LoadCollections(config);
            MobsMaxDeltaZ = config.Combat.MobsMaxDeltaZ;
            MobLevelLowerLimit = config.Combat.MobLevelLowerLimit;
            MobLevelUpperLimit = config.Combat.MobLevelUpperLimit;
            SpoilIfPossible = config.Combat.SpoilIfPossible;
            SpoilIsPriority = config.Combat.SpoilIsPriority;
            SpoilSkillId = config.Combat.SpoilSkillId;
            SweeperSkillId = config.Combat.SweeperSkillId;
            SweepAttemptsCount = config.Combat.SweepAttemptsCount;
            PickupIfPossible = config.Combat.PickupIfPossible;
            PickupMaxDeltaZ = config.Combat.PickupMaxDeltaZ;
            PickupAttemptsCount = config.Combat.PickupAttemptsCount;
            RestStartPercentHp = config.Combat.RestStartPercentHp;
            RestEndPercentHp = config.Combat.RestEndPecentHp;
            RestStartPercentMp = config.Combat.RestStartPecentMp;
            RestEndPercentMp = config.Combat.RestEndPecentMp;
            AutoUseShots = config.Combat.AutoUseShots;
            AttackDistanceMili = config.Combat.AttackDistanceMili;
            AttackDistanceBow = config.Combat.AttackDistanceBow;
            UseOnlySkills = config.Combat.UseOnlySkills;
            Zone.X = config.Combat.Zone.Center.X;
            Zone.Y = config.Combat.Zone.Center.Y;
            Zone.Radius = config.Combat.Zone.Radius;
            DelevelingTargetLevel = config.Deleveling.TargetLevel;
            DelevelingAttackDistance = config.Deleveling.AttackDistance;
            DelevelingSkillId = config.Deleveling.SkillId;
            MaxPassableHeight = config.Combat.MaxPassableHeight;
            PickupRadius = config.Combat.PickupRadius;
        }

        private void SaveConfig()
        {
            config.Combat.MobsMaxDeltaZ = MobsMaxDeltaZ;
            config.Combat.MobLevelLowerLimit = MobLevelLowerLimit;
            config.Combat.MobLevelUpperLimit = MobLevelUpperLimit;
            config.Combat.SpoilIfPossible = SpoilIfPossible;
            config.Combat.SpoilIsPriority = SpoilIsPriority;
            config.Combat.SpoilSkillId = SpoilSkillId;
            config.Combat.SweeperSkillId = SweeperSkillId;
            config.Combat.SweepAttemptsCount = SweepAttemptsCount;
            config.Combat.PickupIfPossible = PickupIfPossible;
            config.Combat.PickupMaxDeltaZ = PickupMaxDeltaZ;
            config.Combat.PickupAttemptsCount = PickupAttemptsCount;
            config.Combat.RestStartPercentHp = RestStartPercentHp;
            config.Combat.RestEndPecentHp = RestEndPercentHp;
            config.Combat.RestStartPecentMp = RestStartPercentMp;
            config.Combat.RestEndPecentMp = RestEndPercentMp;
            config.Combat.AutoUseShots = AutoUseShots;
            config.Combat.AttackDistanceMili = AttackDistanceMili;
            config.Combat.AttackDistanceBow = AttackDistanceBow;
            config.Combat.UseOnlySkills = UseOnlySkills;
            config.Combat.Zone.Center.X = Zone.X;
            config.Combat.Zone.Center.Y = Zone.Y;
            config.Combat.Zone.Center.Z = 0;
            config.Combat.Zone.Radius = Zone.Radius;
            config.Deleveling.TargetLevel = DelevelingTargetLevel;
            config.Deleveling.AttackDistance = DelevelingAttackDistance;
            config.Deleveling.SkillId = DelevelingSkillId;
            config.Combat.MaxPassableHeight = MaxPassableHeight;
            config.Combat.PickupRadius = PickupRadius;
            SaveCollections();
        }

        private void LoadCollections(Config config)
        {
            ExcludedMobs.Clear();
            npcInfoHelper.GetAllNpc().ForEach(n => ExcludedMobs.Add(n));
            SelectedExcludedMobs.Clear();
            IncludedMobs.Clear();
            npcInfoHelper.GetAllNpc().ForEach(n => IncludedMobs.Add(n));
            SelectedIncludedMobs.Clear();
            ExcludedSpoilMobs.Clear();
            npcInfoHelper.GetAllNpc().ForEach(n => ExcludedSpoilMobs.Add(n));
            SelectedExcludedSpoilMobs.Clear();
            IncludedSpoilMobs.Clear();
            npcInfoHelper.GetAllNpc().ForEach(n => IncludedSpoilMobs.Add(n));
            SelectedIncludedSpoilMobs.Clear();
            ExcludedItems.Clear();
            itemInfoHelper.GetAllItems().ForEach(n => ExcludedItems.Add(n));
            SelectedExcludedItems.Clear();
            IncludedItems.Clear();
            itemInfoHelper.GetAllItems().ForEach(n => IncludedItems.Add(n));
            SelectedIncludedItems.Clear();

            var loadCollection = (ObservableCollection<ObjectInfo> items, ObservableCollection<ObjectInfo> selectedItems, Dictionary<uint, bool> configItems) =>
            {
                for (var i = items.Count - 1; i >= 0; i--)
                {
                    var mob = items[i];
                    if (configItems.ContainsKey(mob.Id))
                    {
                        items.Remove(mob);
                        selectedItems.Add(mob);
                    }
                }
            };

            loadCollection(ExcludedMobs, SelectedExcludedMobs, config.Combat.ExcludedMobIds);
            loadCollection(IncludedMobs, SelectedIncludedMobs, config.Combat.IncludedMobIds);
            loadCollection(ExcludedSpoilMobs, SelectedExcludedSpoilMobs, config.Combat.ExcludedSpoilMobIds);
            loadCollection(IncludedSpoilMobs, SelectedIncludedSpoilMobs, config.Combat.IncludedSpoilMobIds);
            loadCollection(ExcludedItems, SelectedExcludedItems, config.Combat.ExcludedItemIdsToPickup);
            loadCollection(IncludedItems, SelectedIncludedItems, config.Combat.IncludedItemIdsToPickup);

            CombatSkills.RemoveAll();
            config.Combat.SkillConditions.ForEach(x =>
            {
                CombatSkills.Add(new SkillCondition()
                {
                    Id = x.Id,
                    MaxTargetPercentHp = x.MaxTargetPercentHp,
                    MinPlayerPercentMp = x.MinPlayerPercentMp,
                    MaxPlayerPercentHp = x.MaxPlayerPercentHp
                });
            });
        }

        private void SaveCollections()
        {
            config.Combat.ExcludedMobIds = SelectedExcludedMobs.ToDictionary(x => x.Id, x => true);
            config.Combat.IncludedMobIds = SelectedIncludedMobs.ToDictionary(x => x.Id, x => true);

            config.Combat.ExcludedSpoilMobIds = SelectedExcludedSpoilMobs.ToDictionary(x => x.Id, x => true);
            config.Combat.IncludedSpoilMobIds = SelectedIncludedSpoilMobs.ToDictionary(x => x.Id, x => true);

            config.Combat.ExcludedItemIdsToPickup = SelectedExcludedItems.ToDictionary(x => x.Id, x => true);
            config.Combat.IncludedItemIdsToPickup = SelectedIncludedItems.ToDictionary(x => x.Id, x => true);

            config.Combat.SkillConditions = CombatSkills.Select(x => new Config.SkillCondition()
            {
                Id = x.Id,
                MaxTargetPercentHp = x.MaxTargetPercentHp,
                MinPlayerPercentMp = x.MinPlayerPercentMp,
                MaxPlayerPercentHp = x.MaxPlayerPercentHp
            }).ToList();
        }

        private void OnSaveDialog(object? sender)
        {
            SaveConfig();
            if (OpenSaveDialog != null)
            {
                OpenSaveDialog(configSerializer.Serialize(config));
            }
            if (Close != null)
            {
                Close();
            }
        }

        private void OnOpenDialog(object? sender)
        {
            if (OpenOpenDialog != null)
            {
                var data = OpenOpenDialog();
                if (data != null)
                {
                    var config = configDeserializer.Deserialize(data);
                    if (config != null)
                    {
                        LoadConfigFrom(config);
                    }
                }
            }
        }

        private void OnSave(object? sender)
        {
            SaveConfig();
            if (Close != null)
            {
                Close();
            }
        }

        private void OnReset(object? sender)
        {
            LoadConfigFrom(config);
        }

        private void OnGetHeroPosition(object? sender)
        {
            if (hero != null)
            {
                Zone.X = hero.Transform.Position.X;
                Zone.Y = hero.Transform.Position.Y;
            }
        }

        private readonly NpcInfoHelperInterface npcInfoHelper;
        private readonly ItemInfoHelperInterface itemInfoHelper;
        private readonly SkillInfoHelperInterface skillInfoHelper;
        private readonly Config config;
        private readonly ConfigSerializerInterface configSerializer;
        private readonly ConfigDeserializerInterface configDeserializer;
        private uint mobsMaxDeltaZ = 0;
        private byte? mobLevelLowerLimit = null;
        private byte maxPassableHeight = 0;
        private bool spoilIfPossible = false;
        private bool spoilIsPriority = false;
        private uint spoilSkillId = 0;
        private uint sweeperSkillId = 0;
        private byte sweepAttemptsCount = 0;
        private bool pickupIfPossible = false;
        private uint pickupMaxDeltaZ = 0;
        private byte pickupAttemptsCount = 0;
        private byte restStartPercentHp = 0;
        private byte restEndPercentHp = 0;
        private byte restStartPercentMp = 0;
        private byte restEndPercentMp = 0;
        private bool autoUseShots = false;
        private uint attackDistanceMili = 0;
        private uint attackDistanceBow = 0;
        private bool useOnlySkills = false;
        private CombatZone combatZone = new CombatZone();
        private Hero? hero;
        private byte delevelingTargetLevel = 0;
        private uint delevelingAttackDistance = 0;
        private uint delevelingSkillId = 0;
        private byte? mobLevelUpperLimit = null;
        private short pickupRadius = 0;
    }
}
