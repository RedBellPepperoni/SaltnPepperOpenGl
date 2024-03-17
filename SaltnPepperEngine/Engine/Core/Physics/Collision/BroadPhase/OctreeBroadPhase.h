#pragma once
#include "BroadPhase.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingBox.h"


#define MAX_OBJECTS_PER_NODE 1024
#define MAX_PARTITION_DEPTH 16

namespace SaltnPepperEngine
{
	


	namespace Physics
	{
		class RigidBody3D;
		struct CollisionPair;

		
		class OctreeBroadPhase : public BroadPhase
		{
		public:

			struct OctreeNode
			{
				OctreeNode()
					: Index(0)
					, ChildCount(0)
					, PhysicsObjectCount(0)
				{}

				~OctreeNode()
				{}

				uint32_t Index = 0;
				uint32_t ChildCount = 0;
				uint32_t PhysicsObjectCount = 0;
				uint32_t ChildNodeIndicies[8] = { 0 };
				SharedPtr<RigidBody3D> PhysicsObjects[MAX_OBJECTS_PER_NODE];

				BoundingBox boundingBox;

			};

		public:


			OctreeBroadPhase(size_t maxObjectsPerPartition, size_t maxPartitionDepth, const SharedPtr<BroadPhase>& secondaryBroadphase);
			virtual ~OctreeBroadPhase();

			void FindCollisionPairs(SharedPtr<RigidBody3D>* bodies, uint32_t count, std::vector<CollisionPair>& collisionPairs) override;
			
			void DebugDraw() override;


		private:

			void Divide(OctreeNode& node, size_t iteration);
			void DebugDrawNode(const OctreeNode& node);


		private:

			size_t m_maxObjectsPerPartition;
			size_t m_maxPartitionDepth;

			uint32_t m_currentPoolIndex = 0;
			uint32_t m_leafCount = 0;

			SharedPtr<BroadPhase> m_secondaryBroadPhase;
			OctreeNode m_nodePool[MAX_PARTITION_DEPTH * MAX_PARTITION_DEPTH * MAX_PARTITION_DEPTH];
			uint32_t m_leaves[MAX_PARTITION_DEPTH * MAX_PARTITION_DEPTH * MAX_PARTITION_DEPTH] = {0};
		};
	}
}

