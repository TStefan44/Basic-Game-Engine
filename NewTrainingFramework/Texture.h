#pragma once

#include "Resource.h"

#include "../Utilities/utilities.h"

namespace res {
	class Texture
	{
	public:
		Texture(TextureResource const* tr);
		Texture();
		~Texture();

		void Load();

		friend std::ostream& operator<<(std::ostream& os, const Texture& texture) {
			os << *texture.tr;
			return os;
		}

	public:
		GLuint textureId;

	private:
		TextureResource const* tr;
		char* array_pixels;
		int width;
		int height;
		int bpp;
	};
}