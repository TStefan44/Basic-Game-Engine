#pragma once

#include "Resource.h"
#include <vector>

#include "../Utilities/utilities.h"

namespace res {
	class Texture
	{
	public:
		// Constructors
		Texture(TextureResource const* tr);
		Texture();
		~Texture();

		void Load();

		friend std::ostream& operator<<(std::ostream& os, const Texture& texture) {
			os << *texture.tr;
			return os;
		}

		GLenum getTarget() const;

	private:
		// Load texture by type functions
		void LoadTexture2D();
		void LoadTextureCube();

		char* ExtractFace(int row, int colomn, int width, int height, char* image);

		//Class variables
	public:
		GLuint textureId;

	private:
		TextureResource const* tr;
		char* array_pixels;

		GLint format;
		int width;
		int height;
		int bpp;
	};
}