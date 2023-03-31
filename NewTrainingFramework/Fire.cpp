#pragma once
#include "stdafx.h"
#include <time.h>

#include "Fire.h"

using namespace scene_obj;

Fire::Fire(res::Model* model, res::Shader* shader, std::vector<res::Texture*>* textures, float dispMax) :
	SceneObject(model, shader, textures), dispMax(dispMax) {

	Time = (float)clock() / CLOCKS_PER_SEC;

	// Set shader location for the fire attributes
	glUseProgram(shader->program);
	u_dispMax = glGetUniformLocation(shader->program, "u_dispMax");
	u_Time = glGetUniformLocation(shader->program, "u_Time");
}

Fire::Fire() : Fire(nullptr, nullptr, nullptr, 0) {}

void Fire::Update(ESContext* esContext, float deltaTime) {
	Time = (float)clock() / CLOCKS_PER_SEC;
	Time -= (long)Time;
}

void Fire::DrawSpecific() {
	if (u_Time != -1) {
		glUniform1f(u_Time, Time);
	}

	if (u_dispMax != -1) {
		glUniform1f(u_dispMax, dispMax);
	}
}

Fire::~Fire() {}

