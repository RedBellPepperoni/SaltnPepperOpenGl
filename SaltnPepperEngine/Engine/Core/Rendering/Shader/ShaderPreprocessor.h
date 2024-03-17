#ifndef SHADERPREPROCESSOR_H
#define SHADERPREPROCESSOR_H
#include <string>
#include "Engine/Utils/Loading/FileSystem.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{

		class ShaderPreprocessor
		{
		private:

			std::string source;

#if defined(_DEBUG)
			std::vector<std::string> includeFilePaths;
			bool areIncludeFilePathsLoaded = false;
#endif

		public:

			ShaderPreprocessor(const std::string& shaderSource);
			ShaderPreprocessor& LoadIncludes(const FilePath& lookupPath);
			ShaderPreprocessor& EmitPrefixLine(const std::string& line);
			ShaderPreprocessor& EmitPostfixLine(const std::string& line);
			const std::vector<std::string>& GetIncludeFiles() const;
			const std::string& GetResult();

		};


	}
}

#endif // !SHADERPREPROCESSOR_H
