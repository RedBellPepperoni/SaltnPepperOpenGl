#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Engine/Core/Physics/PhysicsSystem/PhysicsAPI.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{

		enum class AnisotropicFriction
        {
            DISABLED = 0,
            ENABLED = 1,
            ROLLING_FRICTION = 2
        };

        enum class RigidBodyType
        {
            STATIC,
            DYNAMIC,
            KINEMATIC,
        };

        enum class PhysicsTag
        {
            PLAYER,
            PLATFORM,
            GROUND,
            BOOMERANG
        };

		class RigidBody
		{
            using CollisionCallback = std::function<void(RigidBody*, RigidBody*)>;
		
            UniquePtr<BulletRigidBody> rigidbody;
            CollisionCallback onCollision;
            CollisionCallback onCollisionEnter;
            CollisionCallback onCollisionExit;

            void UpdateTransform(Transform& ECSTransform);
            void UpdateCollider();

            Transform localTransform;
        

        public:
            RigidBody() = default;
            void Init();
            void OnUpdate(float dt);
            void SyncObjectState();

            BulletRigidBody* GetBulletHandle() const;
            void InvokeOnCollisionCallback(RigidBody* self, RigidBody* object);
            void InvokeOnCollisionEnterCallback(RigidBody* self, RigidBody* object);
            void InvokeOnCollisionExitCallback(RigidBody* self, RigidBody* object);

            void MakeKinematic();
            void MakeDynamic();
            void MakeStatic();
            void MakeTrigger();
            bool IsKinematic() const;
            bool IsDynamic() const;
            bool IsStatic() const;
            bool IsTrigger() const;
            bool IsRayCastable() const;
            void ToggleRayCasting(bool value);
            bool IsMoving() const;
            void ToggleTrigger(bool value);
            RigidBodyType GetTypeInternal() const;
            void SetTypeInternal(RigidBodyType type);


            template<typename Func>
            void SetOnCollisionCallback(Func&& func)
            {
                static_assert(std::is_convertible_v<Func, CollisionCallback>,
                    "callback must be in form `void callback(RigidBody* self, RigidBody* object)`");
                this->onCollision = std::forward<Func>(func);
            }

            template<typename Func>
            void SetOnCollisionEnterCallback(Func&& func)
            {
                static_assert(std::is_convertible_v<Func, CollisionCallback>,
                    "callback must be in form `void callback(RigidBody* self, RigidBody* object)`");
                this->onCollisionEnter = std::forward<Func>(func);
            }

            template<typename Func>
            void SetOnCollisionExitCallback(Func&& func)
            {
                static_assert(std::is_convertible_v<Func, CollisionCallback>,
                    "callback must be in form `void callback(RigidBody* self, RigidBody* object)`");
                this->onCollisionExit = std::forward<Func>(func);
            }

            void SetCollisionFilter(uint32_t mask, uint32_t group = CollisionGroup::ALL);
            void SetCollisionFilter(CollisionLayer::Mask mask, CollisionGroup::Group group = CollisionGroup::ALL);
            uint32_t GetCollisionGroup() const;
            uint32_t GetCollisionMask() const;
            void ActivateParentIsland();
            void SetActivationState(ActivationState state);
            ActivationState GetActivationState() const;
            //AABB GetAABB() const;
            void Activate();
            void ClearForces();
            float GetMass() const;
            void SetMass(float mass);
            float GetFriction() const;
            void SetFriction(float value);
            float GetSpinningFriction() const;
            void SetSpinningFriction(float value);
            float GetRollingFriction() const;
            void SetRollingFriction(float value);
            float GetBounceFactor() const;
            void SetBounceFactor(float value);
            void SetGravity(Vector3 gravity);
            Vector3 GetGravity() const;
            void SetLinearVelocity(Vector3 velocity);
            Vector3 GetLinearVelocity() const;
            void SetAngularVelocity(Vector3 velocity);
            Vector3 GetAngularVelocity() const;
            float GetInverseMass() const;
            Vector3 GetAngularForceFactor() const;
            void SetAngularForceFactor(Vector3 factor);
            Vector3 GetLinearForceFactor() const;
            void SetLinearForceFactor(Vector3 factor);
            void SetAngularAirResistance(float value);
            float GetAngularAirResistance() const;
            void SetLinearAirResistance(float value);
            float GetLinearAirResistance() const;
            Vector3 GetTotalForceApplied() const;
            Vector3 GetTotalTorqueApplied() const;
            Vector3 GetInertia() const;
            Vector3 GetVelocityInPoint(Vector3 relativePosition);
            Vector3 GetPushVelocityInPoint(Vector3 relativePosition);
            void ApplyCentralImpulse(Vector3 impulse);
            void ApplyCentralPushImpulse(Vector3 impulse);
            void ApplyForce(const Vector3& force, const Vector3& relativePosition);
            void ApplyImpulse(const Vector3& impulse, const Vector3& relativePosition);
            void ApplyPushImpulse(const Vector3& impulse, const Vector3& relativePosition);
            void ApplyTorque(const Vector3& force);
            void ApplyTorqueImpulse(const Vector3& impulse);
            void ApplyTorqueTurnImpulse(const Vector3& impulse);
            void ApplyCentralForce(const Vector3& force);
            void SetPushVelocity(Vector3 velocity);
            Vector3 GetPushVelocity() const;
            void SetTurnVelocity(Vector3 velocity);
            Vector3 GetTurnVelocity() const;
            void SetAnisotropicFriction(Vector3 friction, AnisotropicFriction mode);
            void SetAnisotropicFriction(Vector3 friction);
            bool HasAnisotropicFriction() const;
            void ToggleAnisotropicFriction(bool value);
            Vector3 GetAnisotropicFriction() const;
        };

	}
	

}


#endif // !RIGIDBODY_H
