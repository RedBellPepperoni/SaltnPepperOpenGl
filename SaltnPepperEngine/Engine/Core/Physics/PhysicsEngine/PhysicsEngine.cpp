#include "PhysicsEngine.h"
#include "Engine/Core/Scene/Scene.h"
#include "Engine/Core/Components/SceneComponents.h"
#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Physics/Collision/Broadphase/OctreeBroadPhase.h"
#include "Engine/Core/Physics/Collision/Broadphase/DefaultBroadPhase.h"
#include "Engine/Core/Physics/Collision/NarrowPhase/NarrowPhase.h"
#include "Engine/Utils/Logging/Log.h"
#include "RungeKuttaIntegration.h"


namespace SaltnPepperEngine
{
	//using Components::RigidBodyComponent;
	//using Components::Transform;

	//namespace Physics
	//{


	//	float PhysicsEngine::m_physicsTimeStep = 1.0f / 60.0f;



	//	PhysicsEngine::PhysicsEngine()
	//		: m_gravity(Vector3(0.0f, -9.81f,0.0f))
	//		, m_paused(true)
	//		, m_dampingFactor(0.99f)
	//		, m_broadPhaseDetection(nullptr)
	//		, m_velocityIntegrationType(VelocityIntegrationType::LINEAR_EULAR)
	//		
	//	{
	//		m_timeStepCounter = 0.0f;
	//		//m_rigidBodies.reserve(50);
	//		//m_broadPhasePairs.reserve(500);

	//	}

	//	PhysicsEngine::~PhysicsEngine()
	//	{
	//		m_rigidBodies.clear();
	//		m_broadPhasePairs.clear();
	//	}

	//	void PhysicsEngine::Init()
	//	{
	//		m_timeStepCounter = 0.0f;
	//		//m_gravity = Vector3(0.0f, -9.81f, 0.0f);
	//		m_gravity = Vector3(0.0f, 0.0f, 0.0f);
	//		m_paused = true;
	//		m_dampingFactor = 0.978f;
	//		//m_broadPhaseDetection = MakeShared<DefaultBroadPhase>();
	//		m_broadPhaseDetection = MakeShared<OctreeBroadPhase>(5,5,MakeShared<DefaultBroadPhase>());
	//		
	//	}

	//	void PhysicsEngine::Update(const float deltatime)
	//	{
	//		// Dont update if paused
	//		if (m_paused)
	//		{
	//			return;
	//		}

	//		if (m_rigidBodies.empty())
	//		{
	//			return;
	//		}

	//		m_debugStats.rigidBodyCount = (uint32_t)m_rigidBodies.size();
	//		
	//		
	//		m_timeStepCounter += deltatime;

	//		for (uint32_t i = 0; (m_timeStepCounter >= m_physicsTimeStep) && (i < m_maxSubstepsPerUpdate); i++)
	//		{
	//			m_timeStepCounter -= m_physicsTimeStep;
	//			UpdatePhysics();
	//		}

	//		if (m_timeStepCounter > m_physicsTimeStep)
	//		{

	//			LOG_WARN("Physics taking too long to run in realtime");
	//			m_timeStepCounter = 0;
	//		}
	//	    

	//	}

	//	/*void PhysicsEngine::UpdateScene(Scene* scene)
	//	{
	//		m_scene = scene;
	//	}*/

	//	void PhysicsEngine::UpdateECSTransforms(Scene* scene)
	//	{

	//		if (!scene || m_paused)
	//		{
	//			// No scene , so why update anything
	//			return;
	//		}

	//	    entt::registry& registry = scene->GetRegistry();

	//		// get a view of all entites having transform and Rigidbody components
	//		auto group = registry.group<RigidBodyComponent>(entt::get<Transform>);

	//		for (entt::entity entity : group)
	//		{
	//			// Get the Rigidbodycomponent reference and the Transform Reference from the group view
	//			const auto& [rigidbody, transform] = group.get<RigidBodyComponent, Transform>(entity);

	//			// Only Update entities that arent static or already stationary
	//			if(!rigidbody.GetRigidBody()->GetIsStatic() && !rigidbody.GetRigidBody()->GetIsStationary())
	//			{ 
	//				transform.SetPosition(rigidbody.GetRigidBody()->GetPosition());
	//				transform.SetRotation(rigidbody.GetRigidBody()->GetRotation());
	//			}
	//		}
	//	

	//	}

	//	const bool PhysicsEngine::GetIsPaused() const
	//	{
	//		return m_paused;
	//	}

	//	void PhysicsEngine::SetPaused(const bool paused)
	//	{
	//		m_paused = paused;
	//	}

	//	const Vector3& PhysicsEngine::GetGravity() const
	//	{
	//		// TODO: insert return statement here
	//		return m_gravity;
	//	}

	//	void PhysicsEngine::SetGravity(const Vector3& gravity)
	//	{
	//		m_gravity = gravity;
	//	}

