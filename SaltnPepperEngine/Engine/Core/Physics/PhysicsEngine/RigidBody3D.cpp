#include "RigidBody3D.h"
#include "Engine/Core/Physics/Collision/Colliders/Collider.h"
#include "Engine/Core/Physics/Collision/Colliders/BoxCollider.h"
#include "Engine/Core/Physics/Collision/Colliders/SphereCollider.h"
#include "Engine/Core/Physics/Collision/Colliders/CapsuleCollider.h"
#include "Engine/Utils/Logging/Log.h"
#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"

namespace SaltnPepperEngine
{
	using namespace Rendering;

	namespace Physics
	{
		RigidBody3D::RigidBody3D(const PhysicsProperties& properties)
			: m_AABBDirty(true)
			, m_transformDirty(true)
			, m_isStationary(properties.stationary)
			, m_position(properties.position)
			, m_velocity(properties.velocity)
			, m_force(properties.force)
			, m_rotation(properties.rotation)
			, m_isStatic(properties.isStatic)
			, m_friction(properties.friction)
			, m_tag(properties.tag)
			,m_elasticity(properties.elasticity)
		{
			m_modelboundingBox.Set(Vector3(-0.5f), Vector3(0.5f));

			m_invMass = 1.0f/properties.mass;

			if (properties.collider)
			{
				SetCollider(properties.collider);
			}


			m_Id = UniqueId();

			

		}


		RigidBody3D::~RigidBody3D()
		{
		}

		const Vector3& RigidBody3D::GetPosition() const
		{
			return m_position;
		}

		const Vector3& RigidBody3D::GetVelocity() const
		{
			return m_velocity;
		}

		const Vector3& RigidBody3D::GetForce() const
		{
			return m_force;
		}
		const Quaternion& RigidBody3D::GetRotation() const
		{
			return m_rotation;
		}

		const Matrix4& RigidBody3D::GetTransform() const
		{
			// We need to recalculate the Transform
			if (m_transformDirty)
			{
				// Update teh cached trasnform
				m_transform = Translate(Matrix4(1.0f), m_position) * QuatToMatrix(m_rotation);
			
				// We just Updated, no it can be cached
				m_transformDirty = false;
			}

			// Return the cached tranform
			return m_transform;

		}

		const BoundingBox RigidBody3D::GetAABB() 
		{
			if (m_AABBDirty)
			{
				m_aabb = m_modelboundingBox.GetTransformedBox(GetTransform());
				m_AABBDirty = false;
			}

			return m_aabb;
		}

		BoundingBox RigidBody3D::GetBoundingBox() const
		{
			return m_modelboundingBox;
		}

		void RigidBody3D::SetBoundingBox(const BoundingBox& newBoundingBox)
		{
			m_modelboundingBox.Set(newBoundingBox.Min(), newBoundingBox.Max());

			m_AABBDirty = true;
		}

		void RigidBody3D::UpdateModelBoundingBox()
		{
			m_modelboundingBox.Reset();


			const Vector3 x(1.0f, 0.0f, 0.0f);
			const Vector3 y(0.0f, 1.0f, 0.0f);
			const Vector3 z(0.0f, 0.0f, 1.0f);

			Vector3 lower;
			Vector3 upper;

			

			if (!m_collider)
			{
				return;
			}

			m_collider->GetMinMaxFromAxis(nullptr, x, &lower, &upper);
			m_modelboundingBox.Merge(lower);
			m_modelboundingBox.Merge(upper);

			m_collider->GetMinMaxFromAxis(nullptr, y, &lower, &upper);
			m_modelboundingBox.Merge(lower);
			m_modelboundingBox.Merge(upper);

			m_collider->GetMinMaxFromAxis(nullptr, z, &lower, &upper);
			m_modelboundingBox.Merge(lower);
			m_modelboundingBox.Merge(upper);

			m_AABBDirty = true;
		}

		void RigidBody3D::SetPosition(const Vector3& newPosition)
		{
			m_position = newPosition;
			m_transformDirty = true;
			m_AABBDirty = true;
		}


		void RigidBody3D::SetVelocity(const Vector3& newVelocity)
		{
			if (m_isStatic)
			{ 
				// No velocity change happens on static bodies
				return;
			}

			m_velocity = newVelocity;

			// there is some velocity so the Physics engine should simulate
			m_isStationary = false;


		}
		void RigidBody3D::SetForce(const Vector3& newForce)
		{
			if (m_isStatic)
			{
				// No force change happens on static bodies
				return;
			}

			m_force = newForce;
			m_isStationary = false;

		}

