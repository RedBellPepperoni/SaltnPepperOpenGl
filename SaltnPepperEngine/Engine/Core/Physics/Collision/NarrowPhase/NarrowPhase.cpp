#pragma once
#include "NarrowPhase.h"
#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"

#include "Engine/Core/Physics/Collision/Colliders/SphereCollider.h"
#include "Engine/Core/Physics/Collision/Colliders/MeshCollider.h"
#include "Engine/Core/Physics/Collision/Colliders/CapsuleCollider.h"
#include "Engine/Utils/Logging/Log.h"


namespace SaltnPepperEngine
{
	namespace Physics
	{

		


		NarrowPhase::NarrowPhase()
		{
			// making the binary value to the fill all the values with 1
			// right now max colliders is 32 (10000) so making it 63 which is (11111)

			m_MaxType = (ColliderType::MAX | (ColliderType::MAX >> 1));
		}

		NarrowPhase::~NarrowPhase()
		{
		}

		bool NarrowPhase::DetectCollision(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData)
		{
			// caching the types for faster computation
			ColliderType typeOne = colliderOne->GetType();
			ColliderType typeTwo = colliderTwo->GetType();

			if ((typeOne | typeTwo) > m_MaxType)
			{
				LOG_ERROR("INVALID Collider : {0} {1}", std::to_string(typeOne), std::to_string(typeTwo));
			}


			// Check Sphere Sphere First
			
			if ((typeOne & ColliderType::SPHERE) && (typeTwo & ColliderType::SPHERE))
			{
				// Reverse casting cuz its fun :(
				// This will Break a lot of stuff if teh casts are wrong
				return DetectSphereCollision(bodyOne, bodyTwo, colliderOne, colliderTwo, outData);
			}

			if ((typeOne & ColliderType::CAPSULE) && (typeTwo & ColliderType::CAPSULE))
			{
				return DetectCapsuleCollision(bodyOne, bodyTwo, colliderOne, colliderTwo, outData);
			}

			if ((typeOne & ColliderType::BOX) && (typeTwo & ColliderType::BOX))
			{
				return DetectPolygonPolygon(bodyOne, bodyTwo, colliderOne, colliderTwo, outData);
			}

			// If one of the object is a sphere and the other is either a box or mesh collider
			//else if ((typeOne & ColliderType::SPHERE && (typeTwo & ColliderType::MESH || typeTwo & ColliderType::BOX)) || (typeTwo & ColliderType::SPHERE && (typeOne & ColliderType::MESH || typeOne & ColliderType::BOX)))
			if ((typeOne == ColliderType::SPHERE && (typeTwo == ColliderType::MESH || typeTwo == ColliderType::BOX)))
			{
	
				return DetectSpherePolygonCollision(bodyOne, bodyTwo, colliderOne, colliderTwo, outData);
			}

			if ((typeOne & ColliderType::CAPSULE && (typeTwo & ColliderType::BOX)))
			{
				return DetectCapsulePolygonCollision(bodyOne, bodyTwo, colliderOne, colliderTwo, outData);
			}

			if ((typeOne & ColliderType::SPHERE )&& (typeTwo & ColliderType::CAPSULE))
			{
				return DetectCapsuleSphereCollision(bodyOne, bodyTwo, colliderOne, colliderTwo, outData);
			}

			return false;
		}

		bool NarrowPhase::BuildCollisionManifold(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData& colData, Manifold* outManifold)
		{

			if (!outManifold)
			{			
				return false;
			}

			ManifoldPolygon poly1, poly2;
			colliderOne->GetManifoldPolygon(bodyOne, colData.collisionNormal, poly1);
			colliderTwo->GetManifoldPolygon(bodyTwo, -colData.collisionNormal, poly2);

			if (poly1.FaceCount == 0 || poly2.FaceCount == 0)
				return false;
			else if (poly1.FaceCount == 1)
				outManifold->AddContactPoint(poly1.Faces[0], poly1.Faces[0] - colData.collisionNormal * colData.penetration, colData.collisionNormal, colData.penetration);
			else if (poly2.FaceCount == 1)
				outManifold->AddContactPoint(poly2.Faces[0] + colData.collisionNormal * colData.penetration, poly2.Faces[0], colData.collisionNormal, colData.penetration);
			else
			{
				bool flipped;
				Vector3* incPolygon;
				int incPolygonCount;
				Plane* refAdjPlanes;
				int refAdjPlanesCount;
				Plane refPlane;

				if (Abs(Dot(colData.collisionNormal, poly1.Normal)) > Abs(Dot(colData.collisionNormal, poly2.Normal)))
				{
					float planeDist = -(Dot(poly1.Faces[0], -poly1.Normal));
					refPlane = Plane(-poly1.Normal, planeDist);

					refAdjPlanes = poly1.AdjacentPlanes;
					refAdjPlanesCount = poly1.PlaneCount;
					incPolygon = poly2.Faces;
					incPolygonCount = poly2.FaceCount;

					flipped = false;
				}
				else
				{
					float planeDist = -(Dot(poly2.Faces[0], -poly2.Normal));
					refPlane = Plane(-poly2.Normal, planeDist);

					refAdjPlanes = poly2.AdjacentPlanes;
					refAdjPlanesCount = poly2.PlaneCount;
					incPolygon = poly1.Faces;
					incPolygonCount = poly1.FaceCount;

					flipped = true;
				}

				SutherlandHodgesonClipping(incPolygon, incPolygonCount, refAdjPlanesCount, refAdjPlanes, incPolygon, incPolygonCount, false);
				SutherlandHodgesonClipping(incPolygon, incPolygonCount, 1, &refPlane, incPolygon, incPolygonCount, true);

				for (int i = 0; i < incPolygonCount; i++)
				{
					auto& endPoint = incPolygon[i];
					float contact_penetration;
					Vector3 globalOnA, globalOnB;

					if (flipped)
					{
						contact_penetration = -(Dot(endPoint, colData.collisionNormal)
							- (Dot(colData.collisionNormal, poly2.Faces[0])));

						globalOnA = endPoint + colData.collisionNormal * contact_penetration;
						globalOnB = endPoint;
					}
					else
					{
						contact_penetration = Dot(endPoint, colData.collisionNormal) - Dot(colData.collisionNormal, poly1.Faces[0]);

						globalOnA = endPoint;
						globalOnB = endPoint - colData.collisionNormal * contact_penetration;
					}

					outManifold->AddContactPoint(globalOnA, globalOnB, colData.collisionNormal, contact_penetration);
				}
			}
			return true;


			
		}

		

