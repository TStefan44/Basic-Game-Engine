#pragma once

#include "../Utilities/utilities.h"

#include "Resource.h"

namespace res {
	class Shader
	{
	public:
		Shader(ShaderResource const* sr);
		Shader();
		~Shader();

		int Load();
		void LoadLocationsAttributes();

		friend std::ostream& operator<<(std::ostream& os, const Shader& shader) {
			os << *shader.sr;
			return os;
		}

		// class variables

	public:
		GLuint program;

		GLint positionAttribute;
		GLint colorAttribute;
		GLint uvAttribute;

		GLint modelMatrix;
		GLint viewMatrix;
		GLint perspectiveMatrix;

		// TODO: Use a vector in the future
		GLint textureUniform_0;
		GLint textureUniform_1;
		GLint textureUniform_2;

	private:
		ShaderResource const* sr;
		GLuint vertexShader;
		GLuint fragmentShader;
	};
}