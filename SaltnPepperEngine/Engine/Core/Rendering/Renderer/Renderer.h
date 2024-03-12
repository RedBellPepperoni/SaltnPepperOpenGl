#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"

#include "Engine/Core/Rendering/Buffers/VertexAttribute.h"
#include "Engine/Core/Rendering/Skybox/Skybox.h"
#include "Engine/Core/Rendering/Skybox/SkyboxObject.h"
#include "Engine/Core/Rendering/Buffers/FrameBuffer.h"
#include "Engine/Core/Rendering/Buffers/ShaderStorageBuffer.h"

#include "Engine/Core/Rendering/Geometry/RectangleObject.h"

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
		class ComputeShader;

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
			// The Shader Uniform name
			std::string uniformName;

			// Color of the Light 
			Vector3 color;

			// POsition of the transform component
			Vector3 position;

			// Incase of directional/spot light, the forwward vector
			Vector3 direction;

			// How bright the light is
			float intensity;

			// Incase of Point light , the light effective radius
			float radius;

			// the actual type of light
			LightType type;

			// Spot light inner and outer angles
			float innerAngle;
			float outerAngle;
		};


		// Data Structure to define main camera properties
		struct CameraElement
		{
			// The frame buffer of the particluar camera
			SharedPtr<FrameBuffer> gBuffer;

			// Rendering Textures
			SharedPtr<Texture> albedoTexture;
			SharedPtr<Texture> normalTexture;
			SharedPtr<Texture> materialTexture;
			SharedPtr<Texture> depthTexture;

			// Buffer Textures
			SharedPtr<Texture> swapTextureOne;
			SharedPtr<Texture> swapTextureTwo;

			// the premultiplied View ProjectionMatrix
			Matrix4 viewProjMatrix;

			// Static view proj matric for the Skybox rendering
			Matrix4 staticViewProjectMatrix;

			// the position of the camera
			Vector3 viewPosition;

			// The final render Texture
			SharedPtr<Texture> outputTexture;

			// Defines if the camera should render at all
			bool shouldRenderToTexture;

			// Camera Details
			float aspectRatio;
			bool isOrtho;
		};


		struct ParticleElement
		{
			
			Matrix4 transform;
			float particleLifetime;
			float fading;
			size_t invocationCount;
			size_t materialIndex;
			bool isRelative;
			SharedPtr<ShaderStorageBuffer> SSBO;
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

			RectangleObject rectangularObject;

			// All the Render Elements that need to be drawn
			std::vector<RenderElement> renderElementList;

			std::vector<LightElement> lightElementList;

			// All the Material for the current loaded Render Elements
			std::vector<SharedPtr<Material>> MaterialList;

			// All the mehses that needs to be drawn
			std::vector<SharedPtr<Mesh>> MeshList;



			// The indices of all the Opaque Shader Elements
			std::vector<size_t> opaqueElementList;

			// The indices of all the Transparent Shader Elements
			std::vector<size_t> transparentElementList;

			// The indices of all the Custom Shader Elements
			std::vector<size_t> customElementList;

			// properties of all the Cameras that are in the scene
			std::vector<CameraElement> cameraList;


			int textureBindIndex = 0;

			SharedPtr<Texture> defaultTextureMap;

			SkyboxObject SkyboxCubeObject;
			Skybox skybox;



			SharedPtr<FrameBuffer> postprocessFrameBuffer;
			SharedPtr<FrameBuffer> depthFrameBuffer;
			SharedPtr<FrameBuffer> bloomFrameBuffer;

		};

		struct ShadowInformation
		{

		};



		class Renderer
		{




		private:


			PipeLine m_pipeline;

			Vector2Int viewPort;
			DebugDrawData m_debugDrawData;

			SharedPtr<Shader> m_lineShader = nullptr;
			SharedPtr<Shader> m_pointShader = nullptr;

			std::vector<LineVertexElement*> m_LineBufferBase;
			std::vector<PointVertexElement*> m_PointBufferBase;

			
			
			

			unsigned int clearMask = 0;


		public:

			/*SharedPtr<FrameBuffer> SecondaryFrameBuffer;
			SharedPtr<Texture> SecondaryTexture;*/
			//unsigned int rbo;

		private:

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

			void BindSkyBoxInformation(const CameraElement& camera, SharedPtr<Shader>& shader, int textureBindId);
			void BindCameraInformation(const CameraElement& camera, SharedPtr<Shader>& shader);
			//const PipeLine& GetPipeLine() const;
			PipeLine& GetPipeLine();

			// Draws the provided Elements with the provided shader


			void ObjectPass(SharedPtr<Shader> shader, const CameraElement& camera, std::vector<size_t>& elementList);



			void SkyBoxPass(SharedPtr<Shader> shader, const CameraElement& camera);
			void DebugPass(const CameraElement& camera);

			void SetUpCameraElement(Camera& cameraRef, Transform& transform);
			CameraElement GenerateCameraElement(Camera& cameraRef, Transform& transform);

			// Adds a Render Element to the Queue
			void ProcessRenderElement(const SharedPtr<Mesh>& mesh, const SharedPtr<Material>& material, Transform& transform);
			void ProcessLightElement(Light& light, Transform& transform);
			void RenderScreenQuad(SharedPtr<Shader> shader, const SharedPtr<Texture>& texture, int lod = 0);


			// Particle Stuff
			void ComputeParticles(std::vector<ParticleElement>& particleList, SharedPtr<ComputeShader>& computeShader);
			void SortParticles(const CameraElement& camera, std::vector<ParticleElement>& particleList);
			void DrawParticles(const CameraElement& camera, std::vector<ParticleElement>& particleList, SharedPtr<Shader> shader);

			SharedPtr<FrameBuffer>& GetPostProcessFrameBuffer();



			void RenderToAttachedFrameBuffer(const SharedPtr<Shader>& shader);


			void Clear(bool clearDepth = false);
			void ClearRenderCache();

			void SetViewport(int x, int y, int width, int height) const;
			Vector2Int GetViewport();
			void SetWindowSize(const Vector2Int& size);


			void AttachDefaultFrameBuffer();
			void ClearRectangleObjectVAO();

		};


    }
}




#endif // !RENDERER_H
