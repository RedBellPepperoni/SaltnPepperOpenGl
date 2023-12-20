#include "FileSystem.h"
#include <sstream>
#include <map>

#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{


#define ROOTPATH = ".."

	std::map<int, int> FileModeTable =
	{
		{ FileSystem::READ, std::fstream::in},
		{ FileSystem::WRITE, std::fstream::out},
		{ FileSystem::BINARY, std::fstream::binary},
		{ FileSystem::READ | FileSystem::BINARY, std::fstream::in | std::fstream::binary },
		{ FileSystem::WRITE | FileSystem::BINARY, std::fstream::out | std::fstream::binary },
		{ FileSystem::READ | FileSystem::WRITE, std::fstream::in | std::fstream::out },
		{ FileSystem::READ | FileSystem::WRITE | FileSystem::BINARY, std::fstream::in | std::fstream::out | std::fstream::binary }
	};




	FileSystem::FileSystem(const FilePath& path, int mode)
	{
		Open(path, mode);
	}

	FileSystem::FileSystem(const std::string& path, int mode)
	{
		Open(path, mode);
	}
	FileStream& FileSystem::GetStream()
	{
		return m_fileStream;
	}
	bool FileSystem::IsOpen() const
	{
		return m_fileStream.is_open() && m_fileStream.good();
	}

	void FileSystem::Open(FilePath path, int mode)
	{
		m_filePath = std::move(path);

		if (!FileSystem::Exists(m_filePath))
		{
			if ((mode & FileSystem::WRITE) == 0)
			{
				LOG_CRITICAL("File : file not found [{0}]", m_filePath.string());
				return;
			}
			else
			{
				FilePath directory = m_filePath.parent_path();
				if (!directory.empty() && !FileSystem ::Exists(directory))
				{
					FileSystem::CreateDir(directory);
				}
			}
		}

		if (IsOpen()) { Close(); }
		m_fileStream.open(m_filePath, (std::ios_base::openmode)FileModeTable[mode]);
	}

	void FileSystem::Open(const std::string& path, int mode)
	{
		FilePath filepath = GetFilePathfromString(path); 
		Open(std::move(filepath), mode);
	}

	void FileSystem::Close()
	{
		m_fileStream.close();
	}

	void FileSystem::WriteBytes(const uint8_t* bytes, size_t size)
	{
		m_fileStream.write((const char*)bytes, size);
	}

	void FileSystem::ReadBytes(uint8_t* bytes, size_t size)
	{
		m_fileStream.read((char*)bytes, size);
	}

	FilePath FileSystem::GetFilePathfromString(const std::string& stringPath)
	{
		// making sure the encoding is correct, otherwise the path points to the wrong file in rare cases
		return std::filesystem::u8path(stringPath);
	}

	bool FileSystem::Exists(const FilePath& path)
	{
		return std::filesystem::exists(path);
		
	}
	bool FileSystem::Exists(const std::string& stringPath)
	{
		return std::filesystem::exists(GetFilePathfromString(stringPath));
	}
	std::string FileSystem::ReadFileToString(const FilePath& path)
	{
		// Checking if the File exists, otherwise returning a null string
		if (!Exists(path))
		{
			return std::string();
		}


		// Opening a string stream
		std::ifstream stream(path);

		// Creating a buffer to store the data
		std::stringstream buffer;

		// Storing the stream data into the buffer line by line
		buffer << stream.rdbuf();

		// done with the file, so closing
		stream.close();



		// Outputting buffer data in form of a string
		return buffer.str();


	}

	std::string FileSystem::GetFileName(const FilePath& path)
	{
		return path.stem().string();
	}

	std::string FileSystem::GetFileNameWithExt(const FilePath& path)
	{
		return path.filename().string();
	}

	bool FileSystem::WriteFileToText(const FilePath& path, const std::string& text)
	{

		std::ofstream stream(path, std::ios::binary | std::ios::trunc);

		if (!stream)
		{
			stream.close();
			return false;
		}

		stream.write((char*)&text[0], (uint32_t)text.size());
		stream.close();


		return true;
	}

	FilePath FileSystem::GetCurrentPath()
	{
		return std::filesystem::current_path();
	}


	FilePath FileSystem::GetEngineDir()
	{
		return GetCurrentPath() /= "Engine\\";
	}
	FilePath FileSystem::GetShaderDir()
	{
		return GetEngineDir() /= "Shaders\\";
	}

	void FileSystem::CreateDir(const FilePath& path)
	{
		if (!FileSystem::Exists(path))
		{
			std::filesystem::create_directory(path);
		}
	}

	void FileSystem::CreateDir(const std::string& path)
	{
		if (!FileSystem::Exists(path))
		{
			std::filesystem::create_directory(GetFilePathfromString(path));
		}
	}
}