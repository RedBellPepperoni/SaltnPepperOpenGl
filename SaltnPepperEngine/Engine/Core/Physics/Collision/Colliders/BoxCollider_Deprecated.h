#pragma once
#include "Engine/Core/Physics/Collision/Colliders/Collider.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Physics/Collision/Colliders/ConvexHull_Deprecated.h"


namespace SaltnPepperEngine
{
	namespace Physics
	{

		class BoxCollider_Deprecated : public Collider
		{

        public:
			BoxCollider_Deprecated();
			 BoxCollider_Deprecated(const Vector3& halfDims);
			~BoxCollider_Deprecated();


			virtual std::vector<Vector3>& GetCollisionNormals(const RigidBody3D* currentBody) override;

			virtual std::vector<ColliderEdge>& GetEdgeList(const RigidBody3D* currentBody) override;


			// The Seperated Axis Theorem
			virtual void GetMinMaxFromAxis(const RigidBody3D* body, const Vector3& axis, Vector3* outMin, Vector3* outMax) override;

            virtual void GetManifoldPolygon(const RigidBody3D* currentObject, const Vector3& axis, ManifoldPolygon& manifoldPolygon) const override;

            virtual Matrix3 BuildInverseInertia(float invMass) const override;
            
            virtual void DebugDraw(const RigidBody3D* currentBody) const override;

            void SetHalfWidth(float half_width)
            {
                m_CuboidHalfDimensions.x = fabs(half_width);
                m_transform = Scale(Matrix4(1.0), m_CuboidHalfDimensions);
            }
            void SetHalfHeight(float half_height)
            {
                m_CuboidHalfDimensions.y = fabs(half_height);
                m_transform = Scale(Matrix4(1.0), m_CuboidHalfDimensions);
            }
            void SetHalfDepth(float half_depth)
            {
                m_CuboidHalfDimensions.z = fabs(half_depth);
                m_transform = Scale(Matrix4(1.0), m_CuboidHalfDimensions);
            }

            // Get Cuboid Dimensions
            float GetHalfWidth() const
            {
                return m_CuboidHalfDimensions.x;
            }
            float GetHalfHeight() const
            {
                return m_CuboidHalfDimensions.y;
            }
            float GetHalfDepth() const
            {
                return m_CuboidHalfDimensions.z;
            }

            const Vector3& GetHalfDimensions() const
            {
                return m_CuboidHalfDimensions;
            }
            void SetHalfDimensions(const Vector3& dims)
            {
                m_CuboidHalfDimensions = dims;
                m_transform = Scale(Matrix4(1.0), m_CuboidHalfDimensions);
            }

            virtual float GetSize() const override
            {
                return m_CuboidHalfDimensions.x;
            }


            template <typename Archive>
            void save(Archive& archive) const
            {
                archive(m_CuboidHalfDimensions);
            }

            template <typename Archive>
            void load(Archive& archive)
            {
                archive(m_CuboidHalfDimensions);

                m_transform = Scale(Matrix4(1.0), m_CuboidHalfDimensions);
                m_type = ColliderType::BOX;

                if (m_CubeHull->GetNumVertices() == 0)
                {
                    ConstructCubeHull();
                }
            }


        protected:
            // Constructs the static cube hull
            static void ConstructCubeHull();

        protected:
            Vector3 m_CuboidHalfDimensions;

            static SharedPtr<ConvexHull_Deprecated> m_CubeHull;

		};
	}
}