		bool NarrowPhase::CheckCollisionbySAT(const Vector3& axis, RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData)
		{
			Vector3 minOne, maxOne;
			Vector3 minTwo, maxTwo;

			colliderOne->GetMinMaxFromAxis(bodyOne, axis, &minOne, &maxOne);
			colliderTwo->GetMinMaxFromAxis(bodyTwo, axis, &minTwo, &maxTwo);

			float minOneCorrelation = Dot(axis, minOne);
			float maxOneCorrelation = Dot(axis, maxOne);
			float minTwoCorrelation = Dot(axis, minTwo);
			float maxTwoCorrelation = Dot(axis, maxTwo);

			if (minOneCorrelation < minTwoCorrelation && maxOneCorrelation >= minTwoCorrelation)
			{
				if (outData != nullptr)
				{
					outData->collisionNormal = axis;
					outData->penetration = minTwoCorrelation - maxOneCorrelation;
					outData->pointOnPlane = maxOne + outData->collisionNormal * outData->penetration;
				}

				return true;

			}

			if (minTwoCorrelation <= minOneCorrelation && maxTwoCorrelation > minOneCorrelation)
			{
				if (outData != nullptr)
				{
					outData->collisionNormal = -axis;
					outData->penetration = minOneCorrelation - maxTwoCorrelation;
					outData->pointOnPlane = minOne + outData->collisionNormal * outData->penetration;
				}

				return true;

			}


			
			return false;
		}

		bool NarrowPhase::DetectSphereCollision(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData)
		{
			CollisionData data;

			Vector3 axis = bodyTwo->GetPosition() - bodyOne->GetPosition();

			if (Length(axis) > 0.1f)
			{
				axis = Normalize(axis);
			}

			if (!CheckCollisionbySAT(axis, bodyOne, bodyTwo, colliderOne, colliderTwo, &data))
			{
				return false;
			}
			   
			if (outData)
			{
				*outData = data;
			}

			return true;
		}

