#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H



#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Physics/Collision/BroadPhase/BroadPhase.h"
#include "RigidBody3D.h"
#include "Engine/Core/EntitySystem/EntityManager.h"
#include <vector>


namespace SaltnPepperEngine
{ 

	//class Scene;

	//enum class VelocityIntegrationType
	//{
	//	LINEAR_EULAR = 0,
	//	RUNGE_KUTTA_2 = 1,
	//	RUNGE_KUTTA_4 = 2
	//};

	//namespace Physics
	//{
	//	struct PhysicsStats
	//	{
	//		uint32_t rigidBodyCount;
	//		uint32_t collisionCount;
	//		uint32_t stationaryCount;
	//		uint32_t staticCount;
	//	};

	//	//class RigidBody3D;
	//	
	//	


	//	class PhysicsEngine
	//	{
	//		

	//	public:
	//		//friend class RigidBody3D;

	//		PhysicsEngine();
	//		~PhysicsEngine();


	//		void Init();
	//		void Update(const float deltatime);
	//		//void UpdateScene(Scene* scene);

	//		void UpdateECSTransforms(Scene* scene);


	//		const bool GetIsPaused() const;
	//		void SetPaused(const bool paused);

	//		const Vector3& GetGravity() const;
	//		void SetGravity(const Vector3& gravity);

	//		static float GetDeltaTime();

	//		void UpdateRigidBodyCache();


	//		//RigidBody3D* CreateRigidBody(Entity& entity, PhysicsProperties properties);

	//		SharedPtr<RigidBody3D> CreateRigidBody(const PhysicsProperties properties = PhysicsProperties());
	//		void DeleteRigidBody(SharedPtr<RigidBody3D> body );

	//		void DebugDraw();


	//	protected:
	//		
	//		void UpdatePhysics();

	//		void BroadPhaseCollision();

	//		void NarrowPhaseCollision();

	//		void UpdateAllBodies();

	//		void UpdateRigidBody(SharedPtr<RigidBody3D>& body) const;

	//		void SolveManifolds();

	//	protected:

	//		bool m_paused = false;
	//		Vector3 m_gravity;
	//		float m_dampingFactor;

	//		uint32_t m_positionIterations = 1;
	//		uint32_t m_velocityIterations = 50;

	//		// The Storage for Rigidbody pointers for faster calculations
	//		std::vector<SharedPtr<RigidBody3D>> m_rigidBodies{};

	//		std::vector<CollisionPair> m_broadPhasePairs;


	//		std::vector<Manifold> m_manifoldList;

	//		std::mutex m_manifoldLock;


	//		SharedPtr<BroadPhase> m_broadPhaseDetection;


	//		PhysicsStats m_debugStats;

	//		static float m_physicsTimeStep;
	//		float m_timeStepCounter = 0.0f;

	//		uint8_t m_maxSubstepsPerUpdate = 5;


	//		VelocityIntegrationType m_velocityIntegrationType;


	//		//Scene* m_scene = nullptr;

	//	};
	//}
}

#endif // !PHYSICSENGINE_H

