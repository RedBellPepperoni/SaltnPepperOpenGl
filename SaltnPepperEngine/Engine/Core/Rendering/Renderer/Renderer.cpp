#include "Renderer.h"
#include "Engine/Core/System/Window/Window.h"
#include "Engine/Utils/Logging/Log.h"

#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Rendering/Shader/Shader.h"
#include "Engine/Core/Rendering/Buffers/VertexArray.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"
#include "Engine/Core/Rendering/Camera/Camera.h"

#include "Engine/Core/Resources/ResourceManager.h"
#include "Engine/Core/Components/Transform.h"


#include "Engine/Core/Rendering/Lights/Light.h"
#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"

#include "Engine/Core/Rendering/Textures/CubeMap.h"

namespace SaltnPepperEngine
{

	namespace Rendering
	{
        static const uint32_t MaxPoints = 10000;
        static const uint32_t MaxPointVertices = MaxPoints * 4;
        static const uint32_t MaxPointIndices = MaxPoints * 6;
        static const uint32_t MAX_BATCH_DRAW_CALLS = 100;
        static const uint32_t RENDERER_POINT_SIZE = sizeof(PointVertexElement) * 4;
        static const uint32_t RENDERER_POINT_BUFFER_SIZE = RENDERER_POINT_SIZE * MaxPointVertices;

        static const uint32_t MaxLines = 10000;
        static const uint32_t MaxLineVertices = MaxLines * 2;
        static const uint32_t MaxLineIndices = MaxLines * 6;
        static const uint32_t MAX_LINE_BATCH_DRAW_CALLS = 100;
        static const uint32_t RENDERER_LINE_SIZE = sizeof(LineVertexElement) * 4;
        static const uint32_t RENDERER_LINE_BUFFER_SIZE = RENDERER_LINE_SIZE * MaxLineVertices;

        static const uint32_t MAX_QUADS = 50000;

        enum class DrawType
        {
            POINTS = GL_POINTS,
            LINES = GL_LINES,
            TRIANGLES = GL_TRIANGLES
        };


        void Renderer::DrawVertices(const DrawType drawType, const uint32_t vertexOffset, const size_t vertexCount)
        {
            // Drawing all the vertices in tthe Vertex buffer
            // Hacky way to convert long long int to GLsizei which is somw how a vanilla int
            GLDEBUG(glDrawArrays((GLenum)drawType, (GLint)vertexOffset, (GLsizei)vertexCount));
            //glDrawArrays(GL_TRIANGLES, (GLint)vertexOffset, (GLsizei)vertexCount);

        }

        void Renderer::DrawIndices(const DrawType drawType, const uint32_t indexCount, const uint32_t indexOffset)
        {
            // Increase Drawcalls here
            // Drawing the Indices
            GLDEBUG(glDrawElements((GLenum)drawType, indexCount, GL_UNSIGNED_INT, nullptr));

        }