		void RigidBody3D::SetRotation(const Quaternion& newRot)
		{
			m_rotation = newRot;
			m_transformDirty = true;
			m_isStationary = false;
		}
		bool RigidBody3D::OnCollisionEvent(RigidBody3D* bodyFirst, RigidBody3D* bodySecond)
		{
			// Work on this

			const bool handleCollision = (m_OnCollisionCallback) ? m_OnCollisionCallback(bodyFirst, bodySecond) : true;

			if (handleCollision)
			{
				SetIsStationary(false);
			}


			return handleCollision;
		}

		void RigidBody3D::OnCollisionManifoldCallback(RigidBody3D* bodyFirst, RigidBody3D* bodySecond, Manifold* manifold)
		{
			//// Send a callback for each manifold
			//for (ManifoldCollisionCallback callback : m_onCollisionManifoldCallbacks)
			//{
			//	callback(bodyFirst, bodySecond, manifold);
			//}

			for (auto it = m_onCollisionManifoldCallbacks.begin(); it != m_onCollisionManifoldCallbacks.end(); ++it)
			{
				it->operator()(bodyFirst, bodySecond, manifold);
			}

		}

		void RigidBody3D::SetCollider(const SharedPtr<Collider>& collider)
		{
			m_collider = collider;
			ColliderUpdated();

		}
		
		
		void RigidBody3D::SetCollider(ColliderType type)
		{
			switch (type)
			{
			case Physics::BOX: SetCollider(MakeShared<BoxCollider>());
				break;
			case Physics::SPHERE:SetCollider(MakeShared<SphereCollider>());
				break;
			case Physics::CAPSULE:SetCollider(MakeShared<CapsuleCollider>());
				break;
			default: LOG_ERROR("Unsupported Collider");
				break;
			}
		}


		SharedPtr<Collider>& RigidBody3D::GetCollider()
		{
			return m_collider;
		}

		uint64_t RigidBody3D::GetUniqueId() const
		{
			return m_Id.GetId();
		}

		void RigidBody3D::ColliderUpdated()
		{
			if (m_collider)
			{
				m_inverseInertia = m_collider->BuildInverseInertia(m_invMass);

			}

			UpdateModelBoundingBox();
		}
		const Matrix3 RigidBody3D::GetInverseinertia() const
		{
			return m_inverseInertia;
		}
		bool RigidBody3D::GetIsStatic() const
		{
			return m_isStatic;
		}
		float RigidBody3D::GetFriction() const
		{
			return 0.0f;
		}
		void RigidBody3D::SetFriction(float newFriction)
		{
		}
		void RigidBody3D::SetStatic(bool isstatic)
		{
			m_isStatic = isstatic;
		}
		bool RigidBody3D::GetIsTrigger() const
		{
			return false;
		}

		void RigidBody3D::SetIsTrigger(bool trigger)
		{
		}

		float RigidBody3D::GetElasticity() const
		{
			return m_elasticity;
		}

		void RigidBody3D::SetElasticity(const float newElasticity)
		{
			m_elasticity = newElasticity;
		}

		void RigidBody3D::StationaryCheck()
		{
			if (m_StationaryVelocityThresholdSquared < 0.0f)
			{
				return;
			}

			static const float delta = 0.2f;

			

			const float value = LengthSquared(m_velocity);

			velocitySum += delta * (value - velocitySum);

			// Set the object stationry if teh velocity squared is less than the thrershold
			m_isStationary = velocitySum <= m_StationaryVelocityThresholdSquared;

		}

		void RigidBody3D::SetIsStationary(const bool stationary)
		{
			m_isStationary = stationary;
		}

		bool RigidBody3D::GetIsStationary() const
		{
			return m_isStationary;
		}

		void RigidBody3D::SetStationaryThreshold(float value)
		{
			if (value <= 0)
			{
				// some negative value just in case
				m_StationaryVelocityThresholdSquared = -10.0f;
				return;
			}

			m_StationaryVelocityThresholdSquared = Square(value);
		}

		const float RigidBody3D::GetStaionaryThresholdSquared() const
		{
			return m_StationaryVelocityThresholdSquared;
		}

		void RigidBody3D::DebugDraw(uint64_t flags)
		{
			
			Vector4 color(1.0f,0.3f,0.7f,1.0f);

			
			if (!GetIsStationary())
			{
				// Make it green to show it is doing physics
				color = Vector4(0.0f,1.0f,0.0f,1.0f);
			}

			

			BoundingBox box = m_aabb;

			// For now just Draw AABBS
	
			DebugRenderer::DebugDraw(box, color, true);

		}
		
	}
}
