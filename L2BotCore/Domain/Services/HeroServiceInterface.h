#pragma once

#include <vector>
#include "../DTO/EntityState.h"
#include "../ValueObjects/Vector3.h"

namespace L2Bot::Domain::Services
{
	class HeroServiceInterface
	{
	public:
		virtual void Move(ValueObjects::Vector3 location) const = 0;
		virtual void AcquireTarget(int objectId) const = 0;
		virtual void Attack(int objectId) const = 0;
		virtual void Pickup(int objectId) const = 0;
		virtual void UseSkill(int skillId, bool isForced, bool isShiftPressed) const = 0;
		virtual void UseItem(int objectId) const = 0;
		virtual void ToggleAutouseSoulshot(int objectId) const = 0;
		virtual void Sit() const = 0;
		virtual void Stand() const = 0;
	};
}