		bool NarrowPhase::DetectCapsuleCollision(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData)
		{
			CapsuleCollider* capsulecolliderOne = static_cast<CapsuleCollider*>(colliderOne);
			CapsuleCollider* capsulecolliderTwo = static_cast<CapsuleCollider*>(colliderTwo);

			CollisionData collisionData;
			collisionData.penetration = -FLT_MAX;

			float capsule1Radius = capsulecolliderOne->GetRadius();
			float capsule2Radius = capsulecolliderTwo->GetRadius();

			Vector3 positionOne = bodyOne->GetPosition();
			Vector3 positionTwo = bodyTwo->GetPosition();

			Vector3 direction = positionTwo - positionOne;
			float dist = Length(direction);

			float height1 = capsulecolliderOne->GetHeight();
			float height2 = capsulecolliderTwo->GetHeight();

			float capsule1HalfHeight = height1 * 0.5f;
			float capsule2HalfHeight = height2 * 0.5f;

			float radiusSum = capsule1Radius + capsule2Radius;

			Matrix4 capsule1ToCapsule2SpaceTransform = Inverse(bodyTwo->GetTransform()) * bodyOne->GetTransform();

			Vector3 capsule1SegA(0, -capsule1HalfHeight, 0);
			Vector3 capsule1SegB(0, capsule1HalfHeight, 0);
			capsule1SegA = Vector3(capsule1ToCapsule2SpaceTransform * Vector4(capsule1SegA,1.0f));
			capsule1SegB = Vector3(capsule1ToCapsule2SpaceTransform * Vector4(capsule1SegB,1.0f));

			Vector3 capsule2SegA(0, -capsule2HalfHeight, 0);
			Vector3 capsule2SegB(0, capsule2HalfHeight, 0);

			// The two inner capsule segments
			const Vector3 seg1 = capsule1SegB - capsule1SegA;
			const Vector3 seg2 = capsule2SegB - capsule2SegA;
			bool areParallel = (LengthSquared(Cross(seg1,seg2)) < EPSILON);

			if (areParallel)
			{
				float segmentsPerpendicularDistance = PointToLineDistance(capsule1SegA, capsule1SegB, capsule2SegA);
				if (segmentsPerpendicularDistance > radiusSum)
					return false;

				float d1 = Dot(seg1, Vector3(capsule1SegA));
				float d2 = -Dot(seg1, Vector3(capsule1SegB));

				float t1 = PlaneSegmentIntersection(capsule2SegB, capsule2SegA, d1, seg1);
				float t2 = PlaneSegmentIntersection(capsule2SegA, capsule2SegB, d2, -seg1);

				if (t1 > 0.0f && t2 > 0.0f)
				{
					// Clip the inner segment of capsule 2
					if (t1 > 1.0f)
						t1 = 1.0f;
					const Vector3 clipPointA = capsule2SegB - t1 * seg2;
					if (t2 > 1.0f)
						t2 = 1.0f;
					const Vector3 clipPointB = capsule2SegA + t2 * seg2;

					// Project point capsule2SegA onto line of inner segment of capsule 1
					const Vector3 seg1Normalized = Normalize(seg1);
					Vector3 pointOnInnerSegCapsule1 = Vector3(capsule1SegA) + Dot(seg1Normalized, Vector3(capsule2SegA - capsule1SegA)) * seg1Normalized;

					Vector3 normalCapsule2SpaceNormalized;
					Vector3 segment1ToSegment2;

					// If the inner capsule segments perpendicular distance is not zero (the inner segments are not overlapping)
					if (segmentsPerpendicularDistance > EPSILON)
					{
						// Compute a perpendicular vector from segment 1 to segment 2
						segment1ToSegment2 = (capsule2SegA - pointOnInnerSegCapsule1);
						normalCapsule2SpaceNormalized = Normalize(segment1ToSegment2);
					}
					else
					{
						Vector3 vec1(1, 0, 0);
						Vector3 vec2(0, 1, 0);

						Vector3 seg2Normalized = Normalize(seg2);

						float cosA1 = std::abs(seg2Normalized.x);
						float cosA2 = std::abs(seg2Normalized.y);
						segment1ToSegment2 = Vector3(0.0f, 0.0f, 0.0f);

						normalCapsule2SpaceNormalized = cosA1 < cosA2 ? Cross(seg2Normalized, vec1) : Cross(seg2Normalized, vec2);
					}

					Matrix4 capsule2ToCapsule1SpaceTransform = Inverse(capsule1ToCapsule2SpaceTransform);
					const Vector3 contactPointACapsule1Local = capsule2ToCapsule1SpaceTransform * Vector4(clipPointA - segment1ToSegment2 + normalCapsule2SpaceNormalized * capsule1Radius, 1.0f);
					const Vector3 contactPointBCapsule1Local = capsule2ToCapsule1SpaceTransform * Vector4(clipPointB - segment1ToSegment2 + normalCapsule2SpaceNormalized * capsule1Radius, 1.0f);
					const Vector3 contactPointACapsule2Local = clipPointA - normalCapsule2SpaceNormalized * capsule2Radius;
					const Vector3 contactPointBCapsule2Local = clipPointB - normalCapsule2SpaceNormalized * capsule2Radius;

					float penetrationDepth = radiusSum - segmentsPerpendicularDistance;

					Vector3 normalWorld = Matrix4(bodyTwo->GetRotation()) * Vector4(normalCapsule2SpaceNormalized, 1.0f);
					

					float correlation1 = Dot(normalWorld, Vector3(contactPointACapsule1Local));
					float correlation2 = Dot(normalWorld, Vector3(contactPointBCapsule1Local));

					static bool flipNormal = false;
				
					// Flip Normal if needed
					if (Dot(normalWorld, positionTwo - positionOne) < 0.0f)
						
					{
						normalWorld = -normalWorld;
					}

					collisionData.collisionNormal = Normalize(normalWorld);
					collisionData.penetration = penetrationDepth;
					collisionData.pointOnPlane = bodyOne->GetTransform() * Vector4(contactPointACapsule1Local, 1.0f);

					if (outData)
						*outData = collisionData;

					return true;
				}
			}

			Vector3 closestPointCapsule1Seg;
			Vector3 closestPointCapsule2Seg;
			ClosestPointBetweenTwoSegments(capsule1SegA, capsule1SegB, capsule2SegA, capsule2SegB,
				closestPointCapsule1Seg, closestPointCapsule2Seg);

			Vector3 closestPointsSeg1ToSeg2 = (closestPointCapsule2Seg - closestPointCapsule1Seg);
			const float closestPointsDistanceSquare = LengthSquared(closestPointsSeg1ToSeg2);

			if (closestPointsDistanceSquare < radiusSum * radiusSum)
			{
				if (closestPointsDistanceSquare > EPSILON)
				{
					float closestPointsDistance = std::sqrt(closestPointsDistanceSquare);
					closestPointsSeg1ToSeg2 /= closestPointsDistance;

					const Vector3 contactPointCapsule1Local = Vector3(Inverse(capsule1ToCapsule2SpaceTransform) * Vector4((closestPointCapsule1Seg + closestPointsSeg1ToSeg2 * capsule1Radius),1.0f));
					const Vector3 contactPointCapsule2Local = closestPointCapsule2Seg - closestPointsSeg1ToSeg2 * capsule2Radius;

					// const Vector3 normalWorld = bodyTwo->GetRotation() * closestPointsSeg1ToSeg2;
					Vector3 normalWorld = Matrix4(bodyTwo->GetRotation()) * Vector4(closestPointsSeg1ToSeg2, 1.0f);

					float penetrationDepth = radiusSum - closestPointsDistance;

					// Create the contact info object

					float correlation1 = Dot(normalWorld, Vector3(contactPointCapsule1Local));
					float correlation2 = Dot(normalWorld, Vector3(contactPointCapsule2Local));

					// if(correlation1 <= correlation2)
					if (Dot(normalWorld, positionTwo - positionOne) < 0.0f)
					{
						normalWorld = -normalWorld;
					}

					collisionData.collisionNormal = Normalize(normalWorld);
					collisionData.penetration = penetrationDepth;
					collisionData.pointOnPlane = bodyOne->GetTransform() * Vector4(contactPointCapsule1Local, 1.0f);

					if (outData)
						*outData = collisionData;

					return true;
				}
				else
				{
					if (areParallel)
					{
						float squareDistCapsule2PointToCapsuleSegA = LengthSquared((capsule1SegA - closestPointCapsule2Seg));

						Vector3 capsule1SegmentMostExtremePoint = squareDistCapsule2PointToCapsuleSegA > EPSILON ? capsule1SegA : capsule1SegB;
						Vector3 normalCapsuleSpace2 = (closestPointCapsule2Seg - capsule1SegmentMostExtremePoint);
						normalCapsuleSpace2 = Normalize(normalCapsuleSpace2);

						const Vector3 contactPointCapsule1Local = Vector3(Inverse(capsule1ToCapsule2SpaceTransform) * Vector4((closestPointCapsule1Seg + normalCapsuleSpace2 * capsule1Radius),1.0f));
						const Vector3 contactPointCapsule2Local = closestPointCapsule2Seg - normalCapsuleSpace2 * capsule2Radius;

						// const Vector3 normalWorld = bodyTwo->GetRotation() * Vector4(normalCapsuleSpace2, 1.0f);
						Vector3 normalWorld = Matrix4(bodyTwo->GetRotation()) * Vector4(normalCapsuleSpace2, 1.0f);

						float correlation1 = Dot(normalWorld, Vector3(contactPointCapsule1Local));
						float correlation2 = Dot(normalWorld, Vector3(contactPointCapsule2Local));

						// if(correlation1 <= correlation2)
						if (Dot(normalWorld, positionTwo - positionOne) < 0.0f)
						{
							normalWorld = -normalWorld;
						}
						// Create the contact info object
						collisionData.collisionNormal = Normalize(normalWorld);
						collisionData.penetration = radiusSum;
						collisionData.pointOnPlane = bodyOne->GetTransform() * Vector4(contactPointCapsule1Local, 1.0f);

						if (outData)
							*outData = collisionData;

						return true;
					}
					else
					{
						Vector3 normalCapsuleSpace2 = Cross(seg1, seg2);
						Normalize(normalCapsuleSpace2);

						const Vector3 contactPointCapsule1Local = Vector3(Inverse(capsule1ToCapsule2SpaceTransform) * Vector4((closestPointCapsule1Seg + normalCapsuleSpace2 * capsule1Radius),1.0f));
						const Vector3 contactPointCapsule2Local = closestPointCapsule2Seg - normalCapsuleSpace2 * capsule2Radius;

						// const Vector3 normalWorld = bodyTwo->GetRotation() * Vector4(normalCapsuleSpace2, 1.0f);
						Vector3 normalWorld = Matrix4(bodyTwo->GetRotation()) * Vector4(normalCapsuleSpace2, 1.0f);

						float correlation1 = Dot(normalWorld, Vector3(contactPointCapsule1Local));
						float correlation2 = Dot(normalWorld, Vector3(contactPointCapsule2Local));

						// if(correlation1 <= correlation2)
						if (Dot(normalWorld, positionTwo - positionOne) < 0.0f)
						{
							normalWorld = -normalWorld;
						}

						collisionData.collisionNormal = Normalize(normalWorld);
						collisionData.penetration = radiusSum;
						collisionData.pointOnPlane = bodyOne->GetTransform() * Vector4(contactPointCapsule1Local, 1.0f);

						if (outData)
							*outData = collisionData;

						return true;
					}
				}
			}

			return false;
		}

