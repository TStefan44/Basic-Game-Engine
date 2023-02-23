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

int Shader::Load()
{
	std::string fileVertexShader = mainFolder + sr->folder_path + sr->fileVertex_name;
	std::string fileFragmentShader = mainFolder + sr->folder_path + sr->fileFragment_name;

	char * vertexShader_c = new char[fileVertexShader.size() + 1];
	char * fragmentShader_c = new char[fileFragmentShader.size() + 1];

	strcpy(vertexShader_c, fileVertexShader.c_str());
	strcpy(fragmentShader_c, fileFragmentShader.c_str());

	vertexShader = esLoadShader(GL_VERTEX_SHADER, vertexShader_c);

	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fragmentShader_c);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	delete vertexShader_c;
	delete fragmentShader_c;

	return 0;
}

void Shader::LoadLocationsAttributes() {
	positionAttribute = glGetAttribLocation(program, "a_posL");
	colorAttribute = glGetAttribLocation(program, "a_colL");
	uvAttribute = glGetAttribLocation(program, "a_uv");

	modelMatrix = glGetUniformLocation(program, "u_model");
	viewMatrix = glGetUniformLocation(program, "u_view");
	perspectiveMatrix = glGetUniformLocation(program, "u_persp");


	// TODO: use vector in the future
	textureUniform_0 = glGetUniformLocation(program, "u_texture_0");
	textureUniform_1 = glGetUniformLocation(program, "u_texture_1");
	textureUniform_2 = glGetUniformLocation(program, "u_texture_2");
}

Shader::~Shader()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}