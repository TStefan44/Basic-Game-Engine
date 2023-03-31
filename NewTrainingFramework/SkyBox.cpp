#pragma once
#include "stdafx.h"

#include "SkyBox.h"

using namespace scene_obj;

SkyBox::SkyBox(res::Model* model, res::Shader* shader, std::vector<res::Texture*>* textures) :
	SceneObject(model, shader, textures) {

}

SkyBox::SkyBox() : SkyBox(nullptr, nullptr, nullptr) {}

SkyBox::~SkyBox() {}