#include "ComputeShader.h"
#include "Engine/Utils/Logging/Log.h"
#include "Engine/Utils/GLUtils.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		template<>
		inline ComputeShader::BindableId ComputeShader::CreateShaderProgram(const std::string& source, const FilePath& path)
		{
			LOG_INFO("ComputeShader : compiling compute shader");
			ShaderId shaderId = BaseShader::CreateShader(GL_COMPUTE_SHADER, source, path);

			BindableId program = BaseShader::CreateProgram(&shaderId, 1);
			LOG_INFO("ComputeShader : created shader program with id = {0}" + std::to_string(program));

			BaseShader::DeleteShader(shaderId);

			return program;
		}

		void ComputeShader::Load(const std::string& path)
		{
			std::string source = FileSystem::ReadFileToString(path);
			BindableId program = ComputeShader::CreateShaderProgram(source, path);

			SetNewNativeHandle(program);
		}

	

		const std::string& ComputeShader::GetDebugFilePath() const
		{
			// TODO: insert return statement here
			// For testing

			return "";
		}
		const std::vector<std::string>& ComputeShader::GetIncludedFilePaths() const
		{
			// TODO: insert return statement here

			return std::vector<std::string>();
		}
	}
}