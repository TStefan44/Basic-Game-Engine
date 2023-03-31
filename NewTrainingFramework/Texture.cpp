#pragma once
#include "stdafx.h"
#include "Texture.h"

using namespace res;

Texture::Texture(TextureResource const* tr) : tr(tr) {
	textureId = 0;

	Load();
}

Texture::Texture() : Texture(nullptr) {}

void Texture::Load() {
	// Extract file path
	if (tr == nullptr) {
		std::cerr << "NULL pointer to texture resource!\n";
		return;
	}
	std::string filePath = mainFolder + tr->folder_path + tr->file_name;

	// Create and set texture parameters
	glGenTextures(1, &textureId);
	glBindTexture(tr->target, textureId);

	glTexParameteri(tr->target, GL_TEXTURE_MIN_FILTER, tr->min_filter);
	glTexParameteri(tr->target, GL_TEXTURE_MAG_FILTER, tr->mag_filter);

	glTexParameteri(tr->target, GL_TEXTURE_WRAP_S, tr->wrap_s);
	glTexParameteri(tr->target, GL_TEXTURE_WRAP_T, tr->wrap_t);

	// Extract data from file
	array_pixels = LoadTGA(filePath.c_str(), &width, &height, &bpp);

	if (array_pixels == NULL) {
		std::cerr << "Can't open " << filePath << "\n";
		return;
	}

	// Set format data
	if (bpp == 24)
		format = GL_RGB;
	else
		format = GL_RGBA;

	// Load texture by type
	switch (tr->target) {
	case GL_TEXTURE_2D: LoadTexture2D(); break;
	case GL_TEXTURE_CUBE_MAP: LoadTextureCube(); break;
	default:
		std::cerr << "Unknown target given\n";
	}

	delete[]array_pixels;
}

/*
* Function to load a simple 2D texture 
*/
void Texture::LoadTexture2D() {
	glTexImage2D(tr->target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, array_pixels);
}

/*
* Function to load a cube texture.
*/
void Texture::LoadTextureCube() {
	int face_width = width / 4;
	int face_height = height / 3;

	std::vector<Vector2> pos = {
		Vector2(1, 2), // posX
		Vector2(1, 0), // negX
		Vector2(0, 1), // posY
		Vector2(2, 1), // negY
		Vector2(1, 1), // posZ
		Vector2(1, 3)  // negZ
	};
	
	// Load an cube face in the image buffer
	for (int i = 0; i < 6; i++) {
		char* data = ExtractFace(pos[i].x, pos[i].y, face_width, face_height, array_pixels);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, face_width, face_height, 0, format, GL_UNSIGNED_BYTE, data);
		delete[] data;
	}
}

/*
* Extract a face from the main image. Cube configuration:
*  * 2 * *
*  1 4 0 5
*  * 3 * *
*/
char* Texture::ExtractFace(int row, int colomn, int width_f, int height_f, char* image) {
	int size_texel = bpp / 8;
	char* sub_image = new char[width_f * height_f * size_texel];

	int startX = width_f * colomn;
	int startY = height_f * row;
	int startByte = (startY * width + startX) * size_texel;

	for (int y = 0; y < height_f; y++) {
		int rowStartByte = startByte + y * width * size_texel;
		for (int x = 0; x < width_f; x++) {
			int srcByte = rowStartByte + x * size_texel;
			int dstByte = (y * width_f + x) * size_texel;

			for (int k = 0; k < size_texel; k++) {
				sub_image[dstByte + k] = image[srcByte + k];
			}
		}
	}

	return sub_image;
}

GLenum Texture::getTarget() const {
	return tr->target;
}