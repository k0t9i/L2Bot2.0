#pragma once
#include <math.h>
#include "../Serializers/Serializable.h"

namespace L2Bot::Domain::ValueObjects
{
	class Vector3 : public Serializers::Serializable
	{
	public:
		const float_t GetX() const
		{
			return m_X;
		}
		const float_t GetY() const
		{
			return m_Y;
		}
		const float_t GetZ() const
		{
			return m_Z;
		}
		const bool IsEqual(const Vector3* other) const
		{
			float_t epsilon = 0.0001f;
			return fabsf(m_X - other->m_X) < epsilon &&
				fabsf(m_Y - other->m_Y) < epsilon &&
				fabsf(m_Z - other->m_Z) < epsilon;
		}
		const float_t GetSqrDistance(const Vector3& other) const
		{
			return (m_X - other.m_X) * (m_X - other.m_X) + 
				(m_Y - other.m_Y) * (m_Y - other.m_Y) +
				(m_Z - other.m_Z) * (m_Z - other.m_Z);
		}
		const float_t GetHorizontalSqrDistance(const Vector3& other) const
		{
			return (m_X - other.m_X) * (m_X - other.m_X) +
				(m_Y - other.m_Y) * (m_Y - other.m_Y);
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ "x", std::to_string(m_X) },
				{ "y", std::to_string(m_Y) },
				{ "z", std::to_string(m_Z) }
			};
		}

		Vector3(const float_t x, const float_t y, const float_t z) :
			m_X(x), m_Y(y), m_Z(z)
		{
		}

		Vector3() = default;
		virtual ~Vector3() = default;

	private:
		float_t m_X = 0;
		float_t m_Y = 0;
		float_t m_Z = 0;
	};
}