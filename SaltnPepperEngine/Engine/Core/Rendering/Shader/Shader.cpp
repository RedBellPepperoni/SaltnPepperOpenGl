#include "Shader.h"
#include "Engine/Utils/GLUtils.h"
#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		std::array OpenGLShadertype =
		{
			GL_VERTEX_SHADER,
			GL_GEOMETRY_SHADER,
			GL_FRAGMENT_SHADER
		};

		std::array GetShaderTypeString =
		{
			"vertex",
			"geometry",
			"fragment"
		};


		struct ShaderSubInfo
		{
			Shader::ShaderType type;
			std::string sourceCode;
			FilePath path;
		};

		Shader::BindableId Shader::CreateShaderProgram(ShaderId* shaderIds, const ShaderSubInfo* stageInfos, size_t count)
		{
			// create each shader type
			for (size_t i = 0; i < count; i++)
			{
				auto& stageInfo = stageInfos[i];

				std::string fileName = FileSystem::GetFileName(stageInfo.path);
				LOG_INFO("Shader : compiling " + std::string(GetShaderTypeString[stageInfo.type]) + " shader : from [{0}]",fileName);
				shaderIds[i] = BaseShader::CreateShader(OpenGLShadertype[stageInfo.type], stageInfo.sourceCode, stageInfo.path);
			}

			// link stages into one shader program
			BindableId program = BaseShader::CreateProgram(shaderIds, count);
			LOG_INFO("Shader : created shader program with id : {0} " , std::to_string(program));

			// delete separate shader stages
			for (size_t i = 0; i < count; i++)
			{
				BaseShader::DeleteShader(shaderIds[i]);
			}

			return program;
		}

		void Shader::LoadDebugVariables(const ShaderSubInfo* stageInfos, size_t count)
		{
#if defined(_DEBUG)
			
#endif
		}

		bool Shader::Load(const std::string& vertexPath, const std::string& fragmentPath)
		{
			if (!FileSystem::Exists(vertexPath))
			{
				std::string errorPath = vertexPath;

				LOG_WARN("Vertex Shader not found : {0}", errorPath);

				return false;
			}
				
			if (!FileSystem::Exists(fragmentPath))
			{
				LOG_ERROR("Fragment Shader not found : {0}", fragmentPath);
				return false;
			}
				
			// Setting vertex and fragment data
			std::array subInfo =
			{
				ShaderSubInfo
				{	// Defining the type
					ShaderType::VERTEX,
					// Pulling the Shader vertex data into a continuos string
					FileSystem::ReadFileToString(vertexPath),
					// Storing the path
					FileSystem::GetFilePathfromString(vertexPath)
				},
				ShaderSubInfo
				{
					// Defining the typr
					ShaderType::FRAGMENT,
					// Pulling the Shader fragment data into a continuos string
					FileSystem::ReadFileToString(fragmentPath),
					// Storing the path
					FileSystem::GetFilePathfromString(fragmentPath)
				}
			};

			const size_t subShaderCount = subInfo.size();
			std::array<ShaderId, subShaderCount> ids;
				
			// Generating a program id from the given sub shader infos
			uint32_t glProgram = CreateShaderProgram(ids.data(), subInfo.data(), subShaderCount);


			SetNewNativeHandle(glProgram);

			return true;
							
		}

		bool Shader::Load(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath)
		{
			LOG_ERROR("Geometry Shader Loading not implemented");
			return false;
		}

		
		const std::string& Shader::GetDebugFilePath(Shader::ShaderType stage) const
		{
#if defined(_DEBUG)
			return debugFilePaths[(size_t)stage];
#else
			const static std::string EmptyFilePath;
			return EmptyFilePath;
#endif
		}
		const std::vector<std::string>& Shader::GetIncludedFilePaths() const
		{
#if defined(_DEBUG)
			return includedFilePaths;
#else
			const static std::vector<std::string> EmptyVector;
			return EmptyVector;
#endif
		}
	}
}