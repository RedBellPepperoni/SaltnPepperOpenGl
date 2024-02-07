#ifndef SHADER_H
#define SHADER_H

#include "Engine/Core/Rendering/Shader/BaseShader.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		struct ShaderSubInfo;

		class Shader : public BaseShader
		{
		public:

			enum ShaderType
			{
				VERTEX,				// 1 
				GEOMETRY,			// 2
				FRAGMENT,			// 3
				
				MAX_COUNT // This also defines the total number of Shadertypes
			};

#if defined (_DEBUG)
			std::array<std::string, ShaderType::MAX_COUNT> debugFilePaths;
			std::vector<std::string> includedFilePaths;
#endif

			static BindableId CreateShaderProgram(ShaderId* shaderIds, const ShaderSubInfo* stageInfos, size_t count);
			void LoadDebugVariables(const ShaderSubInfo* stageInfos, size_t count);
		public:
			bool Load(const std::string& vertexPath, const std::string& fragmentPath);
			bool Load(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath);
			
			const std::string& GetDebugFilePath(Shader::ShaderType stage) const;
			const std::vector<std::string>& GetIncludedFilePaths() const;

		};


		

	}
}



#endif // !SHADER_H

