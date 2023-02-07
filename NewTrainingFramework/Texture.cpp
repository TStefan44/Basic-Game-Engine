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


	glGenTextures(1, &textureId);
	glBindTexture(tr->target, textureId);

	glTexParameteri(tr->target, GL_TEXTURE_MIN_FILTER, tr->min_filter);
	glTexParameteri(tr->target, GL_TEXTURE_MAG_FILTER, tr->mag_filter);

	glTexParameteri(tr->target, GL_TEXTURE_WRAP_S, tr->wrap_s);
	glTexParameteri(tr->target, GL_TEXTURE_WRAP_T, tr->wrap_t);

	array_pixels = LoadTGA(filePath.c_str(), &width, &height, &bpp);
	glTexImage2D(tr->target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, array_pixels);
}