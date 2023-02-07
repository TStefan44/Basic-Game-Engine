#pragma once
#include "../Utilities/utilities.h"

#include <vector>
#include <string>

#include "Model.h"
#include "Texture.h"
#include "Shaders.h"

namespace scene_obj {
	enum class ObjectType { Normal, Terrain };

	enum class DrawType { Normal, Wired };

	class SceneObject {
	public:
		SceneObject(res::Model *model, res::Shader* shader, std::vector<res::Texture*> * textures);
		SceneObject();
		~SceneObject();

		void Init(int id, std::string name, ObjectType type, Vector3 position, Vector3 rotation, Vector3 scale, bool DEPTH_TEST);
		void Draw(ESContext* esContext);
		void Update(ESContext* esContext, float deltaTime);
		
		friend std::ostream& operator<<(std::ostream& os, const SceneObject& object) {
			os << "Object : Name " + object.name;
			if (object.model != nullptr) os << "\n\t" << *object.model;
			if (object.shader != nullptr) os << "\n\t" << *object.shader;
			if (object.textures != nullptr) {
				os << "\n\tTextures :";
				for (auto& texture : *object.textures) {
					os << "\n\t\t" << *texture;
				}
			}
			return os;
		}

	private:
		int id;
		std::string name;
		ObjectType type;

		Vector3 position;
		Vector3 rotation;
		Vector3 scale;

		res::Model* model;
		res::Shader* shader;
		std::vector<res::Texture*> * textures;

		bool DEPTH_TEST;
	};
}