		bool NarrowPhase::DetectPolygonPolygon(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData)
		{
			CollisionData cur_colData;
			CollisionData best_colData;
			best_colData.penetration = -FLT_MAX;

			std::vector<glm::vec3>& shape1CollisionAxes = colliderOne->GetCollisionNormals(bodyOne);
			std::vector<glm::vec3>& shape2PossibleCollisionAxes = colliderTwo->GetCollisionNormals(bodyTwo);
	
			static const int MAX_COLLISION_AXES = 100;
			static glm::vec3 possibleCollisionAxes[MAX_COLLISION_AXES];

			uint32_t possibleCollisionAxesCount = 0;
			for (const glm::vec3& axis : shape1CollisionAxes)
			{
				possibleCollisionAxes[possibleCollisionAxesCount++] = axis;
			}

			for (const glm::vec3& axis : shape2PossibleCollisionAxes)
			{
				possibleCollisionAxes[possibleCollisionAxesCount++] = axis;
			}

			std::vector<ColliderEdge>& shape1_edges = colliderOne->GetEdgeList(bodyOne);
			std::vector<ColliderEdge>& shape2_edges = colliderTwo->GetEdgeList(bodyTwo);

			for (const ColliderEdge& edge1 : shape1_edges)
			{
				for (const ColliderEdge& edge2 : shape2_edges)
				{
					glm::vec3 e1 = edge1.secondPosition - edge1.firstPosition;
					glm::vec3 e2 = edge2.secondPosition - edge2.firstPosition;
					e1 = glm::normalize(e1);
					e2 = glm::normalize(e2);

					glm::vec3 temp = glm::cross(e1, e2);
					AddPossibleCollisionAxis(temp, possibleCollisionAxes, possibleCollisionAxesCount);
				}
			}

			for (uint32_t i = 0; i < possibleCollisionAxesCount; i++)
			{
				const glm::vec3& axis = possibleCollisionAxes[i];
				if (!CheckCollisionbySAT(axis, bodyOne, bodyTwo, colliderOne, colliderTwo, &cur_colData))
					return false;

				if (cur_colData.penetration >= best_colData.penetration)
					best_colData = cur_colData;
			}

			if (outData)
				*outData = best_colData;

			return true;
		}

		

