#include "Renderer.h"
#include "Engine/Core/System/Window/Window.h"
#include "Engine/Utils/Logging/Log.h"

#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/Rendering/Textures/DepthTextureArray.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Rendering/Shader/Shader.h"

#include "Engine/Core/Rendering/Shader/Compute.h"
#include "Engine/Core/Rendering/Buffers/VertexArray.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"
#include "Engine/Core/Rendering/Camera/Camera.h"

#include "Engine/Core/Resources/ResourceManager.h"
#include "Engine/Core/Components/Transform.h"

#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingSphere.h" 

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

                VertexAttribute::Attribute<Vector4Int>(), // boneId
                VertexAttribute::Attribute<Vector4>(), // boneWeight
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
            Application::GetCurrent().GetTextureLibrary()->LoadTexture("DefaultTexture", "Engine\\Textures\\DefaultTexture.png", TextureFormat::RGB);
            Application::GetCurrent().GetTextureLibrary()->LoadTexture("Black", "Engine\\Textures\\black.png", TextureFormat::RGBA);
            
            Application::GetCurrent().GetCubeMapLibrary()->LoadCubeMap("DuskSkybox", "Engine\\Textures\\sky_right.png", "Engine\\Textures\\sky_left.png", "Engine\\Textures\\sky_top.png", "Engine\\Textures\\sky_bottom.png", "Engine\\Textures\\sky_front.png", "Engine\\Textures\\sky_back.png");
            Application::GetCurrent().GetCubeMapLibrary()->LoadCubeMap("SpaceSkybox", "Engine\\Textures\\spaceright.png", "Engine\\Textures\\spaceleft.png", "Engine\\Textures\\spacetop.png", "Engine\\Textures\\spacebottom.png", "Engine\\Textures\\spacefront.png", "Engine\\Textures\\spaceback.png");

            m_pipeline.defaultTextureMap = Application::GetCurrent().GetTextureLibrary()->GetResource("DefaultTexture");

            m_pipeline.SkyboxCubeObject.Init();



            m_pipeline.skybox.cubeMap = Application::GetCurrent().GetCubeMapLibrary()->GetResource("DuskSkybox");
            m_pipeline.skybox.SetIntensity(0.62f);


            m_pipeline.postprocessFrameBuffer = MakeShared<FrameBuffer>();
            m_pipeline.depthFrameBuffer = MakeShared<FrameBuffer>();
            m_pipeline.bloomFrameBuffer = MakeShared<FrameBuffer>();

            m_pipeline.rectangularObject.Init(1.0f);



            shadowData.numShadowMaps = 4;
            shadowData.shadowMapSize = 1024;
            shadowData.shadowMapInvalidated = true;

            shadowData.cascadeFarPlaneOffset = 50.0f;
            shadowData.cascadeNearPlaneOffset = -50.0f;

            shadowData.cascadeSplitLambda = 0.92f;
            shadowData.lightSize = 1.5f;
            shadowData.maxShadowDistance = 500.0f;
            shadowData.shadowFade = 40.0f;
            shadowData.cascadeFade = 3.0f;
            shadowData.initialBias = 0.0f;


            shadowData.shadowMap = MakeShared<DepthTextureArray>(shadowData.shadowMapSize, shadowData.shadowMapSize, shadowData.numShadowMaps);
        
        
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
            m_pipeline.skybox.cubeMap->Bind(textureBindId);
            shader->SetUniform("skybox", m_pipeline.skybox.cubeMap->getBoundId());
            shader->SetUniform("Rotation", Matrix3(1.0f));


            // get the intensity from the pipeline
            float skyluminance = m_pipeline.skybox.GetIntensity();

            skyluminance = skyluminance < 0.0f ? Skybox::Defaultintensity : skyluminance;

            shader->SetUniform("luminance", skyluminance);

        }

        void Renderer::BindCameraInformation(const CameraElement& camera, SharedPtr<Shader>& shader)
        {
            shader->SetUniform("viewProj", camera.viewProjMatrix);
            shader->SetUniform("cameraView", camera.viewPosition);
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



            switch (material->m_type)
            {
            case MaterialType::Opaque:

                // Storing the Render Element in the Opage container
                m_pipeline.opaqueElementList.push_back(elementIndex);

                break;

            case MaterialType::Transparent:
                m_pipeline.transparentElementList.push_back(elementIndex);
                break;

            case MaterialType::Masked:

            case MaterialType::Custom:
                m_pipeline.customElementList.push_back(elementIndex);
                break;

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
        }

        void Renderer::Clear(bool clearDepth)
        {
            GLDEBUG(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
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
            m_pipeline.transparentElementList.clear();
            m_pipeline.customElementList.clear();
            m_pipeline.renderElementList.clear();
            m_pipeline.lightElementList.clear();
            m_pipeline.textureBindIndex = 0;

            dirElement.type = LightType::SpotLight;

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
            //m_pipeline.textureBindIndex = 0;



            LightElement dirElement;
            dirElement.type = LightType::SpotLight;

            if (shader == nullptr) { LOG_CRITICAL("Object PASS :  Shader not loaded"); }

            // Nothing to draw
            if (elementList.empty()) { return; }
        
            // ============Set Shader Unifroms here ==================
            shader->Bind();

            BindCameraInformation(camera, shader);

            SetLightUniform(camera,shader);

           
            for (int index = 0; index < elementList.size(); index++)
            {
                // get the Elements by index from the Render Element List
                RenderElement& elementToDraw = m_pipeline.renderElementList[elementList[index]];





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
            //shader->SetUniform("Rotation", Matrix3(1.0f));
            //shader->SetUniform("gamma", 2.2f);
            //shader->SetUniform("luminance", skyluminance);

            //// Bind the skybox texture
            //m_pipeline.skybox.cubeMap->Bind();

            BindSkyBoxInformation(camera, shader,0);


            // Bind the Skybox Object VAO
            SkyObject.GetVAO().Bind();

            // Finally draw the Object
            DrawIndices(DrawType::TRIANGLES, SkyObject.IndexCount, 0);


            //Setting depth test to back to true
           // GLDEBUG(glDepthMask(GL_TRUE));
            GLDEBUG(glEnable(GL_DEPTH_TEST));



        }

        void Renderer::CalculateShadowCascades(const CameraElement& camera, LightElement& directioanlLightElement)
        {
            float cascadeSplits[SHADOWMAP_MAX];
            float cascadeRadius[SHADOWMAP_MAX];

            float nearClip = camera.camNear;
            float farClip = shadowData.maxShadowDistance * 1.2f;
            float clipRange = farClip - nearClip;

            float minZ = nearClip;
            float maxZ = nearClip + clipRange;
            float range = maxZ - minZ;
            float ratio = maxZ / minZ;

            // Calculate split depths based on view camera frustum
            // Based on method presented in https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch10.html
            for (uint32_t i = 0; i < shadowData.numShadowMaps; i++)
            {
                float p = static_cast<float>(i + 1) / static_cast<float>(shadowData.numShadowMaps);
                float log = minZ * std::pow(ratio, p);
                float uniform = minZ + range * p;
                float d = shadowData.cascadeSplitLambda * (log - uniform) + uniform;
                cascadeSplits[i] = (d - nearClip) / clipRange;
            }

            cascadeSplits[3] = 0.35f;
            float lastSplitDist = 0.0f;
            Matrix4 CameraProj = glm::perspective(glm::radians(camera.FOV), camera.aspectRatio, nearClip, farClip);

            const Matrix4 invCam = Inverse(CameraProj * Inverse(camera.worldMatrix));

            for (uint32_t i = 0; i < shadowData.numShadowMaps; i++)
            {
                float splitDist = cascadeSplits[i];

                Vector3 frustumCorners[8] = {
                    Vector3(-1.0f, 1.0f, 0.0f),
                    Vector3(1.0f, 1.0f, 0.0f),
                    Vector3(1.0f, -1.0f, 0.0f),
                    Vector3(-1.0f, -1.0f, 0.0f),
                    Vector3(-1.0f, 1.0f, 1.0f),
                    Vector3(1.0f, 1.0f, 1.0f),
                    Vector3(1.0f, -1.0f, 1.0f),
                    Vector3(-1.0f, -1.0f, 1.0f),
                };

                // Project frustum corners into world space
                for (uint32_t j = 0; j < 8; j++)
                {
                    Vector4 invCorner = invCam * Vector4(frustumCorners[j], 1.0f);
                    frustumCorners[j] = (invCorner / invCorner.w);
                }

                for (uint32_t j = 0; j < 4; j++)
                {
                    Vector3 dist = frustumCorners[j + 4] - frustumCorners[j];
                    frustumCorners[j + 4] = frustumCorners[j] + (dist * splitDist);
                    frustumCorners[j] = frustumCorners[j] + (dist * lastSplitDist);
                }

                lastSplitDist = cascadeSplits[i];

                // Get frustum center
                Vector3 frustumCenter = Vector3(0.0f);
                for (uint32_t j = 0; j < 8; j++)
                {
                    frustumCenter += frustumCorners[j];
                }
                frustumCenter /= 8.0f;

                float radius = 0.0f;
                for (uint32_t j = 0; j < 8; j++)
                {
                    float distance = Distance(frustumCorners[j], frustumCenter);
                    radius = Max(radius, distance);
                }

                // Temp work around to flickering when rotating camera
                // Sphere radius for lightOrthoMatrix should fix this
                // But radius changes as the camera is rotated which causes flickering
                // const float value = 16.0f;
                // radius = std::ceil(radius *value) / value;

                static const float roundTo[8] = { 5.0f, 5.0f, 20.0f, 200.0f, 400.0f, 400.0f, 400.0f, 400.0f };

                int roundedValue = static_cast<int>(std::ceil(radius / 5.0));
                // Multiply the rounded value by 5 to get the nearest multiple of 5
                radius = roundedValue * 5.0f;



                cascadeRadius[i] = radius;

                Vector3 maxExtents = Vector3(radius);
                Vector3 minExtents = -maxExtents;

                Vector3 lightDir = Normalize(-directioanlLightElement.direction);
                Matrix4 lightOrthoMatrix = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, shadowData.cascadeFarPlaneOffset, maxExtents.z - minExtents.z + shadowData.cascadeFarPlaneOffset);
                Matrix4 LightViewMatrix = glm::lookAt(frustumCenter - lightDir * -minExtents.z, frustumCenter, Vector3(0.0f, 0.0f, 1.0f));

                auto shadowProj = lightOrthoMatrix * LightViewMatrix;

                const bool StabilizeCascades = true;
                if (StabilizeCascades)
                {
                    // Create the rounding matrix, by projecting the world-space origin and determining
                    // the fractional offset in texel space
                    Vector4 shadowOrigin = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
                    shadowOrigin = shadowProj * shadowOrigin;
                    shadowOrigin *= (shadowData.shadowMapSize * 0.5f);

                    Vector4 roundedOrigin = glm::round(shadowOrigin);
                    Vector4 roundOffset = roundedOrigin - shadowOrigin;
                    roundOffset = roundOffset * (2.0f / shadowData.shadowMapSize);
                    roundOffset.z = 0.0f;
                    roundOffset.w = 0.0f;

                    lightOrthoMatrix[3] += roundOffset;
                }
                // Store split distance and matrix in cascade
                shadowData.splitDepth[i] = Vector4((camera.camNear + splitDist * clipRange) * -1.0f);
                shadowData.shadowProjView[i] = lightOrthoMatrix * LightViewMatrix;

                if (i == 0)
                {
                    shadowData.lightMatrix = glm::inverse(LightViewMatrix);
                }
            }
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
                mat->textureMaps.albedoMap->Bind(m_pipeline.textureBindIndex++);
                shader->SetUniform("mapAlbedo", mat->textureMaps.albedoMap->GetBoundId());

                if (mat->m_type == MaterialType::Transparent)
                {

                    BindSkyBoxInformation(camera,shader, m_pipeline.textureBindIndex++);

                    shader->SetUniform("materialProperties.Metallic", mat->metallic);
                    shader->SetUniform("materialProperties.Reflectance", mat->roughness);

                    int skybodId = m_pipeline.skybox.cubeMap->getBoundId();
                    shader->SetUniform("skybox", skybodId);

                }

                if (mat->m_type == MaterialType::Custom)
                {
                    float time = Time::CurrentTime();
                    mat->textureMaps.metallicMap->Bind(m_pipeline.textureBindIndex++);
                    shader->SetUniform("mapMetallic", mat->textureMaps.metallicMap->GetBoundId());
                    shader->SetUniform("shaderTime",time);

                    if (mat->name == "Distort")
                    {
                        shader->SetUniform("distort",true);
                    }
                    else
                    {
                        shader->SetUniform("distort", false);
                    }
                }

                // Opaque Shader
                shader->SetUniform("materialProperties.AlbedoMapFactor", mat->albedoMapFactor);
                shader->SetUniform("materialProperties.AlbedoColor", mat->albedoColour);

                shader->SetUniform("materialProperties.Metallic", mat->metallic);

            }


            // mat->textureMaps.albedoMap->UnBind();


            shader->SetUniform("model", element.ModelMatrix);

            shader->SetUniform("normalMat", element.NormalMatrix);

            shadowData.shadowMap->Bind(m_pipeline.textureBindIndex++);
            shader->SetUniform("mapShadow", shadowData.shadowMap->GetBoundId());




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

        void Renderer::SetLightUniform(const CameraElement& camera,SharedPtr<Shader>& shader)
        {
            dirElement.type = LightType::SpotLight;
           
            int lightCount = 0;


            for (LightElement& element : m_pipeline.lightElementList)
            {
              /*  if (element.type != LightType::DirectionLight)
                {
                    bool insideFrustum = camera.frustum.Intersects(BoundingSphere(Vector3(element.position), element.radius*100.0f));
                }*/

                std::string name = "uboLights.lights[" + std::to_string(lightCount) + "]";

                const std::string colorUniform = name + ".color";
                const std::string positionUniform = name + ".position";
                const std::string directionUniform = name + ".direction";
                const std::string intensityUniform = name + ".intensity";
                const std::string radiusUniform = name + ".radius";
                const std::string typeUniform = name + ".type";


                const std::string innerAngleUniform = name + ".innerAngle";
                const std::string outerAngleUniform = name + ".outerAngle";


                shader->SetUniform(colorUniform, element.color);


                shader->SetUniform(intensityUniform, element.intensity);
                shader->SetUniform(typeUniform, (int)element.type);
                shader->SetUniform(positionUniform, element.position);



                switch (element.type)
                {
                case LightType::DirectionLight:
                    shader->SetUniform(directionUniform, element.direction);

                    dirElement = element;

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

                lightCount++;
            }

            shader->SetUniform("uboLights.lightCount",lightCount);

            if (dirElement.type != LightType::DirectionLight)
            {
                LOG_ERROR("No Directional Light Found");
                return;
            }


            shadowData.shadowProjView;
            shadowData.splitDepth;

            Matrix4 lightView = shadowData.lightMatrix;

            float width = (float)camera.outputTexture->GetWidth();
            float height = (float)camera.outputTexture->GetHeight();
            Matrix4 view = Inverse(camera.worldMatrix);

            // true for now
            int shadowEnabled = 1;


            shader->SetUniform("uboLights.ViewMatrix", &view);
            shader->SetUniform("uboLights.LightView", &lightView);

            std::string uniformname;
            for (int i = 0; i < shadowData.shadowProjView.size(); i++)
            {
                uniformname = "uboLights.ShadowTransform[" + std::to_string(i) + "]";
                shader->SetUniform(uniformname, shadowData.shadowProjView[i]);
            }
           
            for (int i = 0; i < shadowData.splitDepth.size(); i++)
            {
                uniformname = "uboLights.SplitDepths[" + std::to_string(i) + "]";
                shader->SetUniform(uniformname, shadowData.splitDepth[i]);
            }
           
            shader->SetUniform("uboLights.BiasMatrix", &shadowData.biasMatrix);
            shader->SetUniform("uboLights.LightSize", &shadowData.lightSize);

            shader->SetUniform("uboLights.ShadowFade", &shadowData.shadowFade);
            shader->SetUniform("uboLights.CascadeFade", &shadowData.cascadeFade);

            shader->SetUniform("uboLights.MaxShadowDist", &shadowData.maxShadowDistance);
            shader->SetUniform("uboLights.InitialBias", &shadowData.initialBias);

            shader->SetUniform("uboLights.Width", &width);
            shader->SetUniform("uboLights.Height", &height);

            shader->SetUniform("uboLights.castShadow", &shadowEnabled);
            shader->SetUniform("uboLights.ShadowCount", &shadowData.numShadowMaps);
            

            const int renderMode = 0;
            shader->SetUniform("renderMode", &renderMode);
            
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
            Vector3 Newposition = transformMatrix[3];

            //LOG_INFO("{0} : {1} : {2}",Newposition.x,Newposition.y,Newposition.z);

            // Matrix4 transformMatrix = transform.GetLocalMatrix();

             // get the inverse of the Camera trasfrom
            // Matrix4 view = (transformMatrix);
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

            camera.worldMatrix = transformMatrix;
            camera.viewProjMatrix = projView;
            camera.staticViewProjectMatrix = staticProjView;

            camera.outputTexture = cameraRef.GetRenderTexture();

            camera.gBuffer = cameraRef.GetBuffers()->gBuffer;
            camera.albedoTexture = cameraRef.GetBuffers()->albedoTexture;
            camera.normalTexture = cameraRef.GetBuffers()->normalTexture;
            camera.materialTexture = cameraRef.GetBuffers()->materialTexture;
            camera.depthTexture = cameraRef.GetBuffers()->depthTexture;


            camera.FOV = cameraRef.GetFOV();
            camera.camNear = cameraRef.GetZNear();
            camera.camFar = cameraRef.GetZFar();
            camera.frustum = cameraRef.GetFrustum(view);
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
            //size_t currentIndex = m_pipeline.lightElementList.size();  

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

            //lightElement.uniformName = "uboLights.lights[" + std::to_string(currentIndex) + "]";






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

            m_pipeline.rectangularObject.GetVAO()->UnBind();
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
