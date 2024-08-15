using Client.Domain.Entities;
using Client.Domain.Events;
using Client.Domain.Parsers;
using Client.Domain.ValueObjects;
using Client.Domain.DTO;
using Client.Domain.Enums;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.Concurrent;
using Client.Domain.Transports;
using Client.Domain.Common;
using Client.Domain.Helpers;

namespace Client.Domain.Service
{
    public class WorldHandler :
        EventHandlerInterface<HeroCreatedEvent>,
        EventHandlerInterface<HeroDeletedEvent>,
        EventHandlerInterface<CreatureCreatedEvent>,
        EventHandlerInterface<CreatureDeletedEvent>,
        EventHandlerInterface<DropCreatedEvent>,
        EventHandlerInterface<DropDeletedEvent>,
        EventHandlerInterface<SkillCreatedEvent>,
        EventHandlerInterface<SkillDeletedEvent>,
        EventHandlerInterface<ItemCreatedEvent>,
        EventHandlerInterface<ItemDeletedEvent>
    {
        public Hero? Hero => hero;

        public void RequestMoveToLocation(Vector3 location)
        {
            if (hero == null)
            {
                return;
            }

            SendMessage(OutgoingMessageTypeEnum.Move, location);
        }

        public void RequestAcquireTarget(uint id)
        {
            if (hero == null)
            {
                return;
            }

            if (hero.TargetId == id)
            {
                Debug.WriteLine("RequestAcquireTarget: creature " + id + " is already target");
                return;
            }
            if (!creatures.ContainsKey(id) && hero.Id != id)
            {
                Debug.WriteLine("RequestAcquireTarget: creature " + id + " not found");
                return;
            }

            SendMessage(OutgoingMessageTypeEnum.AcquireTarget, id);
        }

        public void RequestAttackOrFollow(uint id)
        {
            if (hero == null)
            {
                return;
            }

            if (!creatures.ContainsKey(id))
            {
                Debug.WriteLine("RequestAttackOrFollow: creature " + id + " not found");
                return;
            }

            SendMessage(OutgoingMessageTypeEnum.Attack, id);
        }

        public void RequestPickUp(uint id)
        {
            if (hero == null)
            {
                return;
            }

            if (!drops.ContainsKey(id))
            {
                Debug.WriteLine("RequestPickUp: drop " + id + " not found");
                return;
            }

            SendMessage(OutgoingMessageTypeEnum.Pickup, id);
        }

        public void RequestUseSkill(uint id, bool isForced, bool isShiftPressed)
        {
            if (hero == null)
            {
                return;
            }

            if (!skills.TryGetValue(id, out Skill? skill))
            {
                Debug.WriteLine("RequestUseSkill: skill " + id + " not found");
                return;
            }

            if (!skill.IsActive)
            {
                Debug.WriteLine("RequestUseSkill: skill " + id + " is passive");
                return;
            }

            var data = new UseSkillParams
            {
                skillId = id,
                isForced = isForced,
                isShiftPressed = isShiftPressed
            };

            SendMessage(OutgoingMessageTypeEnum.UseSkill, data);
        }

        public void RequestUseItem(uint id)
        {
            if (hero == null)
            {
                return;
            }

            if (!items.ContainsKey(id))
            {
                Debug.WriteLine("RequestUseItem: item " + id + " not found");
                return;
            }

            SendMessage(OutgoingMessageTypeEnum.UseItem, id);
        }

        public void RequestToggleAutouseSoulshot(uint id)
        {
            if (hero == null)
            {
                return;
            }

            if (!items.ContainsKey(id))
            {
                Debug.WriteLine("RequestToggleAutouseSoulshot: item " + id + " not found");
                return;
            }

            SendMessage(OutgoingMessageTypeEnum.ToggleSoulshot, id);
        }

        public void RequestSit()
        {
            if (hero == null)
            {
                return;
            }

            if (!hero.IsStanding)
            {
                Debug.WriteLine("RequestSit: hero is already sitting");
                return;
            }

            SendMessage(OutgoingMessageTypeEnum.Sit);
        }

        public void RequestStand()
        {
            if (hero == null)
            {
                return;
            }

            if (hero.IsStanding)
            {
                Debug.WriteLine("RequestStand: hero is already standing");
                return;
            }

            SendMessage(OutgoingMessageTypeEnum.Stand);
        }

        public void RequestRestartPoint(RestartPointTypeEnum type)
        {
            if (hero == null)
            {
                return;
            }

            SendMessage(OutgoingMessageTypeEnum.RestartPoint, type);
        }


        public List<NPC> GetAliveMobsSortedByDistanceToHero(uint deltaZ)
        {
            if (hero == null)
            {
                return new List<NPC> { };
            }

            return creatures
                .Where(a =>
                {
                    return a.Value is NPC
                        && a.Value.IsHostile
                        && !a.Value.VitalStats.IsDead
                        && MathF.Abs(a.Value.DeltaZ(hero)) <= deltaZ;
                })
                .OrderBy(a => a.Value.Distance(hero))
                .Select(a => (NPC)a.Value)
                .ToList();
        }

        public List<NPC> GetDeadMobsSortedByDistanceToHero(uint deltaZ)
        {
            if (hero == null)
            {
                return new List<NPC> { };
            }

            return creatures
                .Where(a =>
                {
                    return a.Value is NPC
                        && a.Value.IsHostile
                        && a.Value.VitalStats.IsDead
                        && MathF.Abs(a.Value.DeltaZ(hero)) <= deltaZ;
                })
                .OrderBy(a => a.Value.Distance(hero))
                .Select(a => (NPC)a.Value)
                .ToList();
        }

        public List<Drop> GetDropsSortedByDistanceToHero(uint deltaZ)
        {
            if (hero == null)
            {
                return new List<Drop> { };
            }

            return drops
                .Where(x => MathF.Abs(x.Value.Transform.Position.Z - hero.Transform.Position.Z) <= deltaZ)
                .OrderBy(a => a.Value.Transform.Position.HorizontalDistance(hero.Transform.Position))
                .Select(a => a.Value)
                .ToList();
        }

        public Skill? GetSkillById(uint id)
        {
            return skills.GetValueOrDefault(id);
        }

        public ItemInterface? GetItemById(uint id)
        {
            return items.Select(x => x.Value)
                .Where(x => x.ItemId == id)
                .FirstOrDefault();
        }

        public List<EtcItem> GetShotItems()
        {
            var shotIds = itemInfoHelper.GetAllItems()
                .Where(x => x.IsShot)
                .Select(x => x.Id)
                .ToDictionary(x => x, x => x);

            return items.Select(x => x.Value)
                .Where(x => x is EtcItem && shotIds.ContainsKey(x.ItemId))
                .Cast<EtcItem>()
                .ToList();
        }

        public List<NPC> GetGuards()
        {
            if (hero == null)
            {
                return new List<NPC> { };
            }

            var npcIds = npcInfoHelper.GetAllNpc()
                .Where(x => x.IsGuard)
                .Select(x => x.Id)
                .ToDictionary(x => x, x => x);

            return creatures
                .Where(x =>
                {
                    return x.Value is NPC && npcIds.ContainsKey(((NPC)x.Value).NpcId);
                })
                .Select(x => (NPC) x.Value)
                .OrderBy(x => x.Distance(hero))
                .ToList();
        }

        public WeaponItem? GetEquippedWeapon()
        {
            return items.Select(x => x.Value)
                .Where(x =>
                {
                    return x is WeaponItem
                        && ((WeaponItem)x).IsEquipped;
                })
                .Cast<WeaponItem>()
                .FirstOrDefault();
        }

        private void SendMessage<T>(OutgoingMessageTypeEnum type, T? content = default)
        {
            var message = outgoingMessageBuilder.Build(
                new OutgoingMessage<T>(type, content)
            );
            transport.SendAsync(message);
        }

        private void SendMessage(OutgoingMessageTypeEnum type)
        {
            SendMessage<uint>(type);
        }

        #region Handle Entity
        public void Handle(HeroCreatedEvent @event)
        {
            hero = @event.Hero;
        }

        public void Handle(HeroDeletedEvent @event)
        {
            hero = null;
        }

        public void Handle(CreatureCreatedEvent @event)
        {
            if (!creatures.ContainsKey(@event.Creature.Id))
            {
                creatures.TryAdd(@event.Creature.Id, @event.Creature);
            }
        }

        public void Handle(CreatureDeletedEvent @event)
        {
            creatures.Remove(@event.Id, out CreatureInterface? value);
        }

        public void Handle(DropCreatedEvent @event)
        {
            if (!drops.ContainsKey(@event.Drop.Id))
            {
                drops.TryAdd(@event.Drop.Id, @event.Drop);
            }
        }

        public void Handle(DropDeletedEvent @event)
        {
            drops.Remove(@event.Id, out Drop? value);
        }

        public void Handle(SkillCreatedEvent @event)
        {
            if (!skills.ContainsKey(@event.Skill.Id))
            {
                skills.TryAdd(@event.Skill.Id, @event.Skill);
            }
        }

        public void Handle(SkillDeletedEvent @event)
        {
            skills.Remove(@event.Id, out Skill? value);
        }

        public void Handle(ItemCreatedEvent @event)
        {
            if (!items.ContainsKey(@event.Item.Id))
            {
                items.TryAdd(@event.Item.Id, @event.Item);
            }
        }

        public void Handle(ItemDeletedEvent @event)
        {
            items.Remove(@event.Id, out ItemInterface? value);
        }
        #endregion

        public WorldHandler(OutgoingMessageBuilderInterface outgoingMessageBuilder, TransportInterface transport, ItemInfoHelperInterface itemInfoHelper, NpcInfoHelperInterface npcInfoHelper)
        {
            this.outgoingMessageBuilder = outgoingMessageBuilder;
            this.transport = transport;
            this.itemInfoHelper = itemInfoHelper;
            this.npcInfoHelper = npcInfoHelper;
        }

        private Hero? hero;
        private ConcurrentDictionary<uint, CreatureInterface> creatures = new ConcurrentDictionary<uint, CreatureInterface>();
        private ConcurrentDictionary<uint, Drop> drops = new ConcurrentDictionary<uint, Drop>();
        private ConcurrentDictionary<uint, Skill> skills = new ConcurrentDictionary<uint, Skill>();
        private ConcurrentDictionary<uint, ItemInterface> items = new ConcurrentDictionary<uint, ItemInterface>();
        private readonly OutgoingMessageBuilderInterface outgoingMessageBuilder;
        private readonly TransportInterface transport;
        private ItemInfoHelperInterface itemInfoHelper;
        private readonly NpcInfoHelperInterface npcInfoHelper;
    }
}
