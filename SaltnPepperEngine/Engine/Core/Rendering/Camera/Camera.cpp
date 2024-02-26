#include "Camera.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/Rendering/Buffers/FrameBuffer.h"
#include "Engine/Core/Resources/ResourceManager.h"
#include "Engine/Utils/Ray.h"

namespace SaltnPepperEngine
{
    namespace Rendering
    {

        Camera::Camera()
            : m_aspectRatio(16.0f / 10.0f)
            , m_near(0.01f)
            , m_far(1000.0f)
            , m_fov(60.0f)
            , m_orthographic(false)
        {
           
             InitializeRenderTexture();


        }
        Camera::Camera(float FOV, float Near, float Far, float Aspect)
            : m_aspectRatio(Aspect)
            , m_fov(FOV)
            , m_near(Near)
            , m_far(Far)
            , m_orthographic(false)
        {
             InitializeRenderTexture();

        }

        Camera::Camera(float Aspect, float Near, float Far)
            : m_aspectRatio(Aspect)
            , m_fov(60.0f)
            , m_near(Near)
            , m_far(Far)
            , m_orthographic(false)
        {
             InitializeRenderTexture();
        }


        const Matrix4& Camera::GetProjectionMatrix()
        {

            UpdateProjectionMatrix();

            return m_projection;
        }


        const float Camera::GetZoom() const
        {
            return m_zoom;
        }
        void Camera::SetZoom(float zoom)
        {
            m_zoom = zoom;
            m_shouldUpdateProjection = true;
        }
        void Camera::SetAspectRatio(float m_width, float m_height)
        {
            SetAspectRatio(m_width / m_height);
        }
        void Camera::SetAspectRatio(float m_aspectRatio)
        {
            m_aspectRatio = m_aspectRatio;
            m_shouldUpdateProjection = true;
        }
        const float Camera::GetAspectRatio() const
        {
            return m_aspectRatio;
        }

        void Camera::SetOrthoSize(float size)
        {
            m_orthosize = size;
            m_shouldUpdateProjection = true;
        }

        const float Camera::GetOrthoSize() const
        {
            return m_orthosize;
        }

        const float Camera::GetZFar() const
        {
            return m_far;
        }
        void Camera::SetZFar(float zFar)
        {
            m_far = zFar;
            m_shouldUpdateProjection = true;
        }
        const float Camera::GetZNear() const
        {
            return m_near;
        }
        void Camera::SetZNear(float zNear)
        {
            m_near = zNear;
            m_shouldUpdateProjection = true;
        }

        void Camera::SetOrthographic(bool ortho)
        {
            m_orthographic = ortho;
            m_shouldUpdateProjection = true;
        }

        const bool Camera::IsOrthographic() const
        {
            return m_orthographic;
        }

        const float Camera::GetFOV() const
        {
            return m_fov;
        }

        void Camera::SetFOV(float newFOV)
        {
            m_fov = newFOV;
        }

        void Camera::SetDirection(const Vector3 vector)
        {
            m_direction = Math::Normalize(vector);
        }

        const Vector3& Camera::GetDirection()
        {
            m_direction = Math::Normalize(m_direction);
            return m_direction;
        }

        const Vector3& Camera::GetUpVector() const
        {
            return m_upVector;
        }

        const Vector3& Camera::GetRightVector() const
        {
            return m_rightVector;
        }

        const Vector3& Camera::GetForwardVector() const
        {
            return m_forwardVector;
        }



        void Camera::UpdateProjectionMatrix()
        {
            if (m_orthographic)
            {

                m_projection = GetOrthographicMatrix(-m_aspectRatio * m_orthosize, m_aspectRatio * m_orthosize, -m_orthosize, m_orthosize, m_near, m_far);
            }

            else
            {
                m_projection = GetPerspectiveMatrix(m_fov, m_aspectRatio, m_near, m_far);

                //m_projection = Math::CalculateReversePerspective(m_fov, m_aspectRatio, m_near, m_far);
            }
        }


