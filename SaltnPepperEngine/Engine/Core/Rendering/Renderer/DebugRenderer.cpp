#include "DebugRenderer.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingBox.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingSphere.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingCylinder.h"
#include "Engine/Core/Rendering/Lights/Light.h"
#include "Engine/Core/Components/SceneComponents.h"
#include "Engine/Utils/Logging/Log.h"
#include <cstdarg>



namespace SaltnPepperEngine
{
	namespace Rendering
	{
		DebugRenderer* DebugRenderer::m_debugInstance = nullptr;

		static const uint32_t MaxLines = 10000;
		static const uint32_t MaxLineVertices = MaxLines * 2;
		static const uint32_t MaxLineIndices = MaxLines * 6;

#define VSNPRINTF(_DstBuf, _DstSize, _MaxCount, _Format, _ArgList) vsnprintf_s(_DstBuf, _DstSize, _MaxCount, _Format, _ArgList)



		void DebugRenderer::Init()
		{
			if (m_debugInstance)
			{
				return;
			}

			m_debugInstance = new DebugRenderer();
		}


		void DebugRenderer::Release()
		{
			delete m_debugInstance;
			m_debugInstance = nullptr;
		}


		void DebugRenderer::Reset()
		{
			m_debugInstance->m_drawList.debugLines.clear();
			m_debugInstance->m_drawList.debugPoints.clear();
			m_debugInstance->m_drawList.debugTriangles.clear();

			m_debugInstance->m_drawListAlwaysFront.debugLines.clear();
			m_debugInstance->m_drawListAlwaysFront.debugPoints.clear();
			m_debugInstance->m_drawListAlwaysFront.debugTriangles.clear();



			m_debugInstance->m_TextList.clear();
			m_debugInstance->m_TextListAlwaysFront.clear();
			m_debugInstance->m_TextListCameraSpace.clear();


		}


		DebugRenderer::DebugRenderer()
		{

		}


		DebugRenderer::~DebugRenderer()
		{


		}


		void DebugRenderer::DrawPoint(const Vector3& position, float pointradius, const Vector4& color)
		{
			GenDrawPoint(false, position, pointradius, color);
		}


		void DebugRenderer::DrawPointAlwaysFront(const Vector3& position, float pointradius, const Vector4& color)
		{
			GenDrawPoint(true, position, pointradius, color);
		}


		void DebugRenderer::DrawLine(const Vector3& startPos, const Vector3& endPos, const Vector4& color, float thickness)
		{
			GenDrawLine(false, startPos, endPos, thickness, color);
		}


		void DebugRenderer::DrawLineAlwaysFront(const Vector3& startPos, const Vector3& endPos, const Vector4& color, float thickness)
		{
			GenDrawLine(true, startPos, endPos, thickness, color);
		}

		void DebugRenderer::DrawMatrix(const Matrix3& rotationMat, const Vector3& position)
		{
			// X axis (Red Color)
			GenDrawLine(false, position, position + rotationMat[0], 1.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));

			// Y Axis (Green Color)
			GenDrawLine(false, position, position + rotationMat[1], 1.0f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));

