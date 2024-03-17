#ifndef COMPUTESHADER_H
#define COMPUTESHADER_H

#include "Engine/Core/Rendering/Shader/BaseShader.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		class ComputeShader : public BaseShader
		{

			template<typename FilePath> static BindableId CreateShaderProgram(const std::string& source, const FilePath& path);
		
		public:

			void Load(const std::string& path);
			const std::string& GetDebugFilePath() const;
			const std::vector<std::string>& GetIncludedFilePaths() const;
		};

	

	}
}


#endif // !COMPUTESHADER_H


