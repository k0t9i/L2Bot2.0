#pragma once

#include <unordered_map>
#include <math.h>
#include <functional>
#include "GameStructs.h"

class FindObjectsTrait
{
public:
	template <typename T>
	std::unordered_map<uint32_t, T> FindAllObjects(float_t radius, std::function<const T(float_t, int32_t)> getNextObject) const
	{
		std::unordered_map<uint32_t, T> result;

		auto object = getNextObject(radius, -1);

		while (object)
		{
			if (result.find(object->objectId) != result.end()) {
				break;
			}
			else {
				result.emplace(object->objectId, object);
			}
			object = getNextObject(radius, object->objectId);
		}

		return result;
	}
};