			// X Axis (Blue Color)
			GenDrawLine(false, position, position + rotationMat[2], 1.0f, Vector4(0.0f, 0.0f, 1.0f, 1.0f));

		}


		void DebugRenderer::DrawMatrixAlwaysFront(const Matrix3& rotationMat, const Vector3& position)
		{
			// X axis (Red Color)
			GenDrawLine(true, position, position + rotationMat[0], 1.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));

			// Y Axis (Green Color)
			GenDrawLine(true, position, position + rotationMat[1], 1.0f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));

			// X Axis (Blue Color)
			GenDrawLine(true, position, position + rotationMat[2], 1.0f, Vector4(0.0f, 0.0f, 1.0f, 1.0f));

		}

		void DebugRenderer::DrawTriangle(const Vector3& vertexOne, const Vector3& vertexTwo, const Vector3& vertexThree, const Vector4& color)
		{
			GenDrawTriangle(false, vertexOne, vertexTwo, vertexThree, color);
		}

		void DebugRenderer::DrawTriangleAlwaysFront(const Vector3& vertexOne, const Vector3& vertexTwo, const Vector3& vertexThree, const Vector4& color)
		{
			GenDrawTriangle(true, vertexOne, vertexTwo, vertexThree, color);
		}


		void DebugRenderer::DrawTextWorldSpacetAlwaysFront(const Vector3& position, const float fontSize, const Vector4& color, const std::string text, ...)
		{
			va_list args;
			va_start(args, text);

			char buffer[1024];

			int needed = VSNPRINTF(buffer, 1023, _TRUNCATE, text.c_str(), args);

			va_end(args);

			int length = (needed < 0) ? 1024 : needed;

			std::string formattedtext = std::string(buffer, static_cast<size_t>(length));

			DebugText& debugText = m_debugInstance->m_TextListAlwaysFront.emplace_back();
			debugText.text = text;
			debugText.position = Vector4(position, 1.0f);
			debugText.color = color;
			debugText.size = fontSize;
		}


		void DebugRenderer::DrawTextCameraSpaceAlwaysFront(const Vector4& cameraSpaceposition, const float fontSize, const std::string text, const Vector4& color)
		{
			Vector3 cs_size = Vector3(fontSize / m_debugInstance->m_width, fontSize / m_debugInstance->m_height, 0.0f);
			cs_size = cs_size * cameraSpaceposition.w;

			// Work out the starting position of text based off desired alignment
			float x_offset = 0.0f;
			const auto text_len = static_cast<int>(text.length());

			DebugText& debugText = m_debugInstance->m_TextListCameraSpace.emplace_back();
			debugText.text = text;
			debugText.position = cameraSpaceposition;
			debugText.color = color;
			debugText.size = fontSize;
		}

		void DebugRenderer::DebugDraw(const BoundingBox& box, const Vector4& color, bool cornersOnly, float width)
		{
			Vector3 toptoptop = box.Max();
			Vector3 botbotbot = box.Min();

			Vector3 topbotbot(toptoptop.x, botbotbot.y, botbotbot.z);
			Vector3 toptopbot(toptoptop.x, toptoptop.y, botbotbot.z);
			Vector3 topbottop(toptoptop.x, botbotbot.y, toptoptop.z);

			Vector3 bottoptop(botbotbot.x, toptoptop.y, toptoptop.z);
			Vector3 botbottop(botbotbot.x, botbotbot.y, toptoptop.z);
			Vector3 bottopbot(botbotbot.x, toptoptop.y, botbotbot.z);

			if (!cornersOnly)
			{

				DrawLineAlwaysFront(bottoptop, toptoptop, color, width);
				DrawLineAlwaysFront(bottopbot, toptopbot, color, width);
				DrawLineAlwaysFront(botbottop, topbottop, color, width);
				DrawLineAlwaysFront(botbotbot, topbotbot, color, width);


				DrawLineAlwaysFront(bottopbot, botbotbot, color, width);
				DrawLineAlwaysFront(toptopbot, topbotbot, color, width);
				DrawLineAlwaysFront(bottoptop, botbottop, color, width);
				DrawLineAlwaysFront(toptoptop, topbottop, color, width);


				DrawLineAlwaysFront(botbotbot, botbottop, color, width);
				DrawLineAlwaysFront(topbotbot, topbottop, color, width);
				DrawLineAlwaysFront(bottopbot, bottoptop, color, width);
				DrawLineAlwaysFront(toptopbot, toptoptop, color, width);

			}

			else
			{
				DrawLineAlwaysFront(bottoptop, bottoptop + (toptoptop - bottoptop) * 0.25f, color, width);
				DrawLineAlwaysFront(bottoptop + (toptoptop - bottoptop) * 0.75f, toptoptop, color, width);

				DrawLineAlwaysFront(bottopbot, bottopbot + (toptopbot - bottopbot) * 0.25f, color, width);
				DrawLineAlwaysFront(bottopbot + (toptopbot - bottopbot) * 0.75f, toptopbot, color, width);

				DrawLineAlwaysFront(botbottop, botbottop + (topbottop - botbottop) * 0.25f, color, width);
				DrawLineAlwaysFront(botbottop + (topbottop - botbottop) * 0.75f, topbottop, color, width);

				DrawLineAlwaysFront(botbotbot, botbotbot + (topbotbot - botbotbot) * 0.25f, color, width);
				DrawLineAlwaysFront(botbotbot + (topbotbot - botbotbot) * 0.75f, topbotbot, color, width);

				DrawLineAlwaysFront(bottopbot, bottopbot + (botbotbot - bottopbot) * 0.25f, color, width);
				DrawLineAlwaysFront(bottopbot + (botbotbot - bottopbot) * 0.75f, botbotbot, color, width);

				DrawLineAlwaysFront(toptopbot, toptopbot + (topbotbot - toptopbot) * 0.25f, color, width);
				DrawLineAlwaysFront(toptopbot + (topbotbot - toptopbot) * 0.75f, topbotbot, color, width);

				DrawLineAlwaysFront(bottoptop, bottoptop + (botbottop - bottoptop) * 0.25f, color, width);
				DrawLineAlwaysFront(bottoptop + (botbottop - bottoptop) * 0.75f, botbottop, color, width);

				DrawLineAlwaysFront(toptoptop, toptoptop + (topbottop - toptoptop) * 0.25f, color, width);
				DrawLineAlwaysFront(toptoptop + (topbottop - toptoptop) * 0.75f, topbottop, color, width);

				DrawLineAlwaysFront(botbotbot, botbotbot + (botbottop - botbotbot) * 0.25f, color, width);
				DrawLineAlwaysFront(botbotbot + (botbottop - botbotbot) * 0.75f, botbottop, color, width);

				DrawLineAlwaysFront(topbotbot, topbotbot + (topbottop - topbotbot) * 0.25f, color, width);
				DrawLineAlwaysFront(topbotbot + (topbottop - topbotbot) * 0.75f, topbottop, color, width);

				DrawLineAlwaysFront(bottopbot, bottopbot + (bottoptop - bottopbot) * 0.25f, color, width);
				DrawLineAlwaysFront(bottopbot + (bottoptop - bottopbot) * 0.75f, bottoptop, color, width);

				DrawLineAlwaysFront(toptopbot, toptopbot + (toptoptop - toptopbot) * 0.25f, color, width);
				DrawLineAlwaysFront(toptopbot + (toptoptop - toptopbot) * 0.75f, toptoptop, color, width);
			}


		}




		void DebugRenderer::DebugDraw(const BoundingSphere& sphere, const Vector4& color)
		{
			DrawPointAlwaysFront(sphere.GetCenter(), sphere.GetRadius(), color); 
		}


		void DebugRenderer::DebugDraw(LightComponent& lightcomp, const Quaternion& rotation, const Vector4& color)
		{
			Light* light = lightcomp.GetLightData();

			switch (light->type)
			{
			case LightType::DirectionLight:

				Vector3 offset(0.0f, 0.1f, 0.0f);
				DrawLine(Vector3(light->position) + offset, Vector3(light->position + (light->direction) * 2.0f) + offset, color);
				DrawLine(Vector3(light->position) - offset, Vector3(light->position + (light->direction) * 2.0f) - offset, color);

				DrawLine(Vector3(light->position), Vector3(light->position + (light->direction) * 2.0f), color);
				DebugDrawCone(20, 4, 30.0f, 1.5f, (light->position - (light->direction) * 1.5f), rotation, color);

				break;
			case LightType::SpotLight:

				DebugDrawCone(20, 4, Degrees(light->innerAngle), light->intensity, light->position, rotation, color);

				break;
			case LightType::PointLight:

				DebugDrawSphere(light->radius, light->position, color);

				break;
			default:
				break;
			}

		}

		void DebugRenderer::DebugDraw(const BoundingCylinder& cylinder, const Vector4& color)
		{
			Vector3 up = Vector3(0.0f, 1.0f, 0.0f);

			Vector3 position = cylinder.center;

			Vector3 topCircleCentre = position  + cylinder.height * 0.5f * up;
			Vector3 bottomSphereCentre = position - up * (cylinder.height * 0.5f);

			DebugDrawCircle(8, cylinder.radiusX, topCircleCentre,  Quaternion(Vector3(Radians(90.0f), 0.0f, 0.0f)), color);
			DebugDrawCircle(8, cylinder.radiusX, bottomSphereCentre,  Quaternion(Vector3(Radians(90.0f), 0.0f, 0.0f)), color);


			float step = 360.0f / float(8);
			for (int i = 0; i < 8; i++)
			{
				float z = Cos(step * i) * cylinder.radiusX;
				float x = Sin(step * i) * cylinder.radiusX;

				Vector3 offset = Vector4(x, 0.0f, z, 0.0f);
				DrawLine(bottomSphereCentre + offset, topCircleCentre + offset, color);

				
			}
		}


		void DebugRenderer::DebugDrawSphere(float radius, const Vector3& position, const Vector4& color)
		{

			DebugDrawCircle(20, radius, position, Quaternion(Vector3(0.0f, 0.0f, 0.0f)), color);
			DebugDrawCircle(20, radius, position, Quaternion(Vector3(90.0f, 0.0f, 0.0f)), color);
			DebugDrawCircle(20, radius, position, Quaternion(Vector3(0.0f, 90.0f, 90.0f)), color);
		}


		void DebugRenderer::DebugDrawCircle(int numVertices, float radius, const Vector3& position, const Quaternion& rotation, const Vector4& color)
		{
			float step = 360.0f / float(numVertices);

			for (int i = 0; i < numVertices; i++)
			{
				float cx = Cos(step * i) * radius;
				float cy = Sin(step * i) * radius;
				Vector3 current = Vector3(cx, cy, 0.0f);

				float nx = Cos(step * (i + 1)) * radius;
				float ny = Sin(step * (i + 1)) * radius;
				Vector3 next = Vector3(nx, ny, 0.0f);



				DrawLine(position + (rotation * current), position + (rotation * next), color);
			}
		}


		void DebugRenderer::DebugDrawCone(int numCircleVertices, int numCircleLines, float angle, float length, const Vector3& position, const Quaternion& rotation, const Vector4& color)
		{
			float endAngle = Tan(angle * 0.5f) * length;
			Vector3 forward = -(rotation * Vector3(0.0f, 0.0f, -1.0f));
			Vector3 endPosition = position + forward * length;
			float offset = 0.0f;
			DebugDrawCircle(numCircleVertices, endAngle, endPosition, rotation, color);

			for (int i = 0; i < numCircleLines; i++)
			{
				float a = i * 90.0f;
				Vector3 point = rotation * Vector3(Cos(a), Sin(a), 0.0f) * endAngle;
				DrawLine(position, position + point + forward * length, color);
			}
		}

		void DebugDrawArc(int numVerts, float radius, const Vector3& start, const Vector3& end, const Quaternion& rotation, const Vector4& color)
		{

			float step = 180.0f / numVerts;
			Quaternion rot = GetViewMatrix(rotation * start, rotation * end, Vector3(0.0f, 1.0f, 0.0f));
			rot = rotation * rot;

			Vector3 arcCentre = (start + end) * 0.5f;
			for (int i = 0; i < numVerts; i++)
			{
				float cx = Cos(step * i) * radius;
				float cy = Sin(step * i) * radius;
				Vector3 current = Vector3(cx, cy, 0.0f);

				float nx = Cos(step * (i + 1)) * radius;
				float ny = Sin(step * (i + 1)) * radius;
				Vector3 next = Vector3(nx, ny, 0.0f);

				DebugRenderer::DrawLine(arcCentre + (rot * current), arcCentre + (rot * next), color);
			}
		}

		void DebugRenderer::DebugDrawCapsule(const Vector3& position, const Quaternion& rotation, float height, float radius, const Vector4& color)
		{

			Vector3 up = (rotation * Vector3(0.0f, 1.0f, 0.0f));

			Vector3 topSphereCentre = position + up * (height * 0.5f);
			Vector3 bottomSphereCentre = position - up * (height * 0.5f);

			DebugDrawCircle(8, radius, topSphereCentre, rotation * Quaternion(Vector3(Radians(90.0f), 0.0f, 0.0f)), color);
			DebugDrawCircle(8, radius, bottomSphereCentre, rotation * Quaternion(Vector3(Radians(90.0f), 0.0f, 0.0f)), color);

			// Draw 10 arcs
			// Sides
			float step = 360.0f / float(8);
			for (int i = 0; i < 8; i++)
			{
				float z = Cos(step * i) * radius;
				float x = Sin(step * i) * radius;

				Vector3 offset = rotation * Vector4(x, 0.0f, z, 0.0f);
				DrawLine(bottomSphereCentre + offset, topSphereCentre + offset, color);

				if (i < 4)
				{
					float z2 = Cos(step * (i + 4)) * radius;
					float x2 = Sin(step * (i + 4)) * radius;

					Vector3 offset2 = rotation * Vector4(x2, 0.0f, z2, 0.0f);
					// Top Hemishpere
					DebugDrawArc(8, radius, topSphereCentre + offset, topSphereCentre + offset2, rotation, color);
					// Bottom Hemisphere
					DebugDrawArc(8, radius, bottomSphereCentre + offset, bottomSphereCentre + offset2, rotation * Quaternion(Vector3(Radians(180.0f), 0.0f, 0.0f)), color);
				}
			}

		}

		const std::vector<DebugTriangleData>& DebugRenderer::GetTriangles(bool depthtested) const
		{
			return depthtested ? m_drawList.debugTriangles : m_drawListAlwaysFront.debugTriangles;
		}

		const std::vector<DebugLineData>& DebugRenderer::GetLines(bool depthtested) const
		{
			return depthtested ? m_drawList.debugLines : m_drawListAlwaysFront.debugLines;
		}

		const std::vector<DebugPointData>& DebugRenderer::GetPoints(bool depthtested) const
		{
			return depthtested ? m_drawList.debugPoints : m_drawListAlwaysFront.debugPoints;
		}

		const std::vector<DebugText>& DebugRenderer::GetDebugText() const
		{
			return m_TextList;
		}

		const std::vector<DebugText>& DebugRenderer::GetDebugTextAlwaysFront() const
		{
			return m_TextListAlwaysFront;
		}

		const std::vector<DebugText>& DebugRenderer::GetDebugTextCameraSpace() const
		{
			return m_TextListCameraSpace;
		}




		// ================ Drawing Functions ====================


		void DebugRenderer::GenDrawPoint(bool ignoreDepthtest, const Vector3& position, float radius, const Vector4& color)
		{
			if (ignoreDepthtest)
			{
				m_debugInstance->m_drawListAlwaysFront.debugPoints.emplace_back(position, radius, color);
			}
			else
			{
				m_debugInstance->m_drawList.debugPoints.emplace_back(position, radius, color);
			}

		}

		void DebugRenderer::GenDrawLine(bool ignoreDepthtest, const Vector3& startPos, const Vector3& endPos, float width, const Vector4& color)
		{



			if (ignoreDepthtest)
			{
				m_debugInstance->m_drawListAlwaysFront.debugLines.emplace_back(startPos, endPos, color, width);


			}
			else
			{
				m_debugInstance->m_drawList.debugLines.emplace_back(startPos, endPos, color, width);
			}
		}



		void DebugRenderer::GenDrawTriangle(bool ignoreDepthtest, const Vector3& vertOne, const Vector3& vertTwo, const Vector3& vertThree, const Vector4& color)
		{
			if (ignoreDepthtest)
			{
				m_debugInstance->m_drawListAlwaysFront.debugTriangles.emplace_back(vertOne, vertTwo, vertThree, color);
			}
			else
			{
				m_debugInstance->m_drawList.debugTriangles.emplace_back(vertOne, vertTwo, vertThree, color);
			}

		}


		void DebugRenderer::ClearInternal()
		{

		}
	}
}