	//	float PhysicsEngine::GetDeltaTime()
	//	{
	//		return m_physicsTimeStep;
	//	}

	//	// Too Expensive -> DO not Use
	//	// Call this when you create or Delete a rigidbody
	//	void PhysicsEngine::UpdateRigidBodyCache()
	//	{

	//		//m_rigidBodies.clear();

	//		//if (!m_scene)
	//		//{
	//		//	return;
	//		//}

	//		//ComponentView<RigidBody3D> rigidBodyView = m_scene->GetEntityManager()->GetComponentsOfType<RigidBody3D>();


	//		//for (Entity rigidbodyEntity : rigidBodyView)
	//		//{			

	//		//	RigidBody3D* rigidBody = &rigidbodyEntity.GetComponent<RigidBody3D>();
	//		//	Transform* transform = &rigidbodyEntity.GetComponent<Transform>();

	//		//	// get all the rigidBodies
	//		//	/*m_rigidBodies.push_back(rigidBody);
	//		//	m_transforms.push_back(transform);*/



	//		//}

	//		

	//	}

	//	//RigidBody3D* PhysicsEngine::CreateRigidBody(Entity& entity, PhysicsProperties properties)
	//	//{

	//	//	RigidBody3D* body = &entity.AddComponent<RigidBody3D>(properties);
	//	//	Transform* transform = &entity.GetComponent<Transform>();

	//	//	body->SetPosition(properties.position);

	//	//	transform->SetPosition(properties.position);
	//	//	transform->SetRotation(properties.rotation);
	//	//	transform->SetScale(Vector3(1.0f));

	//	//	/*if (!m_rigidBodies.empty())
	//	//	{
	//	//		m_rigidBodies.clear();
	//	//		m_transforms.clear();
	//	//	}*/


	//	//	m_rigidBodyList.emplace(body, transform);


	//	//	//UpdateRigidBodyCache();

	//	//	return body;
	//	//}

	//	SharedPtr<RigidBody3D> PhysicsEngine::CreateRigidBody(const PhysicsProperties properties)
	//	{
	//		// Create a new Rigidbody pointer and add it to the List
	//		SharedPtr<RigidBody3D> body = MakeShared<RigidBody3D>(properties);
	//		m_rigidBodies.push_back(body);
	//		return body;
	//	}

	//	void PhysicsEngine::DeleteRigidBody(SharedPtr<RigidBody3D> body)
	//	{
	//		//Compares the pointers and removes the element from the vector
	//		m_rigidBodies.erase(std::remove(m_rigidBodies.begin(), m_rigidBodies.end(), body), m_rigidBodies.end());
	//	}


	//	void PhysicsEngine::DebugDraw()
	//	{
	//	

	//		for (SharedPtr<RigidBody3D>& body : m_rigidBodies)
	//		{
	//			//body->DebugDraw(0);

	//			SharedPtr<Collider> collider = body->GetCollider();

	//			if (collider)
	//			{
	//				collider->DebugDraw(body.get());
	//			}
	//		}

	//	}

	//	

	//

	//	// Initial Check to get pairs of potential collision using AABBs
	//	
	//	void PhysicsEngine::UpdatePhysics()
	//	{

	//		m_manifoldList.clear();

	//		BroadPhaseCollision();

	//		NarrowPhaseCollision();

	//		SolveManifolds();

	//		UpdateAllBodies();



	//	}

	//	void PhysicsEngine::BroadPhaseCollision()
	//	{
	//		// Resetting the Pair cahce
	//		m_broadPhasePairs.clear();

	//		if (!m_broadPhaseDetection)
	//		{
	//			// No BroadPhase Detector found
	//			
	//			return;
	//		}

	//		
	//		
	//		// get the potential Collison pairs
	//		m_broadPhaseDetection->FindCollisionPairs(m_rigidBodies.data(), (uint32_t)m_rigidBodies.size(), m_broadPhasePairs);

	//	

	//	

	//	}
	//	void PhysicsEngine::NarrowPhaseCollision()
	//	{
	//		if(m_broadPhasePairs.empty())
	//		{
	//			return;
	//		}


	//		//LOG_CRITICAL("NarrowPhasePairs : {0}", m_broadPhasePairs.size());

	//		for (CollisionPair& pair : m_broadPhasePairs)
	//		{
	//			SharedPtr<Collider> colliderOne = pair.firstBody->GetCollider();
	//			SharedPtr<Collider> colliderTwo = pair.secondBody->GetCollider();


	//			if (colliderOne && colliderTwo)
	//			{
	//				CollisionData coldata;


	//				pair.firstBody->isColliding = false;
	//				pair.secondBody->isColliding = false;


