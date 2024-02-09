#pragma once
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/Plane.h"
#include "Engine/Utils/Logging/Log.h"
#include <vector>
#include <cereal/cereal.hpp>

namespace SaltnPepperEngine
{
	namespace Physics
	{

		class RigidBody3D;

		// this is the Edge for the Collison Shape that will be used for detection 
		struct ColliderEdge
		{
			ColliderEdge(const Vector3& firstPoint = Vector3(0.0f), const Vector3& secondPoint = Vector3(0.0f))
				: firstPosition(firstPoint)
				, secondPosition(secondPoint)
			{

			}


			Vector3 firstPosition;
			Vector3 secondPosition;
			
		};

		struct ManifoldPolygon
		{
			Vector3 Faces[8];
			Plane AdjacentPlanes[8];
			Vector3 Normal;
			uint32_t FaceCount = 0;
			uint32_t PlaneCount = 0;
		};


		enum ColliderType : uint8_t
		{
			NONE = 1 << 0,				// 0
			BOX = 1 << 1,	// 1
			SPHERE = 1 << 2,	// 2
			CAPSULE = 1 << 3,	// 4
			MESH = 1 << 4,		// 8
			MAX = 1 << 5			//16

		};

		class Collider
		{
		public:

			Collider();


			virtual ~Collider();

			void SetTransform(const Matrix4& transform);

			virtual float GetSize() const = 0;



			virtual std::vector<Vector3>& GetCollisionNormals(const RigidBody3D* currentBody) = 0;

			virtual std::vector<ColliderEdge>& GetEdgeList(const RigidBody3D* currentBody) = 0;


			// The Seperated Axis Theorem
			virtual void GetMinMaxFromAxis(const RigidBody3D* body, const Vector3& axis, Vector3* outMin, Vector3* outMax) = 0;

			virtual void GetManifoldPolygon(const RigidBody3D* currentObject,const Vector3& axis, ManifoldPolygon& manifoldPolygon) const = 0;


			virtual Matrix3 BuildInverseInertia(float invMass) const = 0;

			ColliderType GetType() const;

			virtual void DebugDraw(const RigidBody3D* currentBody) const = 0;


			template <class Archive>
			void load(Archive& archive)
			{
				LOG_TRACE("Loading abstract CollisionShape");
			}

			template <class Archive>
			void save(Archive& archive) const
			{
				LOG_TRACE("Serialising abstract CollisionShape");
			}


		protected:

			// type of collider
			ColliderType m_type;

			// The local transform matrix
			Matrix4 m_transform;

			// Conatiner to store all the Colliding edge data
			std::vector<ColliderEdge> m_edgelist;

			// All the face normals used for detection
			std::vector<Vector3> m_normallist;

		};
	}
}

