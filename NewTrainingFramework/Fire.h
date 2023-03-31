#pragma once

#include "SceneObject.h"

#include "../Utilities/utilities.h"

namespace scene_obj {
	class  Fire : public SceneObject
	{
	public:
		Fire(res::Model* model, res::Shader* shader, std::vector<res::Texture*>* textures, float dispMax);
		Fire();
		~Fire();

		void Update(ESContext* esContext, float deltaTime) override;

	protected:
		void DrawSpecific() override;

	private:
		float dispMax;
		float Time;

		GLint u_dispMax;
		GLint u_Time;

	};
}