		bool NarrowPhase::DetectSpherePolygonCollision(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData)
		{
			if (!(colliderOne->GetType() & ColliderType::SPHERE || colliderTwo->GetType() & ColliderType::SPHERE))
			{

				LOG_ERROR("None of the colliders are spheres");
				return false;
			}

			Collider* complexShape;
			RigidBody3D* complexObj;
			RigidBody3D* sphereObj;

			if (bodyOne->GetCollider()->GetType() == ColliderType::SPHERE)
			{
				sphereObj = bodyOne;
				complexShape = colliderTwo;
				complexObj = bodyTwo;
			}
			else
			{
				sphereObj = bodyTwo;
				complexShape = colliderOne;
				complexObj = bodyOne;
			}

			CollisionData cur_colData;
			CollisionData colData;
			colData.penetration = -FLT_MAX;

			
			std::vector<Vector3>& shapeCollisionAxes = complexShape->GetCollisionNormals(complexObj);

			// The predefined Edgesof the triangles are checked here
			std::vector<ColliderEdge>& complex_shape_edges = complexShape->GetEdgeList(complexObj);

			Vector3 p = GetClosestPointOnEdges(sphereObj->GetPosition(), complex_shape_edges);
			Vector3 p_t = sphereObj->GetPosition() - p;
			p_t = Normalize(p_t);

			static const int MAX_COLLISION_AXES = 100;
			static Vector3 possibleCollisionAxes[MAX_COLLISION_AXES];

			uint32_t possibleCollisionAxesCount = 0;
			for (const Vector3& axis : shapeCollisionAxes)
			{
				possibleCollisionAxes[possibleCollisionAxesCount++] = axis;
			}

			AddPossibleCollisionAxis(p_t, possibleCollisionAxes, possibleCollisionAxesCount);

			for (uint32_t i = 0; i < possibleCollisionAxesCount; i++)
			{
				const Vector3& axis = possibleCollisionAxes[i];
				if (!CheckCollisionbySAT(axis, bodyOne, bodyTwo, colliderOne, colliderTwo, &cur_colData))
					return false;

				if (cur_colData.penetration > colData.penetration)
					colData = cur_colData;
			}

			if (outData)
				*outData = colData;


			

			return true;
		}

		bool NarrowPhase::DetectCapsulePolygonCollision(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData)
		{
			Collider* complexCollider;
			CapsuleCollider* capsuleCollider;
			RigidBody3D* complexObj;
			RigidBody3D* capsuleObj;

			if (bodyOne->GetCollider()->GetType() & ColliderType::CAPSULE)
			{
				capsuleObj = bodyOne;
				complexCollider = colliderTwo;
				complexObj = bodyTwo;
				capsuleCollider = (CapsuleCollider*)colliderOne;
			}
			else
			{
				capsuleObj = bodyTwo;
				complexCollider = colliderOne;
				complexObj = bodyOne;
				capsuleCollider = (CapsuleCollider*)colliderTwo;
			}

			CollisionData cur_colData;
			CollisionData colData;
			colData.penetration = -FLT_MAX;

			std::vector<Vector3>& shapeCollisionAxes = complexCollider->GetCollisionNormals(complexObj);
			std::vector<ColliderEdge>& complex_shape_edges = complexCollider->GetEdgeList(complexObj);

			Vector3 p = GetClosestPointOnEdges(capsuleObj->GetPosition(), complex_shape_edges);
			Vector3 p_t = capsuleObj->GetPosition() - p;
			p_t = Normalize(p_t);

			static const int MAX_COLLISION_AXES = 100;
			static Vector3 possibleCollisionAxes[MAX_COLLISION_AXES];

			uint32_t possibleCollisionAxesCount = 0;
			for (const Vector3& axis : shapeCollisionAxes)
			{
				possibleCollisionAxes[possibleCollisionAxesCount++] = axis;
			}

			AddPossibleCollisionAxis(p_t, possibleCollisionAxes, possibleCollisionAxesCount);

			Vector3 capsulePos = capsuleObj->GetPosition();
			Vector4 forward = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
			Vector3 capsuleDir = capsuleObj->GetTransform() * forward;

			float capsuleRadius = capsuleCollider->GetRadius();
			float capsuleHeight = capsuleCollider->GetHeight();

			float capsuleTop = capsulePos.y + (capsuleHeight * 0.5f);
			float capsuleBottom = capsulePos.y - (capsuleHeight * 0.5f);

			for (int i = 0; i < shapeCollisionAxes.size(); i++)
			{
				const Vector3& axis = shapeCollisionAxes[i];
				if (!CheckCollisionbySAT(axis, bodyOne, bodyTwo, colliderOne, colliderTwo, &cur_colData))
					return false;

				if (cur_colData.penetration >= colData.penetration)
					colData = cur_colData;
			}

			if (Dot(colData.collisionNormal, capsuleDir) < 0.0f)
				colData.collisionNormal = -colData.collisionNormal;

			if (outData)
				*outData = colData;

			return true;
		}

