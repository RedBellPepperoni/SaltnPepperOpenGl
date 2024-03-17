#include "ShaderPreprocessor.h"
#include "Engine/Utils/Logging/Log.h"
#include <regex>

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		ShaderPreprocessor::ShaderPreprocessor(const std::string& shaderSource)
			: source(shaderSource)
		{
		}

		std::vector<std::pair<std::string, std::string>> FindAllIncludePaths(const std::string& source)
		{
			std::vector<std::pair<std::string, std::string>> includes;

			std::regex r(R"(#include\s+\"(.+)\")");
			std::sregex_iterator pathIt(source.begin(), source.end(), r);

			for (; pathIt != std::sregex_iterator(); pathIt++)
			{
				std::string includeCommand(source.begin() + pathIt->position(0), source.begin() + pathIt->position(0) + pathIt->length(0));
				std::string filepath(source.begin() + pathIt->position(1), source.begin() + pathIt->position(1) + pathIt->length(1));
				includes.emplace_back(std::move(filepath), std::move(includeCommand));
			}

			return includes;
		}

		ShaderPreprocessor& ShaderPreprocessor::LoadIncludes(const FilePath& lookupPath)
		{
#if defined(_DEBUG)
			areIncludeFilePathsLoaded = true;
#endif


			auto paths = FindAllIncludePaths(source);
			for (const auto& [path, include] : paths)
			{
				auto filepath = lookupPath / path.c_str();
				if (!FileSystem::Exists(filepath))
				{
					LOG_ERROR("ShaderPreprocessor : included file was not found: " + path);
					return *this;
				}

#if defined(_DEBUG)
				includeFilePaths.push_back(path);
#endif

				auto it = source.find(include);
				if (it != source.npos) source.erase(it, include.size());

				source = FileSystem::ReadFileToString(filepath) + source;
			}
			// maybe there are new includes in included files
			if (!paths.empty())
			{
				LoadIncludes(lookupPath);
			}

			return *this;
		}
		ShaderPreprocessor& ShaderPreprocessor::EmitPrefixLine(const std::string& line)
		{
			source = line + '\n' + source;
			return *this;
		}
		ShaderPreprocessor& ShaderPreprocessor::EmitPostfixLine(const std::string& line)
		{
			source += '\n';
			source += line;
			return *this;
		}

		std::vector<std::string> emptyFilePathList;


		const std::vector<std::string>& ShaderPreprocessor::GetIncludeFiles() const
		{
#if defined(_DEBUG)
			if (!this->areIncludeFilePathsLoaded)
			{
				LOG_WARN("ShaderPreprocessor : included filepaths are not loaded as LoadIncludes() was not called");
			}
			return this->includeFilePaths;
#else
			LOG_WARN("ShaderPreprocessor : included filepaths are not saved in non-debug build");
			return emptyFilePathList;
#endif
		}

		const std::string& ShaderPreprocessor::GetResult()
		{
			return source;
		}
	}
}