	//				if (NarrowPhase::GetInstance().DetectCollision(pair.firstBody, pair.secondBody, colliderOne.get(), colliderTwo.get(), &coldata))
	//				{
	//					const bool callfirst = pair.firstBody->OnCollisionEvent(pair.secondBody, pair.secondBody);
	//					const bool callSecond = pair.secondBody->OnCollisionEvent(pair.firstBody, pair.secondBody);

	//					//pair.firstBody->isColliding = true;
	//					//pair.secondBody->isColliding = true;



	//					// Only Have collision Responce if none of the bodys have a response event
	//					if (callfirst && callSecond)
	//					{

	//						m_manifoldLock.lock();
	//						Manifold& manifold = m_manifoldList.emplace_back();
	//						manifold.Initilize(pair.firstBody, pair.secondBody);

	//						if (NarrowPhase::GetInstance().BuildCollisionManifold(pair.firstBody, pair.secondBody, colliderOne.get(), colliderTwo.get(), coldata, &manifold))
	//						{
	//							pair.firstBody->OnCollisionManifoldCallback(pair.firstBody, pair.secondBody, &manifold);
	//							pair.secondBody->OnCollisionManifoldCallback(pair.firstBody, pair.secondBody, &manifold);

	//							m_debugStats.collisionCount++;

	//							
	//						}

	//						else
	//						{
	//							m_manifoldList.pop_back();
	//						}


	//						m_manifoldLock.unlock();

	//					}


	//					

	//				}

	//			}

	//		}



	//		

	//	}


	//	void PhysicsEngine::UpdateAllBodies()
	//	{

	//		m_debugStats.staticCount = 0;
	//		m_debugStats.stationaryCount = 0;
	//		m_debugStats.rigidBodyCount = 0;


	//		for (SharedPtr<RigidBody3D>& body : m_rigidBodies)
	//		{
	//			UpdateRigidBody(body);

	//			m_debugStats.rigidBodyCount++;

	//			// make it stationary if the velocity is too low
	//			body->StationaryCheck();
	//		}


	//		m_debugStats.rigidBodyCount;

	//	}


	//	void PhysicsEngine::UpdateRigidBody(SharedPtr<RigidBody3D>& body) const
	//	{
	//		


	//		if (!body->GetIsStatic() && !body->GetIsStationary())
	//		{
	//			

	//			const float damping = m_dampingFactor;

	//			// Apply gravity
	//			if (body->m_invMass > 0.0f && Length(m_gravity) > 0.2f)
	//			{
	//				body->m_velocity += m_gravity * m_physicsTimeStep;
	//			}

	//			switch (m_velocityIntegrationType)
	//			{
	//			case VelocityIntegrationType::LINEAR_EULAR:
	//			{
	//				// Update position
	//				body->m_position += body->m_velocity * m_physicsTimeStep;

	//				// Update linear velocity (v = u + at)
	//				body->m_velocity += body->m_force * body->m_invMass * m_physicsTimeStep;

	//				// Linear velocity damping
	//				body->m_velocity = body->m_velocity * damping;


	//				break;
	//			}

	//			case VelocityIntegrationType::RUNGE_KUTTA_2:
	//			{
	//				RungeKuttaIntegration::State state = { body->m_position, body->m_velocity, body->m_force * body->m_invMass };

	//				RungeKuttaIntegration::RungeKutta2(state, 0.0f, m_physicsTimeStep);

	//				// Update position
	//				body->m_position = state.position;

	//				// Update linear velocity (v = u + at)
	//				body->m_velocity = state.velocity;

	//				// Linear velocity damping
	//				body->m_velocity = body->m_velocity * damping;


	//				break;
	//			}

	//			case VelocityIntegrationType::RUNGE_KUTTA_4:
	//			{
	//				RungeKuttaIntegration::State state = { body->m_position, body->m_velocity, body->m_force * body->m_invMass };


	//				RungeKuttaIntegration::RungeKutta4(state, 0.0f, m_physicsTimeStep);

	//				// Update position
	//				body->m_position = state.position;

	//				// Update linear velocity (v = u + at)
	//				body->m_velocity = state.velocity;

	//				

	//				// Linear velocity damping
	//				body->m_velocity = body->m_velocity * damping;

	//				break;
	//			}
	//			default:
	//				break;
	//			}
	//			
	//	
	//			
	//				// No rotation for now
	//			
	//			
	//			// Mark cached world transform and AABB as invalid
	//			body->m_AABBDirty = true;
	//			body->m_transformDirty = true;
	//		}

	//		m_physicsTimeStep *= m_positionIterations;

	//	}

	//	void PhysicsEngine::SolveManifolds()
	//	{
	//		for (Manifold& manifold : m_manifoldList)
	//		{
	//			manifold.SolveElasticity(m_physicsTimeStep);
	//			
	//		}


	//		for (uint32_t i = 0; i < m_velocityIterations; i++)
	//		{
	//			for (Manifold& manifold : m_manifoldList)
	//			{
	//				manifold.ApplyImpulse();
	//			}
	//		}

	//	}
	//}
}