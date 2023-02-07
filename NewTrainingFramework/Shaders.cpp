#pragma once
#include "stdafx.h"
#include "Shaders.h"

using namespace res;

Shader::Shader(ShaderResource const* sr) : sr(sr) {

	Load();
}

Shader::Shader() : Shader(nullptr) {}

int Shader::Load()
{
	std::string fileVertexShader = mainFolder + sr->folder_path + sr->fileFragment_name;
	std::string fileFragmentShader = mainFolder + sr->folder_path + sr->fileFragment_name;

	vertexShader = esLoadShader(GL_VERTEX_SHADER, &fileVertexShader[0]);

	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, &fileFragmentShader[0]);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	return 0;
}

Shader::~Shader()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}