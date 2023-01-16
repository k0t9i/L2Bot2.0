#pragma once
#include "../Enums/ObjectStateEnum.h"

namespace L2Bot::Domain::DTO
{
	template <typename T>
	struct ObjectState
	{
	public:
		T object;
		Enums::ObjectStateEnum state = Enums::ObjectStateEnum::none;
	};
}