#pragma once

#include "Resource.h"

#include "../Utilities/utilities.h"

namespace res {
	class Shader
	{
	public:
		Shader(ShaderResource const* sr);
		Shader();
		~Shader();

		int Load();

		friend std::ostream& operator<<(std::ostream& os, const Shader& shader) {
			os << *shader.sr;
			return os;
		}

		// class variables
	private:
		ShaderResource const* sr;
		GLuint program;
		GLuint vertexShader;
		GLuint fragmentShader;
	};
}