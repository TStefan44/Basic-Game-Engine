#pragma once

#include <string>
#include <fstream>

#include "../Utilities/utilities.h"

struct ModelResource {
	std::string folder_path;
	std::string file_name;

	ModelResource(std::string const& folder_path, std::string const& file_name)
		: folder_path(folder_path), file_name(file_name) {};

	friend std::ostream& operator<<(std::ostream& os, const ModelResource& model) {
		os << "Model : Folder Path " + model.folder_path + " Name " + model.file_name;
		return os;
	}
};

struct ShaderResource {
	std::string folder_path;
	std::string fileVertex_name;
	std::string fileFragment_name;

	ShaderResource(std::string const& folder_path, std::string const& fileVertex_name, std::string const& fileFragment_name)
		: folder_path(folder_path), fileVertex_name(fileVertex_name), fileFragment_name(fileFragment_name) {};

	friend std::ostream& operator<<(std::ostream& os, const ShaderResource& shader) {
		os << "Shader: Folder Path " + shader.folder_path
			+ " VertexShader :  " + shader.fileVertex_name
			+ " FragmentShader : " + shader.fileFragment_name;
		return os;
	}
};

struct TextureResource {
	std::string folder_path;
	std::string file_name;
	GLenum target;
	GLint min_filter;
	GLint mag_filter;
	GLint wrap_s;
	GLint wrap_t;

	TextureResource(std::string const& folder_path, std::string const& file_name, GLenum target,
		GLint min_filter, GLint mag_filter, GLenum wrap_s, GLenum wrap_t) :
		folder_path(folder_path), file_name(file_name), target(target), min_filter(min_filter), mag_filter(mag_filter),
		wrap_s(wrap_s), wrap_t(wrap_t) {}

	TextureResource(std::string const& folder_path, std::string const& file_name, std::string const& target,
		std::string const& min_filter, std::string const& mag_filter, std::string const& wrap_s, std::string const& wrap_t) :
		folder_path(folder_path), file_name(file_name) {
		if ("2d" == target) {
			this->target = GL_TEXTURE_2D;
		}
		else {
			this->target = GL_TEXTURE_CUBE_MAP;
		}

		{
			if (wrap_s.compare("CLAMP_TO_EDGE") == 0) {
				this->wrap_s = GL_CLAMP_TO_EDGE;
			}
			else if (wrap_s.compare("GL_MIRRORED_REPEAT") == 0) {
				this->wrap_s = this->wrap_s = GL_MIRRORED_REPEAT;;
			}
			else {
				this->wrap_s = GL_REPEAT;
			}

			if (wrap_t.compare("CLAMP_TO_EDGE") == 0) {
				this->wrap_t = GL_CLAMP_TO_EDGE;
			}
			else if (wrap_t.compare("GL_MIRRORED_REPEAT") == 0) {
				this->wrap_t = this->wrap_t = GL_MIRRORED_REPEAT;;
			}
			else {
				this->wrap_t = GL_REPEAT;
			}
		}

		{
			if (min_filter.compare("GL_NEAREST") == 0) {
				this->min_filter = GL_NEAREST;
			}
			else {
				this->min_filter = GL_LINEAR;
			}

			if (mag_filter.compare("GL_NEAREST") == 0) {
				this->mag_filter = GL_NEAREST;
			}
			else {
				this->mag_filter = GL_LINEAR;
			}
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const TextureResource& texture) {
		os << "Texture : Folder path " + texture.folder_path + " Name " + texture.file_name
			+ " Min filter: " << texture.min_filter << " Mag filter: " << texture.mag_filter
			<< " Wrap_s: " << texture.wrap_s << " Wrap_t: " << texture.wrap_t;
		return os;
	}
};

std::string const mainFolder = "../Resources/";