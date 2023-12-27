#include "Camera.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/Resources/ResourceManager.h"

namespace SaltnPepperEngine
{
    namespace Rendering
    {

        Camera::Camera()
            : m_aspectRatio(16.0f / 9.0f)
            , m_near(0.01f)
            , m_far(1000.0f)
            , m_fov(60.0f)
            , m_orthographic(false)
        {
            // InitializeRenderTexture();

        }
        Camera::Camera(float FOV, float Near, float Far, float Aspect)
            : m_aspectRatio(Aspect)
            , m_fov(FOV)
            , m_near(Near)
            , m_far(Far)
            , m_orthographic(false)
        {
            // InitializeRenderTexture();
        }

        Camera::Camera(float Aspect, float Near, float Far)
            : m_aspectRatio(Aspect)
            , m_fov(60.0f)
            , m_near(Near)
            , m_far(Far)
            , m_orthographic(false)
        {
            // InitializeRenderTexture();
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


        SharedPtr<Texture>& Camera::GetRenderTexture()
        {
            return m_renderTexture;
        }

        void Camera::InitializeRenderTexture()
        {
            Vector2Int viewport = Application::GetCurrent().GetWindowSize();

            m_renderTexture = Factory<Texture>::Create();
            m_renderTexture->Load(nullptr, viewport.x, viewport.y, 3, false, TextureFormat::RGB);
            m_renderTexture->SetWarping(TextureWraping::CLAMP_TO_EDGE);

        }
    }
}