#pragma once
#include <vector>
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Singleton.h"
#include "Engine/Core/Physics/Collision/Colliders/Collider.h"
#include "Manifold.h"



namespace SaltnPepperEngine
{
	namespace Physics
	{
		class RigidBody3D;
		class Collider;
		class SphereCollider_Deprecated;
		class MeshCollider_Deprecated;



		struct CollisionData
		{
			float penetration;

			Vector3 collisionNormal;
			Vector3 pointOnPlane;
		};

		class NarrowPhase : public ThreadSafeSingleton<NarrowPhase>
		{
		private:

			
			int m_MaxType = 0;


		public:

			NarrowPhase();
			~NarrowPhase();


			bool DetectCollision(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData);

			bool BuildCollisionManifold(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData& colData, Manifold* outManifold);
			
protected:

		    static bool CheckCollisionbySAT(const Vector3& axis, RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData);

			
			//bool DetectBoxCollision(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, SphereCollider* colliderOne, SphereCollider* colliderTwo, CollisionData* outData)

	
			bool DetectSphereCollision(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData);
			bool DetectCapsuleCollision(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData);
			bool DetectPolygonPolygon(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData);
			bool DetectSpherePolygonCollision(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData);
			bool DetectCapsulePolygonCollision(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData);
			bool DetectCapsuleSphereCollision(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData);


			Vector3 GetClosestPointOnEdges(const Vector3& target, const std::vector<ColliderEdge>& edges);
			void AddPossibleCollisionAxis(Vector3& axis, Vector3* possibleCollisionAxes, uint32_t& possibleCollisionAxesCount);
			
			float PointToLineDistance(const Vector3& linePointOne, const Vector3& linePointTwo, const Vector3& point);
			float PlaneSegmentIntersection(const Vector3& segmentOne, const Vector3& segmentTwo, const float PlaneDist, const Vector3& planeNormal);
			Vector3 PlaneEdgeIntersection(const Plane& plane, const Vector3& start, const Vector3& end) const;
			void ClosestPointBetweenTwoSegments(const Vector3& segmentOnePointOne, const Vector3& segmentTwoPointTwo, const Vector3& segTwoPointOne, const Vector3& segTwoPointTwo, Vector3& closestPointSegOne, Vector3& closestPointSegTwo);
			Vector3 ComputeClosestPointOnSegment(const Vector3& segPointA, const Vector3& segPointB, const Vector3& pointC);
			
			
			// Clipping algo which starts as an infinite space and keeps clipping on each side
			void SutherlandHodgesonClipping(Vector3* inputPoly, int inputPolyCount, int clipPlaneCount, const Plane* clipPlanes, Vector3* outPoly, int& outPolyCount, bool removePoints) const;



		};
	}
}