		bool NarrowPhase::DetectCapsuleSphereCollision(RigidBody3D* bodyOne, RigidBody3D* bodyTwo, Collider* colliderOne, Collider* colliderTwo, CollisionData* outData)
		{
			if (!((colliderOne->GetType() | ColliderType::SPHERE) || (colliderTwo->GetType() | ColliderType::SPHERE)))
			{
				LOG_ERROR("CapsuleSphereCollison : no spheres detected");
			}

			CollisionData colData;

			CapsuleCollider* capsuleShape;
			SphereCollider* sphereShape;
			RigidBody3D* capsuleObj;
			RigidBody3D* sphereObj;

			if (bodyOne->GetCollider()->GetType() | ColliderType::SPHERE)
			{
				sphereObj = bodyOne;
				sphereShape = (SphereCollider*)colliderOne;
				capsuleShape = (CapsuleCollider*)colliderTwo;
				capsuleObj = bodyTwo;
			}
			else
			{
				capsuleObj = bodyOne;
				sphereObj = bodyTwo;
				sphereShape = (SphereCollider*)colliderTwo;
				capsuleShape = (CapsuleCollider*)colliderOne;
			}

			float sphereRadius = sphereShape->GetRadius();
			float capsuleHeight = capsuleShape->GetHeight();
			float capsuleRadius = capsuleShape->GetRadius();
			float capsuleHalfHeight = capsuleHeight * 0.5f;

			Vector3 capsulePos = capsuleObj->GetPosition();
			Vector3 spherePos = sphereObj->GetPosition();

			const Matrix4& sphereTransform = sphereObj->GetTransform();
			const Matrix4& capsuleTransform = capsuleObj->GetTransform();
			Matrix4 worldToCapsuleTransform = Inverse(capsuleTransform);

			// Transform sphere into capsule space
			Matrix4 sphereToCapsuleSpaceTransform = worldToCapsuleTransform * sphereTransform;
			Vector3 sphereToCapsuleSpacePos = Vector3(sphereToCapsuleSpaceTransform[3]);

			const Vector3 capsuleBottom(0, -capsuleHalfHeight, 0);
			const Vector3 capsuleTop(0, capsuleHalfHeight, 0);

			// Compute the point on the inner capsule segment that is the closes to centre of sphere
			const Vector3 closestPointOnSegment = ComputeClosestPointOnSegment(capsuleBottom, capsuleTop, sphereToCapsuleSpacePos);

			// Compute the distance between the sphere center and the closest point on the segment
			Vector3 sphereCenterToSegment = (closestPointOnSegment - sphereToCapsuleSpacePos);
			const float sphereSegmentDistanceSquare = LengthSquared(sphereCenterToSegment);

			// Compute the sum of the radius of the sphere and the capsule (virtual sphere)
			float sumRadius = sphereRadius + capsuleRadius;

			// If the distance between the sphere center and the closest point on the segment is less than the sum of the radius of the sphere and the capsule,
			// then there is a collision
			if (sphereSegmentDistanceSquare < sumRadius * sumRadius)
			{
				float penetrationDepth;
				Vector3 normalWorld;
				Vector3 contactPointSphereLocal;
				Vector3 contactPointCapsuleLocal;

				// If the sphere center is not on the capsule inner segment
				if (sphereSegmentDistanceSquare > EPSILON)
				{
					float sphereSegmentDistance = std::sqrt(sphereSegmentDistanceSquare);
					sphereCenterToSegment /= sphereSegmentDistance;

					contactPointSphereLocal = Inverse(sphereToCapsuleSpaceTransform) * Vector4(sphereToCapsuleSpacePos + sphereCenterToSegment * sphereRadius, 1.0f);
					contactPointCapsuleLocal = closestPointOnSegment - sphereCenterToSegment * capsuleRadius;

					normalWorld = Matrix4(capsuleObj->GetRotation()) * Vector4(sphereCenterToSegment, 1.0f);

					penetrationDepth = sumRadius - sphereSegmentDistance;

					if (bodyOne != sphereObj)
					{
						normalWorld = -normalWorld;
					}
				}
				else
				{
					// If the sphere center is on the capsule inner segment
					// We take any direction that is orthogonal to the inner capsule segment as a contact normal
					// Capsule inner segment
					Vector3 capsuleSegment = Normalize(capsuleTop - capsuleBottom);

					Vector3 vec1(1, 0, 0);
					Vector3 vec2(0, 1, 0);

					// Get the vectors (among vec1 and vec2) that is the most orthogonal to the capsule inner segment (smallest absolute dot product)
					float cosA1 = std::abs(capsuleSegment.x);
					float cosA2 = std::abs(capsuleSegment.y);

					penetrationDepth = sumRadius;

					// We choose as a contact normal, any direction that is perpendicular to the inner capsule segment
					Vector3 normalCapsuleSpace = cosA1 < cosA2 ? Cross(capsuleSegment, vec1) : Cross(capsuleSegment, vec2);
					normalWorld = Matrix4(capsuleObj->GetRotation()) * Vector4(normalCapsuleSpace, 1.0f);

					// Compute the two local contact points
					contactPointSphereLocal = Inverse(sphereToCapsuleSpaceTransform) * Vector4(sphereToCapsuleSpacePos + normalCapsuleSpace * sphereRadius, 1.0f);
					contactPointCapsuleLocal = sphereToCapsuleSpacePos - normalCapsuleSpace * capsuleRadius;
				}

				if (penetrationDepth <= 0.0f)
				{
					return false;
				}

				colData.collisionNormal = Normalize(normalWorld);
				colData.penetration = penetrationDepth;
				colData.pointOnPlane = contactPointSphereLocal;

				if (outData)
				{
					*outData = colData;
				}

				return true;
			}

			return false;
		}



