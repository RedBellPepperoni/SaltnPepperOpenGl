#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"

#include "Engine/Core/Rendering/Buffers/VertexAttribute.h"
#include "Engine/Core/Rendering/Skybox/Skybox.h"
#include "Engine/Core/Rendering/Skybox/SkyboxObject.h"


namespace SaltnPepperEngine
{
	namespace Components
	{
		// Forward Decalrartions for better compile times
		class Transform;
	}


	using Components::Transform;


	
	

    namespace Rendering
    {
		// Forward Decalrartions for better compile times
		class Mesh;
		class Material;
		class VertexArray;
		class Shader;
		class Camera;
		class Texture;
		struct Light;

		enum class LightType : uint8_t;
		enum class MaterialType;
		enum class DrawType;


		// Data Structure to define Renderable Object properties
		struct RenderElement
		{
			// Stores the material of the current Eleemnt
			size_t materialIndex;

			// Defines which mesh is used for teh current Unit
			size_t meshIndex;

			// Define the transform/ model matrix
			Matrix4 ModelMatrix;

			// Defines the Normal matrix : used for calculating lights			
			Matrix4 NormalMatrix;
		};

		struct LightElement
		{
			std::string uniformName;
			Vector3 color;
			Vector3 position;
			Vector3 direction;

			float intensity;
			float radius;
			LightType type;
			float innerAngle;
			float outerAngle;
		};


		// Data Structure to define main camera properties
		struct CameraElement
		{

			Matrix4 viewProjMatrix;
			Matrix4 staticViewProjectMatrix;

			Vector3 viewPosition;
			SharedPtr<Texture> outputTexture;


			bool shouldRenderToTexture;
			float aspectRatio;
			bool isOrtho;
		};

		// ================ DEBUG RENDERING ELEMENTS

		struct LineVertexElement
		{
			Vector3 vertex;
			Vector4 color;

			LineVertexElement(const Vector3& inVertex, const Vector4& inColor)
				: vertex(inVertex)
				, color(inColor)
			{
			}

			bool operator==(const LineVertexElement& other) const
			{
				return vertex == other.vertex && color == other.color;
			}
		};

		struct PointVertexElement
		{
			Vector3 vertex;
			Vector4 color;
			Vector2 uv;

			bool operator==(const PointVertexElement& other) const
			{
				return vertex == other.vertex && color == other.color && uv == other.uv;
			}

		};


		struct DebugDrawData
		{
			// the Data buffer storing the Line Vertex data
			std::vector<LineVertexElement> lineDataBuffer;

			// the Data buffer storing the Point Vertex data
			std::vector<PointVertexElement> pointDataBuffer;

			std::vector<VertexAttribute> vertexLayout;

			uint32_t lineIndexCount = 0;
			uint32_t pointIndexCount = 0;

			SharedPtr<VertexBuffer> VBO;
			SharedPtr<VertexArray> VAO;

		};


		struct PipeLine
		{


			// The Vertex array for the current Pipeline

			SharedPtr<VertexArray> VAO;

			std::vector<VertexAttribute> vertexLayout;

			// All the Render Elements that need to be drawn
			std::vector<RenderElement> renderElementList;

			std::vector<LightElement> lightElementList;

			// All the Material for the current loaded Render Elements
			std::vector<SharedPtr<Material>> MaterialList;

			// All the mehses that needs to be drawn
			std::vector<SharedPtr<Mesh>> MeshList;



			// The indices of all the Opaque Shader Elements
			std::vector<size_t> opaqueElementList;

			// The indices of all the Opaque Shader Elements
			std::vector<size_t>transparentElementList;

			// properties of all the Cameras that are in the scene
			std::vector<CameraElement> cameraList;


			int textureBindIndex = 0;

			SharedPtr<Texture> defaultTextureMap;

			SkyboxObject SkyboxCubeObject;
			Skybox skybox;

		};



		class Renderer
		{




		private:


			PipeLine m_pipeline;

			DebugDrawData m_debugDrawData;


			SharedPtr<Shader> m_lineShader = nullptr;
			SharedPtr<Shader> m_pointShader = nullptr;

			std::vector<LineVertexElement*> m_LineBufferBase;
			std::vector<PointVertexElement*> m_PointBufferBase;



			void DebugPassInternal(const CameraElement& camera, bool depthtest);


			void DrawVertices(const  DrawType drawType, const uint32_t vertexCount, const size_t vertexOffset);

			void DrawIndices(const DrawType drawType, const uint32_t indexCount, const uint32_t indexOffset = 0);

			void DrawElement(const CameraElement& camera, SharedPtr<Shader>& shader, const RenderElement& element);


			void SetLightUniform(SharedPtr<Shader>& shader);


		public:

			void Init();

			void SetupDebugShaders(SharedPtr<Shader>& line, SharedPtr<Shader>& point);

			void SetSkyboxCubeMap(SharedPtr<CubeMap> cubemap);
			void SetSkyboxIntensity(float intensity);
			const float GetSkyboxIntensity() const;

			const PipeLine& GetPipeLine() const;


			// Draws the provided Elements with the provided shader
			void ForwardPass(SharedPtr<Shader> shader, const CameraElement& camera, const MaterialType type);
			void SkyBoxPass(SharedPtr<Shader> shader, const CameraElement& camera);
			void DebugPass(const CameraElement& camera);

			void SetUpCameraElement(Camera& cameraRef, Transform& transform);

			// Adds a Render Element to the Queue
			void ProcessRenderElement(const SharedPtr<Mesh>& mesh, const SharedPtr<Material>& material, Transform& transform);
			void ProcessLightElement(Light& light, Transform& transform);


			void ClearRenderCache();



		};


    }
}




#endif // !RENDERER_H
