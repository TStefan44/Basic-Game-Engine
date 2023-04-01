#pragma once

#include "Resource.h"

#include "../Utilities/utilities.h"

namespace res {
	class Shader
	{
	public:
		// Constructors
		Shader(ShaderResource const* sr);
		Shader();
		~Shader();

		// Core functions
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
		GLint normalAttribute;

		GLint modelMatrix;
		GLint viewMatrix;
		GLint perspectiveMatrix;

		GLint textureUniform_0;
		GLint textureUniform_1;
		GLint textureUniform_2;
		GLint textureUniform_3;
		GLint textureUniform_4;

		GLint fogClear_r;
		GLint fogTrans_R;
		GLint fogColor;

		GLint ratio;
		GLint colorAmbientalGlobal;
		GLint material_kd;
		GLint material_ks;
		GLint material_shininess;
		GLint light_number;

		GLint cameraPos;

	private:
		ShaderResource const* sr;
		GLuint vertexShader;
		GLuint fragmentShader;
	};
}