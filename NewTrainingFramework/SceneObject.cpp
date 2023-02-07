#pragma once
#include "stdafx.h"
#include "SceneObject.h"

using namespace scene_obj;

SceneObject::SceneObject(res::Model* model, res::Shader* shader, std::vector<res::Texture*> * textures) :
	model(model), shader(shader), textures(textures)
{

}

SceneObject::SceneObject() : SceneObject(nullptr, nullptr, nullptr) {}

void SceneObject::Init(int id, std::string name, ObjectType type, Vector3 position, Vector3 rotation, Vector3 scale, bool DEPTH_TEST) {
	this->id = id;
	this->name = name;
	this->type = type;
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	this->DEPTH_TEST = DEPTH_TEST;
}

void SceneObject::Draw(ESContext* esContext) {

}

void SceneObject::Update(ESContext* esContext, float deltaTime) {

}


SceneObject::~SceneObject() {

}