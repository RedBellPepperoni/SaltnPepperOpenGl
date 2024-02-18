#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H

#include <string>
#include <vector>
#include "Engine/Utils/Maths/MathDefinitions.h"

namespace SaltnPepperEngine
{

	class BoundingBox;
	class BoundingSphere;
	class Scene;

	namespace Rendering
	{
		class Renderer;
		class RenderManager;
        class BaseLight;

        struct DebugLineData
        {
            Vector3 posOne;
            Vector3 posTwo;
            Vector4 color;
            float thickness;

            DebugLineData() = default;

            DebugLineData(const Vector3& inPosOne, const Vector3& inPosTwo, const Vector4& inColor, const float inthickness)
            {
                posOne = inPosOne;
                posTwo = inPosTwo;
                color = inColor;
                thickness = inthickness;
            }
        };

        struct DebugPointData
        {
            Vector3 posOne;
            Vector4 color;
            float size;

            DebugPointData(const Vector3& inPosOne, float inRadius, const Vector4& inColor)
            {
                posOne = inPosOne;
                color = inColor;
                size = inRadius;

            }
        };

        struct DebugTriangleData
        {
            Vector3 posOne;
            Vector3 posTwo;
            Vector3 posThree;
            Vector4 color;

            DebugTriangleData(const Vector3& inPosOne, const Vector3& inPosTwo, const Vector3& inPosThree, const Vector4& inColor)
            {
                posOne = inPosOne;
                posTwo = inPosTwo;
                posThree = inPosThree;
                color = inColor;
            }
        };

        struct DebugText
        {
            std::string text;
            Vector4 position;
            Vector4 color;
            float size;
        };


        class DebugRenderer
        {
            friend class Scene;
            friend class Renderer;
            friend class RendermMnager;

        public:

            static void Init();
            static void Release();
            static void Reset();

            DebugRenderer();
            ~DebugRenderer();


            // ================= DEBUG POINT ================================

            // Actually Draws a small circle instead of a point 
            // Draws the point in the defulat rendering (with depth testing)
            static void DrawPoint(const Vector3& position, float pointradius, const Vector4& color = Vector4(1.0f));

            // Draws the Points in front of everything (Depth test is ignored)
            static void DrawPointAlwaysFront(const Vector3& position, float pointradius, const Vector4& color = Vector4(1.0f));


            // ==================== DEBUG LINE =============================

            // Draws a line with the provided thickness (or draws a line of 1pixel thickness if thickness is not specified)
            static void DrawLine(const Vector3& startPos, const Vector3& endPos, const Vector4& color = Vector4(1.0f), float thickness = 1.0f);

            // Draws a line that is always rendered first (on top of eveything ) (Depth test is ignored)
            static void DrawLineAlwaysFront(const Vector3& startPos, const Vector3& endPos, const Vector4& color = Vector4(1.0f), float thickness = 1.0f);


            // ====================== DEBUG TRANSFORM ============================

            // Draws a matrix (x,y ,z at the give nposition)
            static void DrawMatrix(const Matrix3& rotationMat, const Vector3& position);

            //Draws a matrix that is rendered in the front (no depth test)
            static void DrawMatrixAlwaysFront(const Matrix3& rotationMat, const Vector3& position);



            // ====================== DEBUG TRIANGLE ===================================

            // Draws a triangle
            static void DrawTriangle(const Vector3& vertexOne, const Vector3& vertexTwo, const Vector3& vertexThree, const Vector4& color = Vector4(1.0f));
            static void DrawTriangleAlwaysFront(const Vector3& vertexOne, const Vector3& vertexTwo, const Vector3& vertexThree, const Vector4& color = Vector4(1.0f));



            // ====================== WORLD SPACE TEXT ==================================

            // uses Printf for args
            static void DrawTextWorldSpacetAlwaysFront(const Vector3& position, const float fontSize, const Vector4& color, const std::string text, ...);

            static void DrawTextCameraSpaceAlwaysFront(const Vector4& cameraSpaceposition, const float fontSize, const std::string text, const Vector4& color = Vector4(1.0f));





            // ======================= DRAW FUNCTION ==========================

            static void DebugDraw(const BoundingBox& box, const Vector4& color, bool cornersOnly = false, float width = 0.02f);

            static void DebugDraw(const BoundingSphere& sphere, const Vector4& color);

            static void DebugDraw(const BaseLight& light, const Quaternion& rotation, const Vector4& color);

            //static void DebugDraw()


            static void DebugDrawSphere(float radius, const Vector3& position, const Vector4& color);
            static void DebugDrawCircle(int numVertices, float radius, const Vector3& position, const Quaternion& rotation, const Vector4& color);
            static void DebugDrawCone(int numCircleVertices, int numCircleLines, float angle, float length, const Vector3& position, const Quaternion& rotation, const Vector4& color);
            static void DebugDrawCapsule(const Vector3& position, const Quaternion& rotation, float height, float radius, const Vector4& color);


            const std::vector<DebugTriangleData>& GetTriangles(bool depthtested = false) const;
            const std::vector<DebugLineData>& GetLines(bool depthtested = false) const;
            const std::vector<DebugPointData>& GetPoints(bool depthtested = false) const;

            const std::vector<DebugText>& GetDebugText() const;
            const std::vector<DebugText>& GetDebugTextAlwaysFront() const;
            const std::vector<DebugText>& GetDebugTextCameraSpace() const;


            static DebugRenderer* Get()
            {
                return m_debugInstance;
            }

        protected:


            static void GenDrawPoint(bool ignoreDepthtest, const Vector3& position, float radius, const Vector4& color);
            static void GenDrawLine(bool ignoreDepthtest, const Vector3& startPos, const Vector3& endPos, float width, const Vector4& color);
            static void GenDrawTriangle(bool ignoreDepthtest, const Vector3& vertOne, const Vector3& vertTwo, const Vector3& vertThree, const Vector4& color);

        private:


            void ClearInternal();

            static DebugRenderer* m_debugInstance;


            struct DebugDrawList
            {
                std::vector<DebugLineData> debugLines;
                std::vector<DebugPointData> debugPoints;
                std::vector<DebugTriangleData> debugTriangles;
            };


            std::vector<DebugText> m_TextList;
            std::vector<DebugText> m_TextListAlwaysFront;
            std::vector<DebugText> m_TextListCameraSpace;

            size_t m_offsetChars;

            DebugDrawList m_drawList;
            DebugDrawList m_drawListAlwaysFront;


            Matrix4 m_projVoewMatrix = Matrix4(1.0f);

            uint32_t m_width;
            uint32_t m_height;
        };

	}


}



#endif // !DEBUGRENDERER_H
