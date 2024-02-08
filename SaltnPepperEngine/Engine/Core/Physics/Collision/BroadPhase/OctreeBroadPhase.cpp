#include "OctreeBroadPhase.h"
#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"

namespace SaltnPepperEngine
{
	using namespace Rendering;

	namespace Physics
	{
		OctreeBroadPhase::OctreeBroadPhase(size_t maxObjectsPerPartition, size_t maxPartitionDepth, const SharedPtr<BroadPhase>& secondaryBroadphase)
			: m_maxObjectsPerPartition(maxObjectsPerPartition)
			, m_maxPartitionDepth(maxPartitionDepth)
			, m_secondaryBroadPhase(secondaryBroadphase)
			, m_leaves()
		{
			m_nodePool[0].ChildCount = 0;
			m_nodePool[0].PhysicsObjectCount = 0;
			m_nodePool[0].Index = 0;
		}

		OctreeBroadPhase::~OctreeBroadPhase()
		{
		}

		void OctreeBroadPhase::FindCollisionPairs(SharedPtr<RigidBody3D>* bodies, uint32_t count, std::vector<CollisionPair>& collisionPairs)
		{
			m_currentPoolIndex = 0;
			m_leafCount = 0;

			OctreeNode& rootNode = m_nodePool[0];
			rootNode.ChildCount = 0;
			rootNode.PhysicsObjectCount = 0;
			rootNode.Index = 0;
			rootNode.boundingBox = BoundingBox();

			for (uint32_t index = 0; index < count; index++)
			{
				SharedPtr<RigidBody3D>& physicsBody = bodies[index];

				if (physicsBody && physicsBody->GetCollider())
				{
					rootNode.boundingBox.Merge(physicsBody->GetAABB());
					rootNode.PhysicsObjects[rootNode.PhysicsObjectCount] = physicsBody;
					rootNode.PhysicsObjectCount++;
				}
			}

			m_currentPoolIndex++;

			// Recursively Divide the Root bounding BOX
			Divide(rootNode, 0);
			

			std::vector<CollisionPair> pairs;

			// Add collision pair int leaf world divisions
			for (uint32_t index = 0; index < m_leafCount; index++)
			{


				if (m_nodePool[m_leaves[index]].PhysicsObjectCount > 1)
				{

					m_secondaryBroadPhase->FindCollisionPairs(m_nodePool[m_leaves[index]].PhysicsObjects, m_nodePool[m_leaves[index]].PhysicsObjectCount, collisionPairs);

				}
			}


		}

		

		void OctreeBroadPhase::DebugDraw()
		{
			DebugDrawNode(m_nodePool[0]);

		}

		void OctreeBroadPhase::Divide(OctreeNode& node, size_t iteration)
		{
			if (iteration > m_maxPartitionDepth || node.PhysicsObjectCount <= m_maxObjectsPerPartition)
			{
				// Ignore any subnodes that contain no objects
				if (node.PhysicsObjectCount != 0)
				{
					m_leaves[m_leafCount] = node.Index;
					m_leafCount++;
				}

				return;
			}

			Vector3 nodePoints[3] = { node.boundingBox.Min(), node.boundingBox.Center(), node.boundingBox.Max() };

			static const size_t NUM_nodeS = 8;
			static const size_t node_POINT_INDICES[NUM_nodeS][6] = {
				{ 0, 0, 0, 1, 1, 1 },
				{ 1, 0, 0, 2, 1, 1 },
				{ 0, 1, 0, 1, 2, 1 },
				{ 1, 1, 0, 2, 2, 1 },
				{ 0, 0, 1, 1, 1, 2 },
				{ 1, 0, 1, 2, 1, 2 },
				{ 0, 1, 1, 1, 2, 2 },
				{ 1, 1, 1, 2, 2, 2 }
			};

			for (size_t i = 0; i < NUM_nodeS; i++)
			{
				

				node.ChildNodeIndicies[node.ChildCount] = m_currentPoolIndex;
				auto& newNode = m_nodePool[m_currentPoolIndex];
				newNode.ChildCount = 0;
				newNode.PhysicsObjectCount = 0;
				newNode.Index = m_currentPoolIndex;

				node.ChildCount++;
				m_currentPoolIndex++;

				const Vector3 lower(nodePoints[node_POINT_INDICES[i][0]].x, nodePoints[node_POINT_INDICES[i][1]].y, nodePoints[node_POINT_INDICES[i][2]].z);
				const Vector3 upper(nodePoints[node_POINT_INDICES[i][3]].x, nodePoints[node_POINT_INDICES[i][4]].y, nodePoints[node_POINT_INDICES[i][5]].z);

				newNode.boundingBox.m_min = lower;
				newNode.boundingBox.m_max = upper;

				// Add objects inside node
				for (uint32_t i = 0; i < node.PhysicsObjectCount; i++)
				{
					SharedPtr<RigidBody3D> physicsObject = node.PhysicsObjects[i];

					if (!physicsObject)
						continue;

					const BoundingBox& boundingBox = physicsObject->GetAABB();
					IntersectionResult intersection = newNode.boundingBox.CheckInterscetion(boundingBox);
					if (intersection != OUTSIDE)
					{
						newNode.PhysicsObjects[newNode.PhysicsObjectCount] = physicsObject;
						newNode.PhysicsObjectCount++;

						if (intersection == INSIDE)
							node.PhysicsObjects[i] = nullptr;
					}
				}

				// Do further subnodeing
				Divide(newNode, iteration + 1);
			}
		}

		void OctreeBroadPhase::DebugDrawNode(const OctreeNode& node)
		{

			DebugRenderer::DebugDraw(node.boundingBox, Vector4(0.8f, 0.2f, 0.4f, 1.0f));

			for (uint32_t index = 0; index < node.ChildCount; index++)
			{
				DebugDrawNode(m_nodePool[node.ChildNodeIndicies[index]]);
			}
		}
	}
}