		Vector3 NarrowPhase::GetClosestPointOnEdges(const Vector3& target, const std::vector<ColliderEdge>& edges)
		{
			
			Vector3 closest_point = Vector3(0.0f);
			Vector3 temp_closest_point{0.0f};

			float closest_distsq = FLT_MAX;


			for (const ColliderEdge& edge : edges)
			{
				Vector3 a_t = target - edge.firstPosition;
				Vector3 a_b = edge.secondPosition - edge.firstPosition;

				// Magnitude of AB vector 
				float magnitudeAB = Dot(a_b, a_b);     
				
				// The DOT product of a_to_t and a_to_b
				float ABAPproduct = Dot(a_t, a_b);       

				// The Normalised "distance" from a to your closest point
				float distance = ABAPproduct / magnitudeAB; 

				// Clamp returned point to be on the line, e.g if the closest point is beyond the AB return either A or B as closest points

				if (distance < 0.0f) 
					temp_closest_point = edge.firstPosition;

				else if (distance > 1)
					temp_closest_point = edge.secondPosition;
				else
					temp_closest_point = edge.firstPosition + a_b * distance;

				Vector3 c_t = target - temp_closest_point;
				float temp_distsq = Dot(c_t, c_t);

				if (temp_distsq < closest_distsq)
				{
					closest_distsq = temp_distsq;
					closest_point = temp_closest_point;
				}
			}

			return closest_point;
		}


		void NarrowPhase::AddPossibleCollisionAxis(Vector3& axis, Vector3* possibleCollisionAxes, uint32_t& possibleCollisionAxesCount)
		{
			
			if (LengthSquared(axis) < 0.00001f)
				return;

			axis = Normalize(axis);

			float value = (1.0f - 0.00001f);

			for (uint32_t i = 0; i < possibleCollisionAxesCount; i++)
			{
				const Vector3& p_axis = possibleCollisionAxes[i];
				if (Abs(Dot(axis, p_axis)) >= value)
					return;
			}

			possibleCollisionAxes[possibleCollisionAxesCount++] = axis;
		}

		float NarrowPhase::PointToLineDistance(const Vector3& linePointOne, const Vector3& linePointTwo, const Vector3& point)
		{
			float distanceLine = Length(linePointTwo - linePointOne);

			if (distanceLine < EPSILON)
			{
				return Length(point - linePointOne);
			}

			return (Length(Cross((point - linePointOne), (point- linePointTwo)))) / distanceLine;
		}

		float NarrowPhase::PlaneSegmentIntersection(const Vector3& segmentOne, const Vector3& segmentTwo, const float PlaneDist, const Vector3& planeNormal)
		{
			float t = -1.0f;

			const float nDotAB = Dot(planeNormal, segmentTwo - segmentOne);

			// If the segment is not parallel to the plane
			if (std::abs(nDotAB) > EPSILON)
			{
				t = (PlaneDist - Dot(planeNormal, segmentOne)) / nDotAB;
			}

			return t;
		}

		Vector3 NarrowPhase::PlaneEdgeIntersection(const Plane& plane, const Vector3& start, const Vector3& end) const
		{

			Vector3 ab = end - start;

			float ab_plane = Dot(plane.Normal(), ab);

			if (Abs(ab_plane) > 0.0001f)
			{
				Vector3 pointofContact = plane.Normal() * (-plane.Distance(Vector3(0.0f)));

				Vector3 width = start - pointofContact;
				float fac = -(Dot(plane.Normal(), width)) / ab_plane;
				ab = ab * fac;

				return start + ab;
			}

			return start;
		}