        SharedPtr<Texture> Camera::GetRenderTexture()
        {
            //return m_renderTexture;
            return m_cameraBuffers->albedoTexture;
        }

        Ray Camera::GetRay(float xPos, float yPos, Matrix4 viewMatrix, bool flipY)
        {
            Ray returnRay;
            Matrix4 inverseViewProj = Inverse(m_projection * viewMatrix);
        
            xPos = 2.0f * xPos - 1.0f;
            yPos = 2.0f * yPos - 1.0f;

            if(flipY)
            {
                yPos *= -1.0f;
            }

            Vector4 n = inverseViewProj * Vector4(xPos, yPos, 0.0f, 1.0f);
            n /= n.w;

            Vector4 f = inverseViewProj * Vector4(xPos, yPos, 1.0f, 1.0f);
            f /= f.w;

            returnRay.m_origin = Vector3(n);
            returnRay.m_direction = Normalize(Vector3(f) - returnRay.m_origin);

            return returnRay;
        
        }

        Frustum& Camera::GetFrustum(const Matrix4 viewMatrix)
        {
            if (m_shouldUpdateProjection)
            {
                UpdateProjectionMatrix();
                m_shouldUpdateProjection = false;
            }

            m_frustum.Define(m_projection, viewMatrix);
            m_shouldUpdateFrustum = false;

            return m_frustum;
        }

        SharedPtr<CameraBuffers>& Camera::GetBuffers()
        {
           return m_cameraBuffers;
        }

        void Camera::InitializeRenderTexture()
        {

            Vector2Int viewport = Application::GetCurrent().GetWindowSize();

            m_renderTexture = Factory<Texture>::Create();
            m_renderTexture->Load(nullptr, viewport.x, viewport.y, 3, false, TextureFormat::RGB);
            m_renderTexture->SetWarping(TextureWraping::CLAMP_TO_EDGE);

            m_cameraBuffers = MakeShared<CameraBuffers>();
            m_cameraBuffers->Init(viewport.x, viewport.y);

        }

      


        void CameraBuffers::Init(int width, int height)
        {
            gBuffer = MakeShared<FrameBuffer>();
            albedoTexture = MakeShared<Texture>();
            normalTexture = MakeShared<Texture>();
            depthTexture = MakeShared<Texture>();
            materialTexture = MakeShared<Texture>();
            

            Resize(width,height);


            gBuffer->AttachTexture(albedoTexture, Attachment::COLOR_ATTACH_0);
            gBuffer->AttachTextureExtra(normalTexture, Attachment::COLOR_ATTACH_1);
            gBuffer->AttachTextureExtra(materialTexture, Attachment::COLOR_ATTACH_2);
            gBuffer->AttachTextureExtra(depthTexture, Attachment::DEPTH_ATTACH);

         
            std::vector attachments = {
                 Attachment::COLOR_ATTACH_0,
                 Attachment::COLOR_ATTACH_1,
                 Attachment::COLOR_ATTACH_2,
            };

            
            gBuffer->UseDrawBuffers(attachments);
            gBuffer->Validate();


        }
        void CameraBuffers::Resize(int width, int height)
        {

            albedoTexture->Load(nullptr, width, height, 3, false, TextureFormat::RGBA);
            albedoTexture->SetFilePath("camera albedo");
            albedoTexture->SetWarping(TextureWraping::CLAMP_TO_EDGE);

            normalTexture->Load(nullptr, width, height, 3, false, TextureFormat::RGBA16);
            normalTexture->SetFilePath("camera normal");
            normalTexture->SetWarping(TextureWraping::CLAMP_TO_EDGE);

            materialTexture->Load(nullptr, width, height, 3, false, TextureFormat::RGBA);
            materialTexture->SetFilePath("camera material");
            materialTexture->SetWarping(TextureWraping::CLAMP_TO_EDGE);

            depthTexture->LoadDepth(width, height, TextureFormat::DEPTH32F);
            depthTexture->SetFilePath("camera depth");
            depthTexture->SetWarping(TextureWraping::CLAMP_TO_EDGE);

        }
    }
}