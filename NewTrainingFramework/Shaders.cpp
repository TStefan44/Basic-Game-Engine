#pragma once
#include "stdafx.h"
#include "Shaders.h"

using namespace res;

Shader::Shader(ShaderResource const* sr) : sr(sr) {
	if (sr == nullptr)
		return;

	if (Load() == 0) {
		LoadLocationsAttributes();
	}
}

Shader::Shader() : Shader(nullptr) {}

/*
* Create a shader program form the given files location
*/
int Shader::Load()
{
	std::string fileVertexShader = mainFolder + sr->folder_path + sr->fileVertex_name;
	std::string fileFragmentShader = mainFolder + sr->folder_path + sr->fileFragment_name;

	char* vertexShader_c = new char[fileVertexShader.size() + 1];
	char* fragmentShader_c = new char[fileFragmentShader.size() + 1];

	strcpy(vertexShader_c, fileVertexShader.c_str());
	strcpy(fragmentShader_c, fileFragmentShader.c_str());

	vertexShader = esLoadShader(GL_VERTEX_SHADER, vertexShader_c);

	if (vertexShader == 0) {
		std::cerr << "Cannot load " << vertexShader_c << "\n";
		delete[] vertexShader_c;
		delete[] fragmentShader_c;
		return -1;
	}

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fragmentShader_c);

	if (fragmentShader == 0)
	{
		std::cerr << "Cannot load " << fragmentShader_c << "\n";
		delete[] vertexShader_c;
		delete[] fragmentShader_c;
		glDeleteShader(vertexShader);
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	delete[] vertexShader_c;
	delete[] fragmentShader_c;

	return 0;
}

/*
* Set essential shader locations. Called if the shader program was
* created with success.
*/
void Shader::LoadLocationsAttributes() {
	positionAttribute = glGetAttribLocation(program, "a_posL");
	colorAttribute = glGetAttribLocation(program, "a_colL");
	uvAttribute = glGetAttribLocation(program, "a_uv");
	normalAttribute = glGetAttribLocation(program, "a_normal");

	modelMatrix = glGetUniformLocation(program, "u_model");
	viewMatrix = glGetUniformLocation(program, "u_view");
	perspectiveMatrix = glGetUniformLocation(program, "u_persp");

	textureUniform_0 = glGetUniformLocation(program, "u_texture_0");
	textureUniform_1 = glGetUniformLocation(program, "u_texture_1");
	textureUniform_2 = glGetUniformLocation(program, "u_texture_2");
	textureUniform_3 = glGetUniformLocation(program, "u_texture_3");
	textureUniform_4 = glGetUniformLocation(program, "u_texture_4");

	fogClear_r = glGetUniformLocation(program, "u_fog_r");
	fogTrans_R = glGetUniformLocation(program, "u_fog_R");
	fogColor = glGetUniformLocation(program, "u_fogColor");


	material_kd = glGetUniformLocation(program, "u_material_kd");
	material_ks = glGetUniformLocation(program, "u_material_ks");
	material_shininess = glGetUniformLocation(program, "u_material_shininess");
	light_number = glGetUniformLocation(program, "u_light_number");
	ratio = glGetUniformLocation(program, "u_ratio");
	colorAmbientalGlobal = glGetUniformLocation(program, "u_colorAmbientalGlobal");

	cameraPos = glGetUniformLocation(program, "u_cameraPos");
}

Shader::~Shader()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}