        void Renderer::Init()
        {

            clearMask |= GL_COLOR_BUFFER_BIT;

            // Initialize the Debug Renderer
            DebugRenderer::Init();

            // Creating a new Vertex Array Object foe the Pipeline
            m_pipeline.VAO = Factory<VertexArray>::Create();
            m_debugDrawData.VAO = Factory<VertexArray>::Create();

            m_debugDrawData.VBO = Factory<VertexBuffer>::Create(UsageType::DYNAMIC_DRAW);




            // Hard coding for now
            // Realistically this should be set up for each shader when the shader gets compiled , probably?
            m_pipeline.vertexLayout =
            {
                VertexAttribute::Attribute<Vector3>(), // position
                VertexAttribute::Attribute<Vector2>(), // texture uv
                VertexAttribute::Attribute<Vector3>(), // normal
                VertexAttribute::Attribute<Vector3>(), // tangent
                VertexAttribute::Attribute<Vector3>(), // bitangent
            };


            m_debugDrawData.vertexLayout =
            {
                VertexAttribute::Attribute<Vector3>(), // Position
                VertexAttribute::Attribute<Vector4>(), // Color
            };


            m_debugDrawData.VBO->Bind();
            m_debugDrawData.VAO->Bind();
            m_debugDrawData.VAO->AddVertexAttribLayout(m_debugDrawData.vertexLayout);



            // SharedPtr<TextureLibrary> lib = Application::GetCurrent().GetTextureLibrary();

            // ============== LOADING TEXTURES =======================
            // Loading default Albedo texture
            Application::GetCurrent().GetTextureLibrary()->LoadTexture("DefaultAlbedoTexture", "Engine\\Textures\\DefaultTexture.png", TextureFormat::RGB);
            Application::GetCurrent().GetCubeMapLibrary()->LoadCubeMap("FieldSkybox", "Engine\\Textures\\fieldRight.png", "Engine\\Textures\\fieldLeft.png", "Engine\\Textures\\fieldTop.png", "Engine\\Textures\\fieldBottom.png", "Engine\\Textures\\fieldFront.png", "Engine\\Textures\\fieldBack.png");
            Application::GetCurrent().GetCubeMapLibrary()->LoadCubeMap("SpaceSkybox", "Engine\\Textures\\spaceright.png", "Engine\\Textures\\spaceleft.png", "Engine\\Textures\\spacetop.png", "Engine\\Textures\\spacebottom.png", "Engine\\Textures\\spacefront.png", "Engine\\Textures\\spaceback.png");


            m_pipeline.defaultTextureMap = Application::GetCurrent().GetTextureLibrary()->GetResource("DefaultAlbedoTexture");

            m_pipeline.SkyboxCubeObject.Init();



            m_pipeline.skybox.cubeMap = Application::GetCurrent().GetCubeMapLibrary()->GetResource("SpaceSkybox");
            m_pipeline.skybox.SetIntensity(1.20f);


            m_pipeline.postprocessFrameBuffer = MakeShared<FrameBuffer>();
            m_pipeline.depthFrameBuffer = MakeShared<FrameBuffer>();
            m_pipeline.bloomFrameBuffer = MakeShared<FrameBuffer>();

            m_pipeline.rectangularObject.Init(1.0f);

            SecondaryFrameBuffer = MakeShared<FrameBuffer>();
            SecondaryTexture = MakeShared<Texture>();

            Vector2Int viewport = Application::GetCurrent().GetWindowSize();

            SecondaryTexture->Load(nullptr, viewport.x, viewport.y, 3, false, TextureFormat::RGB);
            SecondaryTexture->SetFilePath("RenderTexture");
            SecondaryTexture->SetWarping(TextureWraping::CLAMP_TO_EDGE);

            SecondaryFrameBuffer->AttachTexture(SecondaryTexture);
            

            
            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewport.x, viewport.y); // use a single renderbuffer object for both a depth AND stencil buffer.
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

           
            SecondaryFrameBuffer->Validate();
            SecondaryFrameBuffer->UnBind();
        }




        void Renderer::SetupDebugShaders(SharedPtr<Shader>& line, SharedPtr<Shader>& point)
        {
            m_lineShader = line;
            m_pointShader = point;
        }

        void Renderer::SetSkyboxIntensity(float intensity)
        {
            m_pipeline.skybox.SetIntensity(intensity);
        }

        const float Renderer::GetSkyboxIntensity() const
        {
            return m_pipeline.skybox.GetIntensity();
        }









        void Renderer::SetSkyboxCubeMap(SharedPtr<CubeMap> cubemap)
        {
            if (cubemap == nullptr)
            {
                return;
            }

            m_pipeline.skybox.cubeMap = cubemap;
        }

        void Renderer::ProcessRenderElement(const SharedPtr<Mesh>& mesh, const SharedPtr<Material>& material, Transform& transform)
        {
            if (material->albedoColour.a <= 0.0f)
            {
                // The Element is Invisible ... shouldn't be drawn
                return;
            }

            // Storing the new Element's index
            size_t elementIndex = m_pipeline.renderElementList.size();

            // Creatig a temporarty Element
            RenderElement newElement;

            if (mesh != nullptr)
            {
                newElement.meshIndex = m_pipeline.MeshList.size();
                m_pipeline.MeshList.push_back(mesh);
            }

            if (material != nullptr)
            {
                newElement.materialIndex = m_pipeline.MaterialList.size();

                // For now just checking for albedo
                if (material->textureMaps.albedoMap == nullptr)
                {
                    material->textureMaps.albedoMap = m_pipeline.defaultTextureMap;
                }

                m_pipeline.MaterialList.push_back(material);
            }


            newElement.ModelMatrix = transform.GetMatrix();
            //newElement.ModelMatrix = transform.GetLocalMatrix();

            // New!! - For lighting
            newElement.NormalMatrix = transform.GetNormalMatrix();


            // Getting the index of the element we are going to create
            size_t currentElementIndex = m_pipeline.renderElementList.size();

            // Storing the render Element in the container
            m_pipeline.renderElementList.push_back(newElement);



            switch (material->type)
            {
            case MaterialType::Opaque:

                // Storing the Render Element in the Opage container
                m_pipeline.opaqueElementList.push_back(elementIndex);

                break;

            case MaterialType::Transparent:
                break;

            case MaterialType::Masked:

                break;

            default:
                break;
            }




        }





