#ifndef FILESYSTEM_H
#define FILESYSTEM_H


#include <filesystem>
#include <fstream>

namespace SaltnPepperEngine
{

	using FilePath = std::filesystem::path;

	using FileStream = std::fstream;


	// Wrapper around the std file system
	class FileSystem
	{

		FileStream m_fileStream;

		FilePath m_filePath;

	public:

		enum FileMode
		{
			READ = 0x1,
			WRITE = 0x2,
			BINARY = 0x4
		};

		FileSystem() = default;

		FileSystem(const FilePath& path, int mode = FileMode::READ);

		FileSystem(const std::string& path, int mode = FileMode::READ);

		FileStream& GetStream();

		bool IsOpen() const;

		void Open(FilePath path, int mode = FileMode::READ);
		void Open(const std::string& path, int mode = FileMode::READ);

		void Close();


		void WriteBytes(const uint8_t* bytes, size_t size);

		void ReadBytes(uint8_t* bytes, size_t size);


		static FilePath GetFilePathfromString(const std::string& stringPath);

		// Check if a file exists at the given path
		static bool Exists(const FilePath& path);

		static bool Exists(const std::string& stringPath);


		// Get data from the file at the context path as a string
		static std::string ReadFileToString(const FilePath& path);



		static std::string GetFileName(const FilePath& path);
		static std::string GetFileNameWithExt(const FilePath& path);

		// Write given string to a Text file
		static bool WriteFileToText(const FilePath& path, const std::string& text);

		static FilePath GetCurrentPath();

		static FilePath GetEngineDir();
		static FilePath GetShaderDir();


		static void CreateDir(const FilePath& path);
		static void CreateDir(const std::string& path);

	};

}

#endif // !FILESYSTEM_H