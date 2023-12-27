#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "Engine/Core/Rendering/Textures/Image.h"

namespace SaltnPepperEngine
{
	using Rendering::Image;


	class ImageLoader
	{
		//loads image from disk.
		// OpenGL makes all images vertically
	public:

		template<typename FilePath>
		static Image LoadImageFromFile(const FilePath& path, bool flipImage = true);

		// loads Image from memory (returns nullptr if no image is found at the location)
		static Image LoadImageFromMemory(const uint8_t* memory, size_t byteSize, bool flipImage = true);



	};
}

#endif // !IMAGELOADER_H