        void Renderer::RenderToAttachedFrameBuffer(const SharedPtr<Shader>& shader)
        {
            RectangleObject& rectObject = m_pipeline.rectangularObject;
            shader->Bind();

            rectObject.GetVAO().Bind();
            
            DrawIndices(DrawType::TRIANGLES, rectObject.IndexCount, 0);
        }

        void Renderer::Clear()
        {
            GLDEBUG(glClearColor(0.5f, 0.1f, 0.1f, 1.0f));
            //GLDEBUG(glClear(clearMask));
            GLDEBUG(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        }

        void Renderer::ClearRenderCache()
        {
            m_pipeline.cameraList.clear();
            m_pipeline.MaterialList.clear();
            m_pipeline.MeshList.clear();
            m_pipeline.opaqueElementList.clear();
            m_pipeline.renderElementList.clear();
            m_pipeline.lightElementList.clear();
            m_pipeline.textureBindIndex = 0;

        }

        void Renderer::SetViewport(int x, int y, int width, int height) const
        {
            
            GLDEBUG(glViewport(x, y, width, height));
        }

        Vector2Int Renderer::GetViewport()
        {
            return viewPort;
        }

        void Renderer::SetWindowSize(const Vector2Int& size)
        {
            viewPort = size;
        }

        void Renderer::AttachDefaultFrameBuffer()
        {
            // Unbinding any framebuffer will revert thback to the default frame buffer ,(the one at 0)
             // doing this allows us to render to application window
            //m_pipeline.postprocessFrameBuffer->UnBind();

            SecondaryFrameBuffer->UnBind();

            SetViewport(0, 0, GetViewport().x, GetViewport().y);

            //GLDEBUG(glClear(GL_COLOR_BUFFER_BIT));
            Clear();

        }

        void Renderer::ClearRectangleObjectVAO()
        {
            m_pipeline.rectangularObject.GetVAO().UnBind();
        }





        void Renderer::ForwardPass(SharedPtr<Shader> shader, const CameraElement& camera, const MaterialType type)
        {

            if (shader == nullptr)
            {
                LOG_CRITICAL("FORWARD PASS : Material type :[{0}] : Shader not loaded", (int)type);
            }


            // ============Set Shader Unifroms here ==================
            shader->Bind();


            // Setting teh View Projection Matrix from the camera
            shader->SetUniform("viewProj", camera.viewProjMatrix);


            SetLightUniform(shader);


            std::vector<size_t> elementList;

            switch (type)
            {
            case MaterialType::Opaque: elementList = m_pipeline.opaqueElementList;

                break;
            default:
                break;
            }


            if (elementList.empty())
            {
                //LOG_ERROR("No Eleemnts to Draw");
                return;
            }

            for (int index = 0; index < elementList.size(); index++)
            {
                // get the Elements by index from the Render Element List
                RenderElement elementToDraw = m_pipeline.renderElementList[elementList[index]];

                // Send the Data to Draw that element
                DrawElement(camera, shader, elementToDraw);
            }



        }

        void Renderer::SkyBoxPass(SharedPtr<Shader> shader, const CameraElement& camera)
        {

            if (shader == nullptr)
            {
                LOG_CRITICAL("SKYBOX PASS : Shader not loaded");
                return;
            }

            //Setting depth test to false (so that the skybox object will always be rendered last)
            //GLDEBUG(glDepthMask(GL_FALSE));
            GLDEBUG(glDisable(GL_DEPTH_TEST));
            SkyboxObject& SkyObject = m_pipeline.SkyboxCubeObject;

            Vector3 radianRotaion = m_pipeline.skybox.GetRotation();
            radianRotaion.x = Radians(radianRotaion.x);
            radianRotaion.y = Radians(radianRotaion.y);
            radianRotaion.z = Radians(radianRotaion.z);

            Matrix3 inverseRotation = Transpose(GetRotationMatrix(radianRotaion));

            // get the intensity from the pipeline
            float skyluminance = m_pipeline.skybox.GetIntensity();

            skyluminance = skyluminance < 0.0f ? Skybox::Defaultintensity : skyluminance;

            // Bind and set Shader Unifroms
            shader->Bind();
            shader->SetUniform("StaticViewProjection", camera.staticViewProjectMatrix);
            shader->SetUniform("Rotation", Matrix3(1.0f));
            shader->SetUniform("gamma", 2.2f);
            shader->SetUniform("luminance", skyluminance);

            // Bind the skybox texture
            m_pipeline.skybox.cubeMap->Bind();

            // Bind the Skybox Object VAO
            SkyObject.GetVAO().Bind();

            // Finally draw the Object
            DrawIndices(DrawType::TRIANGLES, SkyObject.IndexCount, 0);


            //Setting depth test to back to true
           // GLDEBUG(glDepthMask(GL_TRUE));
            GLDEBUG(glEnable(GL_DEPTH_TEST));



        }

        void Renderer::DebugPassInternal(const CameraElement& camera, bool depthtest)
        {
            const std::vector<DebugLineData>& lines = DebugRenderer::Get()->GetLines(depthtest);
            const std::vector<DebugPointData>& points = DebugRenderer::Get()->GetPoints(depthtest);

            // Skipping triangles for now

            Matrix4 projView = camera.viewProjMatrix;

            // ====== DEBUG LINE DRAW CALL ============
            if (!lines.empty())
            {
                m_lineShader->Bind();
                m_lineShader->SetUniform("projectionView", projView);


                for (const DebugLineData& line : lines)
                {
                    if (m_debugDrawData.lineIndexCount >= MAX_QUADS)
                    {
                        break;
                    }


                    m_debugDrawData.lineDataBuffer.emplace_back(line.posOne, line.color);
                    m_debugDrawData.lineDataBuffer.emplace_back(line.posTwo, line.color);

                    m_debugDrawData.lineIndexCount += 2;
                }



                uint32_t dataSize = (uint32_t)sizeof(LineVertexElement) * m_debugDrawData.lineIndexCount;

                m_debugDrawData.VBO->SetData(dataSize, m_debugDrawData.lineDataBuffer.data());
                m_debugDrawData.VAO->Bind();

                DrawVertices(DrawType::LINES, 0, m_debugDrawData.lineIndexCount);

                m_debugDrawData.VBO->UnBind();



            }


        }

        void Renderer::DebugPass(const CameraElement& camera)
        {


            if (m_lineShader == nullptr || m_pointShader == nullptr)
            {
                return;
            }



            // Two Passes one for no depth and other for Depthtested
            DebugPassInternal(camera, false); 
            DebugPassInternal(camera, true);


            m_debugDrawData.lineIndexCount = 0;
            m_debugDrawData.lineDataBuffer.clear();

            DebugRenderer::Reset();
        }



        void Renderer::DrawElement(const CameraElement& camera, SharedPtr<Shader>& shader, const RenderElement& element)
        {
            m_pipeline.textureBindIndex = 0;

            uint32_t shaderId = shader->GetProgramId();


            SharedPtr<Mesh>& mesh = m_pipeline.MeshList[element.meshIndex];
            SharedPtr<Material>& mat = m_pipeline.MaterialList[element.materialIndex];

            if (mat != nullptr)
            {
                mat->textureMaps.albedoMap->Bind(m_pipeline.textureBindIndex++);
                shader->SetUniform("mapAlbedo", mat->textureMaps.albedoMap->GetBoundId());
                //shader->SetUniform("materialProperties.AlbedoMapFactor", mat->albedomapFactor);
                shader->SetUniform("materialProperties.AlbedoColor", mat->albedoColour);
                shader->SetUniform("materialProperties.Metallic", mat->metallic);

            }


            // mat->textureMaps.albedoMap->UnBind();


            shader->SetUniform("model", element.ModelMatrix);

            shader->SetUniform("normalMat", element.NormalMatrix);

            shader->SetUniform("cameraView", camera.viewPosition);





            //Always Bind the Buffer Array before adding Attributes 
            mesh->GetVBO()->Bind();

            m_pipeline.VAO->Bind();

            // Set the Shader Attributes
            m_pipeline.VAO->AddVertexAttribLayout(m_pipeline.vertexLayout);
            //m_pipeline.VAO->AddVertexAttributelayout(shaderId);

            //m_pipeline.VAO->AddVertexAttribLayout();

            // Bind the Index Buffer
            mesh->GetIBO()->Bind();


            DrawIndices(DrawType::TRIANGLES, mesh->GetIndexCount(), 0);



            // Unbind all the bound buffers 
            mesh->GetIBO()->UnBind();
            mesh->GetVBO()->UnBind();



        }

        void Renderer::SetLightUniform(SharedPtr<Shader>& shader)
        {

            shader->SetUniform("uboLights.lightCount", (int)m_pipeline.lightElementList.size());


            for (LightElement element : m_pipeline.lightElementList)
            {

                const std::string colorUniform = element.uniformName + ".color";
                const std::string positionUniform = element.uniformName + ".position";
                const std::string directionUniform = element.uniformName + ".direction";
                const std::string intensityUniform = element.uniformName + ".intensity";
                const std::string radiusUniform = element.uniformName + ".radius";
                const std::string typeUniform = element.uniformName + ".type";


                const std::string innerAngleUniform = element.uniformName + ".innerAngle";
                const std::string outerAngleUniform = element.uniformName + ".outerAngle";


                shader->SetUniform(colorUniform, element.color);


                shader->SetUniform(intensityUniform, element.intensity);
                shader->SetUniform(typeUniform, (int)element.type);
                shader->SetUniform(positionUniform, element.position);



                switch (element.type)
                {
                case LightType::DirectionLight:
                    shader->SetUniform(directionUniform, element.direction);

                    break;

                case LightType::SpotLight:
                    shader->SetUniform(innerAngleUniform, element.innerAngle);
                    shader->SetUniform(outerAngleUniform, element.outerAngle);
                    shader->SetUniform(directionUniform, element.direction);

                    break;
                case LightType::PointLight:
                    shader->SetUniform(radiusUniform, element.radius);


                    break;
                default:
                    break;
                }
            }



        }





        void Renderer::SetUpCameraElement(Camera& cameraRef, Transform& transform)
        {

            CameraElement camElement = GenerateCameraElement(cameraRef, transform);
            m_pipeline.cameraList.push_back(camElement);

        }

        CameraElement Renderer::GenerateCameraElement(Camera& cameraRef, Transform& transform)
        {
            // Creating a new  Data only Camera element
            CameraElement camera;

            // Setting the details
            camera.aspectRatio = cameraRef.GetAspectRatio();
            camera.isOrtho = cameraRef.IsOrthographic();

            camera.viewPosition = transform.GetPosition();

            //          Vector3 dir = camera.viewPosition + cameraRef->GetDirection();

                        // For now , since the camera is not parented to anything. Later multiply the parent's transform to it
                        //Matrix4 view = Math::GetLookAt(camera.viewPosition, camera.viewPosition + cameraRef.GetDirection(), cameraRef.GetUpVector());


            Matrix4 transformMatrix = transform.GetMatrix();
            // Matrix4 transformMatrix = transform.GetLocalMatrix();

             // get the inverse of the Camera trasfrom
            // Matrix4 view = Math::Inverse(transformMatrix);
            Matrix4 view = Math::Inverse(transformMatrix);

            // get the inverse of the Camera transform without any position data (only Rotation0
            Matrix4 staticView = Math::Inverse(Matrix4(Matrix3(transformMatrix)));

            // the Projection of the camera
            Matrix4 proj = (cameraRef.GetProjectionMatrix());

            // calculate the vie projection matrix
            Matrix4 projView = proj * (view);

            //calculate the view projection of the static view (no positional data)
            Matrix4 staticProjView = proj * (staticView);

            // Store the values
            camera.viewProjMatrix = projView;
            camera.staticViewProjectMatrix = staticProjView;
            camera.outputTexture = cameraRef.GetRenderTexture();

            camera.gBuffer = cameraRef.GetBuffers()->gBuffer;

            return camera;
        }


        const PipeLine& Renderer::GetPipeLine() const
        {
            return m_pipeline;
        }

      /*  PipeLine& Renderer::GetPipeLine()
        {
            return m_pipeline;
        }*/

       



        void Renderer::ProcessLightElement(Light& light, Transform& transform)
        {
            size_t currentIndex = m_pipeline.lightElementList.size();

            // Create anew Light Element;
            LightElement& lightElement = m_pipeline.lightElementList.emplace_back();


            lightElement.color = light.color;
            lightElement.direction = light.direction;
            lightElement.innerAngle = light.innerAngle;
            lightElement.outerAngle = light.outerAngle;
            lightElement.intensity = light.intensity;
            lightElement.position = transform.GetPosition();
            light.position = transform.GetPosition();
            lightElement.radius = light.radius;
            lightElement.type = light.type;

            lightElement.uniformName = "uboLights.lights[" + std::to_string(currentIndex) + "]";



        }

        void Renderer::RenderScreenQuad(SharedPtr<Shader> shader, const SharedPtr<Texture>& texture, int lod)
        {
    
            texture->Bind();
            //int boundId = texture->GetBoundId();
            int boundId = texture->GetHandle();
            shader->Bind();
            shader->SetUniform("tex", boundId);
            shader->SetUniform("lod", lod);

            m_pipeline.rectangularObject.GetVAO().Bind();
            DrawIndices(DrawType::TRIANGLES,  m_pipeline.rectangularObject.IndexCount, 0);

        }

        SharedPtr<FrameBuffer>& Renderer::GetPostProcessFrameBuffer()
        {
            return m_pipeline.postprocessFrameBuffer;
        }
	}
}
