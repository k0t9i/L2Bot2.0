#pragma once
#include "../ValueObjects/Vector3.h"
#include "../Serializers/Serializable.h"
#include "../Entities/Hashable.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::ValueObjects
{
	class Transform : public Serializers::Serializable, public Entities::Hashable
	{
	public:
		const Vector3& GetPosition() const
		{
			return m_Position;
		}
		const Vector3& GetRotation() const
		{
			return m_Rotation;
		}
		const Vector3& GetVelocity() const
		{
			return m_Velocity;
		}
		const Vector3& GetAcceleration() const
		{
			return m_Acceleration;
		}
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				m_Position.GetHash(),
				m_Rotation.GetHash(),
				m_Velocity.GetHash(),
				m_Acceleration.GetHash()
			});
		}
		const float_t GetSqrDistance(const Transform& other) const
		{
			return m_Position.GetSqrDistance(other.m_Position);
		}
		const float_t GetHorizontalSqrDistance(const Transform& other) const
		{
			return m_Position.GetHorizontalSqrDistance(other.m_Position);
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ L"position", m_Position.BuildSerializationNodes() },
				{ L"rotation", m_Rotation.BuildSerializationNodes() },
				{ L"velocity", m_Velocity.BuildSerializationNodes() },
				{ L"acceleration", m_Acceleration.BuildSerializationNodes() }
			};
		}

		Transform(
			const Vector3 position,
			const Vector3 rotation,
			const Vector3 velocity,
			const Vector3 acceleration
		) :
			m_Position(position),
			m_Rotation(rotation),
			m_Velocity(velocity),
			m_Acceleration(acceleration)
		{
		}

		Transform() = default;
		virtual ~Transform() = default;
	private:
		Vector3 m_Position = Vector3();
		Vector3 m_Rotation = Vector3();
		Vector3 m_Velocity = Vector3();
		Vector3 m_Acceleration = Vector3();
	};
}
