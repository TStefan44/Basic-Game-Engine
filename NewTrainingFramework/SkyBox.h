#pragma once

#include "SceneObject.h"

#include "../Utilities/utilities.h"

namespace scene_obj {
	class SkyBox : public SceneObject {
	public:
		SkyBox(res::Model* model, res::Shader* shader, std::vector<res::Texture*>* textures);
		SkyBox();
		~SkyBox();
	};
}