		void NarrowPhase::ClosestPointBetweenTwoSegments(const Vector3& segmentOnePointOne, const Vector3& segmentTwoPointTwo, const Vector3& segTwoPointOne, const Vector3& segTwoPointTwo, Vector3& closestPointSegOne, Vector3& closestPointSegTwo)
		{
			const Vector3 d1 = segmentTwoPointTwo - segmentOnePointOne;
			const Vector3 d2 = segTwoPointTwo - segTwoPointOne;
			const Vector3 r = segmentOnePointOne - segTwoPointOne;
			float a = LengthSquared(d1);
			float e = LengthSquared(d2);
			float f = Dot(d2, r);
			float s, t;

			// If both segments degenerate into points
			if (a <= EPSILON && e <= EPSILON)
			{
				closestPointSegOne = segmentOnePointOne;
				closestPointSegTwo = segTwoPointOne;
				return;
			}
			if (a <= EPSILON)
			{ // If first segment degenerates into a point

				s = 0.0f;

				// Compute the closest point on second segment
				t = Clamp(f / e, 0.0f, 1.0f);
			}
			else
			{

				float c = Dot(d1, r);

				// If the second segment degenerates into a point
				if (e <= EPSILON)
				{

					t = 0.0f;
					s = Clamp(-c / a, 0.0f, 1.0f);
				}
				else
				{
					float b = Dot(d1, d2);
					float denom = a * e - b * b;

					// If the segments are not parallel
					if (denom != 0.0f)
					{

						// Compute the closest point on line 1 to line 2 and
						// clamp to first segment.
						s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
					}
					else
					{

						// Pick an arbitrary point on first segment
						s = 0.0f;
					}

					// Compute the point on line 2 closest to the closest point
					// we have just found
					t = (b * s + f) / e;

					// If this closest point is inside second segment (t in [0, 1]), we are done.
					// Otherwise, we clamp the point to the second segment and compute again the
					// closest point on segment 1
					if (t < 0.0f)
					{
						t = 0.0f;
						s = Clamp(-c / a, 0.0f, 1.0f);
					}
					else if (t > 1.0f)
					{
						t = 1.0f;
						s = Clamp((b - c) / a, 0.0f, 1.0f);
					}
				}
			}

			// Compute the closest points on both segments
			closestPointSegOne = segmentOnePointOne + d1 * s;
			closestPointSegTwo = segTwoPointOne + d2 * t;
		}

		Vector3 NarrowPhase::ComputeClosestPointOnSegment(const Vector3& segPointA, const Vector3& segPointB, const Vector3& pointC)
		{
			const Vector3 ab = segPointB - segPointA;

			float abLengthSquare = LengthSquared(ab);

			// If the segment has almost zero length
			if (abLengthSquare < EPSILON)
			{
				// Return one end-point of the segment as the closest point
				return segPointA;
			}

			// Project point C onto "AB" line
			float t = Dot((pointC - segPointA), ab) / abLengthSquare;

			// If projected point onto the line is outside the segment, clamp it to the segment
			t = Clamp(t, 0.0f, 1.0f);

			// Return the closest point on the segment
			return segPointA + t * ab;
		
			
		}


		// Clipping algo which starts as an infinite space and keeps clipping on each side

		void NarrowPhase::SutherlandHodgesonClipping(Vector3* inputPoly, int inputPolyCount, int clipPlaneCount, const Plane* clipPlanes, Vector3* outPoly, int& outPolyCount, bool removePoints) const
		{

			if (!outPoly)
				return;

			Vector3 ppPolygon1[8], ppPolygon2[8];
			int inputCount = 0, outputCount = 0;

			Vector3* input = ppPolygon1, * output = ppPolygon2;
			inputCount = inputPolyCount;
			output = inputPoly;
			outputCount = inputCount;

			for (int iterations = 0; iterations < clipPlaneCount; ++iterations)
			{
				if (outputCount == 0)
					break;

				const Plane& plane = clipPlanes[iterations];

				std::swap(input, output);
				inputCount = outputCount;

				outputCount = 0;

				Vector3 startPoint = input[inputCount - 1];
				for (int i = 0; i < inputCount; i++)
				{
					const auto& endPoint = input[i];
					bool startInPlane = plane.IsPointOnPlane(startPoint);
					bool endInPlane = plane.IsPointOnPlane(endPoint);

					if (removePoints)
					{
						if (endInPlane)
							output[outputCount++] = endPoint;
					}
					else
					{
						// if entire edge is within the clipping plane, keep it as it is
						if (startInPlane && endInPlane)
							output[outputCount++] = endPoint;

						// if edge interesects the clipping plane, cut the edge along clip plane
						else if (startInPlane && !endInPlane)
							output[outputCount++] = PlaneEdgeIntersection(plane, startPoint, endPoint);
						else if (!startInPlane && endInPlane)
						{
							output[outputCount++] = PlaneEdgeIntersection(plane, endPoint, startPoint);
							output[outputCount++] = endPoint;
						}
					}

					startPoint = endPoint;
				}
			}

			outPoly = output;
			outPolyCount = outputCount;

		}



	}
}