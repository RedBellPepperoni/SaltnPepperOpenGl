#ifndef RIGIDBODY3D_H
#define RIGIDBODY3D_H


#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/UniqueId/UniqueId.h"
#include "Engine/Core/Physics/Collision/NarrowPhase/Manifold.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingBox.h"
#include <functional>
#include <cereal/cereal.hpp>

namespace SaltnPepperEngine
{

	namespace Physics
	{

		class Collider;
		enum ColliderType : uint8_t;
		class RigidBody3D;
		class PhysicsEngine;
		class Manifold;

		//typedef  PhysicsCollisionCallback;

		//Hardcoding for now
		enum CollisionTag
		{
			STARDESTROYER = 0,
			XWING = 1,
			MISSILE = 2,
			SPHERETEST = 3,
			GENERATORS = 4

		};


		struct PhysicsProperties
		{
			// World Transform stuff
			Vector3 position = Vector3(0.0f);
			Vector3 velocity = Vector3(0.0f);
			Vector3 force = Vector3(0.0f);
			
			Quaternion rotation = Quaternion(0.0f,0.0,0.0f,1.0f);

			float mass = 10.0f;

			bool stationary = false;

			// Stic collider are easier to query
			bool isStatic = false;

			// The friection coeff
			float friction = 0.9f;

			// Triggers just intersect instead of Colliding
			bool isTrigger = false;

			// teh actual Collider Type
			SharedPtr<Collider> collider = nullptr;

			float elasticity = 0.5f;

			CollisionTag tag;


		};

		
		// Callback for Trigger Checks
		typedef std::function<bool(RigidBody3D* this_obj, RigidBody3D* colliding_obj)> PhysicsCollisionCallback;

		// Callback for Colision response check
		typedef std::function<void(RigidBody3D*, RigidBody3D*, Manifold*)> ManifoldCollisionCallback;

		class RigidBody3D
		{
			//friend class PhysicsEngine;


		public:

			RigidBody3D(const PhysicsProperties& properties = PhysicsProperties());
			~RigidBody3D();

			const Vector3& GetPosition() const;
			const Vector3& GetVelocity() const;
			const Vector3& GetForce() const;

			const Quaternion& GetRotation() const;
			const Matrix4& GetTransform() const;

			const BoundingBox GetAABB();


			BoundingBox GetBoundingBox() const;

			void SetBoundingBox(const BoundingBox& newBoundingBox);

			void UpdateModelBoundingBox();

			void SetPosition(const Vector3& newPosition);
			void SetVelocity(const Vector3& newVelocity);
			void SetForce(const Vector3& newForce);
			void SetRotation(const Quaternion& newRot);
			
			void SetOnCollisionCallback(PhysicsCollisionCallback& callback) { m_OnCollisionCallback = callback; }
			bool OnCollisionEvent(RigidBody3D* bodyFirst, RigidBody3D* bodySecond);

			void OnCollisionManifoldCallback(RigidBody3D* bodyFirst, RigidBody3D* bodySecond, Manifold* manifold);


			void SetCollider(const SharedPtr<Collider>& collider);
			void SetCollider(ColliderType type);
			SharedPtr<Collider>& GetCollider();

			uint64_t GetUniqueId() const;

			void ColliderUpdated();

			const Matrix3 GetInverseinertia() const;

			bool GetIsStatic() const;
			float GetFriction() const;

			void SetFriction(float newFriction);
			void SetStatic(bool isstatic);

			bool GetIsTrigger() const;
			void SetIsTrigger(bool trigger);

			float GetElasticity() const;
			void SetElasticity(const float newElasticity);

			void StationaryCheck();

			void SetIsStationary(const bool stationary);
			bool GetIsStationary() const;

			void SetStationaryThreshold(float value);
			const float GetStaionaryThresholdSquared() const;



			void DebugDraw(uint64_t flags);


			template <typename Archive>
			void save(Archive& archive) const
			{
				auto shape = std::unique_ptr<Collider>(m_collider.get());

				archive(cereal::make_nvp("Position", m_position), cereal::make_nvp("Rotation", m_rotation), cereal::make_nvp("Velocity", m_velocity), cereal::make_nvp("Force", m_force), cereal::make_nvp("Mass", 1.0f / m_invMass),  cereal::make_nvp("Static", m_isStatic), cereal::make_nvp("Friction", m_friction), cereal::make_nvp("Elasticity", m_elasticity), cereal::make_nvp("Collider", shape), cereal::make_nvp("Trigger", isTrigger));
				archive(cereal::make_nvp("UniqueId", (uint64_t)m_Id));
				shape.release();
			}

			template <typename Archive>
			void load(Archive& archive)
			{
				auto shape = std::unique_ptr<Collider>(m_collider.get());

				archive(cereal::make_nvp("Position", m_position), cereal::make_nvp("Rotation", m_rotation), cereal::make_nvp("Velocity", m_velocity), cereal::make_nvp("Force", m_force), cereal::make_nvp("Mass", 1.0f / m_invMass), cereal::make_nvp("Static", m_isStatic), cereal::make_nvp("Friction", m_friction), cereal::make_nvp("Elasticity", m_elasticity), cereal::make_nvp("Collider", shape), cereal::make_nvp("Trigger", isTrigger));

				m_collider = SharedPtr<Collider>(shape.get());

				ColliderUpdated();
				shape.release();

				archive(cereal::make_nvp("UniqueId", (uint64_t)m_Id));
			}

		protected:

			


			UniqueId m_Id;

			// Changing to public for easier access

		public:

			// === Basic Velocity ======
			Vector3 m_position = Vector3(0.0f);
			Vector3 m_velocity = Vector3(0.0f);
			Vector3 m_force = Vector3(0.0f);


			

			// The Callback if the object is a trigger
			PhysicsCollisionCallback m_OnCollisionCallback;

			// Storage of manifold callbacks (List since there can be multiple)
			std::vector<ManifoldCollisionCallback> m_onCollisionManifoldCallbacks;

			// ========= Rotational and angular stuff for later on
			Quaternion m_rotation = Quaternion();

			// If the object's velocity is lower than this it is said to be staionary
			float m_StationaryVelocityThresholdSquared = 0.005f;

			float velocitySum = 0.0f;

			// Check to see if a object is not acted by forces, so it doesnt need to update
			bool m_isStationary = true;

			float m_elasticity = 0.5f;

			Matrix3 m_inverseInertia;

			// Static collider are easier to query
			bool m_isStatic = false;

			// The friection coeff
			float m_friction = 0.5f;

			// Triggers just intersect instead of Colliding
			bool isTrigger = false;

			// teh actual Collider Type
			SharedPtr<Collider> m_collider = nullptr;

			float m_invMass = 0.5;

			//=== Transforms and stuff

			//(made mutable for accessing in const functions)
			mutable Matrix4 m_transform;


			//PhysicsCollisionCallback m_OnCollisionCallback;

			BoundingBox m_modelboundingBox;
			mutable BoundingBox m_aabb;


			// A small optimisation to only update transform when needed (made mutable for acceeing in const functions)
			mutable bool m_transformDirty = false;

			// Determines if the AABB needs to be updated
			bool m_AABBDirty = true;

			CollisionTag m_tag;

			bool isColliding = false;
		};

	}

}
#endif // !RIGIDBODY3D_H


