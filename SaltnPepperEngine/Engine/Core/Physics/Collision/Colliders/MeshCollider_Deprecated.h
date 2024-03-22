#pragma once
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Physics/Collision/Colliders/Collider.h"
#include "Engine/Core/Physics/Collision/Colliders/ConvexHull_Deprecated.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingBox.h"

namespace SaltnPepperEngine
{
	using Rendering::Mesh;

	namespace Physics
	{
		// Stolen from Mr Feeney's Code
		struct Triangle
		{
			static const unsigned int NUMBER_OF_TRIANGLE_VERTICES = 3;

			Triangle()
			{
				vertices[0] = Vector3(0.0f);
				vertices[1] = Vector3(0.0f);
				vertices[2] = Vector3(0.0f);
			}

			Triangle(Vector3 v0, Vector3 v1, Vector3 v2)
			{
				vertices[0] = v0;
				vertices[1] = v1;
				vertices[2] = v2;
				ProcessNormal();
			}

			Triangle(Vector3 vertices[NUMBER_OF_TRIANGLE_VERTICES])
			{
				vertices[0] = vertices[0];
				vertices[1] = vertices[1];
				vertices[2] = vertices[2];
				ProcessNormal();
			}
			
			void ProcessNormal(void)
			{
				Vector3 v0_1 = vertices[1] - vertices[0];
				Vector3 v0_2 = vertices[2] - vertices[1];
				normal = Normalize(Cross(v0_1, v0_2));
				return;
			}
			
			Vector3 getCentre(void)
			{
				return ((vertices[0] + vertices[1] + vertices[2]) / (float)NUMBER_OF_TRIANGLE_VERTICES);
			}

			// ================= Variables ========

			Vector3 vertices[NUMBER_OF_TRIANGLE_VERTICES];
			Vector3 normal = Vector3(0.0f);

		};




		class MeshCollider_Deprecated : public Collider
		{
		public:


			MeshCollider_Deprecated();
			~MeshCollider_Deprecated();

			void BuildFromMesh(Mesh* mesh);

			virtual std::vector<Vector3>& GetCollisionNormals(const RigidBody3D* body) override;
			virtual std::vector<ColliderEdge>& GetEdgeList(const RigidBody3D* body) override;

			virtual void GetMinMaxFromAxis(const RigidBody3D* body, const Vector3& axis, Vector3* outMin, Vector3* outMax) override;
			

			virtual void GetManifoldPolygon(const RigidBody3D* currentObject, const Vector3& axis, ManifoldPolygon& manifoldPolygon) const override;


			virtual Matrix3 BuildInverseInertia(float invMass) const override;

			virtual void DebugDraw(const RigidBody3D* currentBody) const override;

			// Set Cuboid Dimensions
			void SetHalfWidth(float half_width)
			{
				m_HalfDimensions.x = fabs(half_width);
				m_transform = Scale(Matrix4(1.0), m_HalfDimensions);
			}
			void SetHalfHeight(float half_height)
			{
				m_HalfDimensions.y = fabs(half_height);
				m_transform = Scale(Matrix4(1.0), m_HalfDimensions);
			}
			void SetHalfDepth(float half_depth)
			{
				m_HalfDimensions.z = fabs(half_depth);
				m_transform = Scale(Matrix4(1.0), m_HalfDimensions);
			}

			float GetHalfWidth() const
			{
				return m_HalfDimensions.x;
			}
			float GetHalfHeight() const
			{
				return m_BoundingBox.Size().y;
			}
			float GetHalfDepth() const
			{
				return m_BoundingBox.Size().z;
			}

			Vector3 GetHalfDimensions() const
			{
				return m_BoundingBox.Size();
			}
			void SetHalfDimensions(const Vector3& dims)
			{
				m_HalfDimensions = dims;
				m_transform = Scale(Matrix4(1.0), m_HalfDimensions);
			}

			virtual float GetSize() const override
			{
				return m_BoundingBox.Size().x;
			}


			template <typename Archive>
			void save(Archive& archive) const
			{
				archive(m_HalfDimensions);
			}

			template <typename Archive>
			void load(Archive& archive)
			{
				archive(m_HalfDimensions);

				m_transform = glm::scale(glm::mat4(1.0), m_HalfDimensions);
				m_type = ColliderType::MESH;
			}


		protected:
			Vector3 m_HalfDimensions;
			BoundingBox m_BoundingBox;

			SharedPtr<ConvexHull_Deprecated> m_Hull;
		};

	}
}


