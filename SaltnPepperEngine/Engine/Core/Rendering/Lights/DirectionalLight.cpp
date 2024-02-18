#include "DirectionalLight.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Utils/Logging/Log.h"
#include "Engine/Core/System/Application/Application.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		Matrix4 DirectionalLight::ComputeCascadeMatrix(size_t cascadeIndex, float aspectRatio, float fov, const Matrix4& viewMatrix) const
		{
            float nearPlane = this->Projections[cascadeIndex];
            float farPlane = cascadeIndex + 1 < DirectionalLight::TextureCount ? this->Projections[cascadeIndex + 1] : 100000.0f;

            auto projection = GetReversePerspective(Radians(fov), aspectRatio, nearPlane, farPlane);
            auto invViewProj = Inverse(projection * viewMatrix);

            std::array corners = {
                Vector4(-1.0f, -1.0f, -1.0f, 1.0f),
                Vector4(-1.0f, -1.0f,  1.0f, 1.0f),
                Vector4(-1.0f,  1.0f, -1.0f, 1.0f),
                Vector4(-1.0f,  1.0f,  1.0f, 1.0f),
                Vector4(1.0f, -1.0f, -1.0f, 1.0f),
                Vector4(1.0f, -1.0f,  1.0f, 1.0f),
                Vector4(1.0f,  1.0f, -1.0f, 1.0f),
                Vector4(1.0f,  1.0f,  1.0f, 1.0f),
            };

            for (Vector4& corner : corners)
            {
                corner = invViewProj * corner;
                corner /= corner.w;
            }

            auto center = Vector3(0.0f);
            for (const auto& corner : corners)
            {
                center += Vector3(corner);
            }
            center /= corners.size();

            const auto lightView = GetViewMatrix(center + Normalize(this->Direction), center, Vector3(0.001f, 1.0f, 0.001f));

            Vector3 minVector = Vector3(std::numeric_limits<float>::max());
            Vector3 maxVector = Vector3(std::numeric_limits<float>::lowest());

            for (const auto& corner : corners)
            {
                Vector3 transformedCorner = lightView * corner;
                minVector = VectorMin(minVector, transformedCorner);
                maxVector = VectorMax(maxVector, transformedCorner);
            }

            minVector.z *= minVector.z < 0.0f ? this->DepthScale : 1.0f / this->DepthScale;
            maxVector.z *= maxVector.z > 0.0f ? this->DepthScale : 1.0f / this->DepthScale;

            auto shadowMapSize = float(this->DepthMap->GetHeight() + 1);
            auto worldUnitsPerTexel = (maxVector - minVector) / shadowMapSize;
            minVector = floor(minVector / worldUnitsPerTexel) * worldUnitsPerTexel;
            maxVector = floor(maxVector / worldUnitsPerTexel) * worldUnitsPerTexel;

            auto lightProjection = GetOrthographicMatrix(minVector.x, maxVector.x, minVector.y, maxVector.y, -maxVector.z, -minVector.z);
            return lightProjection * lightView;
		}


		DirectionalLight::DirectionalLight()
		{	
			// Set the Shadowmap to be 2K texels
			int depthTextureSize = 2048;
			DepthMap = MakeShared<Texture>();

			//Loading a new Depth Texture of the size 6144 X 2048
			DepthMap->LoadDepth(DirectionalLight::TextureCount * depthTextureSize, depthTextureSize);
		
			DepthMap->SetFilePath("DirectionalLight_Depth");

		}

        void DirectionalLight::UpdateLightCascades(float aspectRatio, float fov, const Matrix4& viewMatrix)
        {
            for (size_t i = 0; i < matrices.size(); i++)
            {
                this->matrices[i] = this->ComputeCascadeMatrix(i, aspectRatio, fov, viewMatrix);
            }

        }

	

		const Matrix4& DirectionalLight::GetMatrix(size_t index) const
		{
            if (index > this->matrices.size())
            {   
                LOG_ERROR("Directional Light: matrix index out of bound");
            }

            return this->matrices[index];
		}
	}
}