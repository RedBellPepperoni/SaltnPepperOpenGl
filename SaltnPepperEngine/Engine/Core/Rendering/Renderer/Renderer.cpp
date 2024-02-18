#include "Renderer.h"
#include "Engine/Core/System/Window/Window.h"
#include "Engine/Utils/Logging/Log.h"

#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Rendering/Shader/Shader.h"

#include "Engine/Core/Rendering/Shader/Compute.h"
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
            Application::GetCurrent().GetTextureLibrary()->LoadTexture("GreyTexture", "Engine\\Textures\\GreyTexture.png", TextureFormat::RGB);
            Application::GetCurrent().GetTextureLibrary()->LoadTexture("WhiteTexture", "Engine\\Textures\\WhiteTexture.png", TextureFormat::RGB);
            Application::GetCurrent().GetTextureLibrary()->LoadTexture("BlackTexture", "Engine\\Textures\\BlackTexture.png", TextureFormat::RGB);
            Application::GetCurrent().GetTextureLibrary()->LoadTexture("NormalTexture", "Engine\\Textures\\NormalTexture.png", TextureFormat::RGB);
            Application::GetCurrent().GetTextureLibrary()->LoadTexture("BrdfLutTexture", "Engine\\Textures\\brdflut.png", TextureFormat::RGB);
           

            Application::GetCurrent().GetCubeMapLibrary()->LoadCubeMap("FieldSkybox", "Engine\\Textures\\fieldRight.png", "Engine\\Textures\\fieldLeft.png", "Engine\\Textures\\fieldTop.png", "Engine\\Textures\\fieldBottom.png", "Engine\\Textures\\fieldFront.png", "Engine\\Textures\\fieldBack.png");
            Application::GetCurrent().GetCubeMapLibrary()->LoadCubeMap("SpaceSkybox", "Engine\\Textures\\spaceright.png", "Engine\\Textures\\spaceleft.png", "Engine\\Textures\\spacetop.png", "Engine\\Textures\\spacebottom.png", "Engine\\Textures\\spacefront.png", "Engine\\Textures\\spaceback.png");


            m_pipeline.defaultGreyTexture = Application::GetCurrent().GetTextureLibrary()->GetResource("GreyTexture");
            m_pipeline.defaultBlackTexture = Application::GetCurrent().GetTextureLibrary()->GetResource("BlackTexture");
            m_pipeline.defaultWhiteTexture = Application::GetCurrent().GetTextureLibrary()->GetResource("WhiteTexture");
            m_pipeline.defaultNormalTexture = Application::GetCurrent().GetTextureLibrary()->GetResource("NormalTexture");
            m_pipeline.brdfLUTTexture = Application::GetCurrent().GetTextureLibrary()->GetResource("BrdfLutTexture");


            m_pipeline.SkyboxCubeObject.Init();



            m_pipeline.skybox.cubeMap = Application::GetCurrent().GetCubeMapLibrary()->GetResource("SpaceSkybox");
            m_pipeline.skybox.SetIntensity(1.20f);


            m_pipeline.postprocessFrameBuffer = MakeShared<FrameBuffer>();
            m_pipeline.depthFrameBuffer = MakeShared<FrameBuffer>();
            m_pipeline.bloomFrameBuffer = MakeShared<FrameBuffer>();

            m_pipeline.rectangularObject.Init(1.0f);

            //SecondaryFrameBuffer = MakeShared<FrameBuffer>();
            //SecondaryTexture = MakeShared<Texture>();

            //Vector2Int viewport = Application::GetCurrent().GetWindowSize();

            //SecondaryTexture->Load(nullptr, viewport.x, viewport.y, 3, false, TextureFormat::RGB);
            //SecondaryTexture->SetFilePath("RenderTexture");
            //SecondaryTexture->SetWarping(TextureWraping::CLAMP_TO_EDGE);

            //SecondaryFrameBuffer->AttachTexture(SecondaryTexture);
            //

            //
            //glGenRenderbuffers(1, &rbo);
            //glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewport.x, viewport.y); // use a single renderbuffer object for both a depth AND stencil buffer.
            //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

           
            //SecondaryFrameBuffer->Validate();
            //SecondaryFrameBuffer->UnBind();
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

        void Renderer::BindSkyBoxInformation(const CameraElement& camera, SharedPtr<Shader>& shader, int textureBindId)
        {
            m_pipeline.skybox.cubeMap->Bind(textureBindId++);
            m_pipeline.brdfLUTTexture->Bind(textureBindId++);
            shader->SetUniform("environment.skybox", m_pipeline.skybox.cubeMap->getBoundId());
            shader->SetUniform("environment.irradiance", m_pipeline.skybox.cubeMap->getBoundId());
            shader->SetUniform("environment.skyboxRotaion", Matrix3(1.0f));
            shader->SetUniform("encironment.envBRDFLUT", m_pipeline.brdfLUTTexture->GetBoundId());

            // get the intensity from the pipeline
            float skyluminance = m_pipeline.skybox.GetIntensity();

            skyluminance = skyluminance < 0.0f ? Skybox::Defaultintensity : skyluminance;

            shader->SetUniform("environment.intensity", skyluminance);

        }

        void Renderer::BindCameraInformation(const CameraElement& camera, SharedPtr<Shader>& shader)
        {
            shader->SetUniform("camera.viewProjMatrix", camera.viewProjMatrix);
            shader->SetUniform("camera.invViewProjMatrix", camera.invViewProjMatrix);
            shader->SetUniform("camera.position", camera.viewPosition);
        }

        void Renderer::BindCameraBuffers(const CameraElement& camera, SharedPtr<Shader>& shader, int startid)
        {
            camera.albedoTexture->Bind(startid++);
            camera.normalTexture->Bind(startid++);
            camera.materialTexture->Bind(startid++);
            camera.depthTexture->Bind(startid++);

            shader->SetUniform("albedoMap", camera.albedoTexture->GetBoundId());
            shader->SetUniform("normalMap", camera.albedoTexture->GetBoundId());
            shader->SetUniform("materialMap", camera.albedoTexture->GetBoundId());
            shader->SetUniform("depthMap", camera.albedoTexture->GetBoundId());
        }









        void Renderer::SetSkyboxCubeMap(SharedPtr<CubeMap> cubemap)
        {
            if (cubemap == nullptr)
            {
                return;
            }

            m_pipeline.skybox.cubeMap = cubemap;
        }

        void Renderer::ProcessRenderElement( SharedPtr<Mesh> mesh, SharedPtr<Material> material, Transform& transform)
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

                //Checking Texture Validity
                if (material->albedoMap == nullptr) 
                { material->albedoMap = m_pipeline.defaultGreyTexture;}

                if (material->normalMap == nullptr) 
                { material->normalMap = m_pipeline.defaultNormalTexture; }

                if (material->metallicMap == nullptr) 
                { material->metallicMap = m_pipeline.defaultWhiteTexture; }

                if (material->roughnessMap == nullptr) 
                { material->roughnessMap = m_pipeline.defaultWhiteTexture; }

                if (material->aoMap == nullptr) 
                { material->aoMap = m_pipeline.defaultWhiteTexture; }

                if (material->emissiveMap == nullptr) 
                { material->emissiveMap = m_pipeline.defaultBlackTexture; }
              
         
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



            switch (material->m_type)
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

            rectObject.GetVAO()->Bind();
            
            DrawIndices(DrawType::TRIANGLES, rectObject.IndexCount, 0);
            //rectObject.GetVAO()->UnBind();
        }

        void Renderer::Clear(bool clearDepth)
        {
            GLDEBUG(glClearColor(0.5f, 0.1f, 0.1f, 1.0f));
            if (clearDepth)
            {
                GLDEBUG(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            }
            else
            {
                GLDEBUG(glClear(GL_COLOR_BUFFER_BIT));
            }
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
            m_pipeline.postprocessFrameBuffer->UnBind();

            SetViewport(0, 0, GetViewport().x, GetViewport().y);
            Clear();

        }

        void Renderer::ClearRectangleObjectVAO()
        {
            m_pipeline.rectangularObject.GetVAO()->UnBind();
        }





        void Renderer::ObjectPass(SharedPtr<Shader> shader, const CameraElement& camera, std::vector<size_t>& elementList)
        {
            GLDEBUG(glEnable(GL_DEPTH_TEST));
            if (shader == nullptr) { LOG_CRITICAL("Object PASS :  Shader not loaded"); }

            // Nothing to draw
            if (elementList.empty()) { return; }
        
            // ============Set Shader Unifroms here ==================
            shader->Bind();

            BindCameraInformation(camera, shader);

           // SetLightUniform(shader);

           
            for (int index = 0; index < elementList.size(); index++)
            {
                // get the Elements by index from the Render Element List
                RenderElement& elementToDraw = m_pipeline.renderElementList[elementList[index]];

                // Send the Data to Draw that element
                DrawElement(camera, shader, elementToDraw);
            }



        }

        void Renderer::ImagedBasedLightPass(SharedPtr<Shader> shader, const CameraElement& camera, SharedPtr<Texture> outputTexture)
        {
            shader->Bind();
            shader->IgnoreNonExistingUniform("camera.viewProjMatrix");
            int textureId = 0;

            BindCameraBuffers(camera,shader,textureId);
            BindCameraInformation(camera,shader);
            BindSkyBoxInformation(camera, shader, textureId);

        }

        void Renderer::DirectionalLightPass(const CameraElement& camera, SharedPtr<Texture> outputTexture)
        {
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

            uint32_t shaderId = shader->GetNativeHandle();


            SharedPtr<Mesh>& mesh = m_pipeline.MeshList[element.meshIndex];
            SharedPtr<Material>& mat = m_pipeline.MaterialList[element.materialIndex];

            if (mat != nullptr)
            {
                // ======== Albedo Map Bindings ==============
                mat->albedoMap->Bind(m_pipeline.textureBindIndex++);
                shader->SetUniform("mapAlbedo", mat->albedoMap->GetBoundId());
                shader->SetUniform("materialProperties.AlbedoMapFactor", mat->albedoMapFactor);
                shader->SetUniform("materialProperties.Color", mat->albedoColour);

                // ======= Metallic Map Bindings ===============
                mat->metallicMap->Bind(m_pipeline.textureBindIndex++); 
                shader->SetUniform("mapMetallic", mat->metallicMap->GetBoundId()); 
                shader->SetUniform("materialProperties.MetallicMapFactor", mat->metallicMapFactor);
                shader->SetUniform("materialProperties.Metallic", mat->metallic);

                // ======= Normal Map Bindings ===============
                mat->normalMap->Bind(m_pipeline.textureBindIndex++); 
                shader->SetUniform("mapMetallic", mat->metallicMap->GetBoundId()); 
                shader->SetUniform("materialProperties.NormalMapFactor", mat->metallicMapFactor);


                // ======== Roughness Map Bindings ============
                mat->roughnessMap->Bind(m_pipeline.textureBindIndex++);
                shader->SetUniform("mapRoughness", mat->roughnessMap->GetBoundId());
                shader->SetUniform("materialProperties.RoughnessMapFactor", mat->roughnessMapFactor);
                shader->SetUniform("materialProperties.Roughness", mat->roughness);

                // ======== Ambient Occlusion Map Bindings ============
                mat->aoMap->Bind(m_pipeline.textureBindIndex++);
                shader->SetUniform("mapAO", mat->aoMap->GetBoundId());
                shader->SetUniform("materialProperties.AOMapFactor", mat->aoMapFactor);

                // ======== Emissive Map Bindings ==============
                mat->emissiveMap->Bind(m_pipeline.textureBindIndex++);
                shader->SetUniform("mapEmissive", mat->emissiveMap->GetBoundId());
                shader->SetUniform("materialProperties.EmissiveMapFactor", mat->emissiveMapFactor);
                shader->SetUniform("materialProperties.Emissive", mat->emissiveColor);

            }


            // mat->textureMaps.albedoMap->UnBind();


            shader->SetUniform("model", element.ModelMatrix);

            shader->SetUniform("normalMat", element.NormalMatrix);


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
            CameraElement camera{};

            // Setting the details
            camera.aspectRatio = cameraRef.GetAspectRatio();
            camera.isOrtho = cameraRef.IsOrthographic();

            camera.viewPosition = transform.GetPosition();
        

            Matrix4 transformMatrix = transform.GetMatrix();
            Vector3 Newposition = transformMatrix[3];

          
            Matrix4 view = Math::Inverse(transformMatrix);

            // get the inverse of the Camera transform without any position data (only Rotation0
            Matrix4 staticView = Math::Inverse(Matrix4(Matrix3(transformMatrix)));

            // the Projection of the camera
            Matrix4 proj = (cameraRef.GetProjectionMatrix());

            // calculate the vie projection matrix
            Matrix4 projView = proj * (view);

            //calculate the view projection of the static view (no positional data)
            Matrix4 staticProjView = proj * (staticView);

            // Store the camera values
            camera.viewProjMatrix = projView;
            camera.invViewProjMatrix = Math::Inverse(projView);
            camera.staticViewProjectMatrix = staticProjView;

            camera.outputTexture = cameraRef.GetRenderTexture();

            camera.gBuffer = cameraRef.GetBuffers()->gBuffer;
            camera.albedoTexture = cameraRef.GetBuffers()->albedoTexture;
            camera.normalTexture = cameraRef.GetBuffers()->normalTexture;
            camera.materialTexture = cameraRef.GetBuffers()->materialTexture;
            camera.depthTexture = cameraRef.GetBuffers()->depthTexture;

            camera.postProcessTexture = cameraRef.GetBuffers()->postProcessTexture;
            camera.swapOneTexture = cameraRef.GetBuffers()->swapOneTexture;
            camera.swapTwoTexture = cameraRef.GetBuffers()->swapTwoTexture;




            return camera;
        }


       /* const PipeLine& Renderer::GetPipeLine() const
        {
            return m_pipeline;
        }*/

        PipeLine& Renderer::GetPipeLine()
        {
            return m_pipeline;
        }

       



        void Renderer::ProcessLightElement(Light& light, Transform& transform)
        {
            size_t currentIndex = m_pipeline.lightElementList.size();

            // Create anew Light Element;
            LightElement& lightElement = m_pipeline.lightElementList.emplace_back();


            lightElement.color = light.color;
            lightElement.direction = light.direction = Normalize(transform.GetForwardVector());
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
    
            texture->Bind(0);
            int boundId = texture->GetBoundId();
            shader->Bind();
            shader->SetUniform("tex", boundId);
            shader->SetUniform("lod", lod);

            m_pipeline.rectangularObject.GetVAO()->Bind();
            DrawIndices(DrawType::TRIANGLES,  m_pipeline.rectangularObject.IndexCount, 0);
            //DrawVertices(DrawType::TRIANGLES, 0, 6);

        }

        void Renderer::ComputeParticles(std::vector<ParticleElement>& particleList, SharedPtr<ComputeShader>& computeShader)
        {
            if (particleList.empty())
            {
                return;
            }

            computeShader->Bind();
            computeShader->SetUniform("dt", Min(Time::DeltaTime(), 1.0f / 60.0f));

            for (ParticleElement& particleSystem : particleList)
            {
                particleSystem.SSBO->BindBase(0);

                //computeShader->SetUniform("bufferOffset", (int)particleSystem.ParticleBufferOffset);
                computeShader->SetUniform("lifetime", particleSystem.particleLifetime);
                computeShader->SetUniform("spawnpoint", particleSystem.isRelative ? Vector3(0.0f) : Vector3(particleSystem.transform[3]));
            
                Compute::Dispatch(computeShader, particleSystem.invocationCount, 1, 1);
            }
            
        }

        void Renderer::SortParticles(const CameraElement& camera, std::vector<ParticleElement>& particleList)
        {
            std::sort(particleList.begin(), particleList.end(),
                [&camera](const ParticleElement& p1, const ParticleElement& p2)
                {
                    auto dist1 = camera.viewPosition - Vector3(p1.transform[3]);
                    auto dist2 = camera.viewPosition - Vector3(p2.transform[3]);
                    return Dot(dist1, dist1) > Dot(dist2, dist2);
                });
        }

        void Renderer::DrawParticles(const CameraElement& camera, std::vector<ParticleElement>& particleList, SharedPtr<Shader> shader)
        {
            if (particleList.empty()){ return;}
            
            // Sort the particles
            SortParticles(camera,particleList);

            shader->Bind();
            shader->IgnoreNonExistingUniform("viewportSize");
            shader->IgnoreNonExistingUniform("depthTex");
            shader->IgnoreNonExistingUniform("light");
            shader->IgnoreNonExistingUniform("light");
            shader->IgnoreNonExistingUniform("fading");
            shader->IgnoreNonExistingUniform("lifetime");
            shader->IgnoreNonExistingUniform("skybox");
            shader->IgnoreNonExistingUniform("irradiance");
            shader->IgnoreNonExistingUniform("skyboxRotation");
            shader->IgnoreNonExistingUniform("intensity");


            Vector2 viewportSize = Vector2((float)camera.outputTexture->GetWidth(), (float)camera.outputTexture->GetHeight());

           
        }

        SharedPtr<FrameBuffer>& Renderer::GetPostProcessFrameBuffer()
        {
            return m_pipeline.postprocessFrameBuffer;
        }
	}
}
