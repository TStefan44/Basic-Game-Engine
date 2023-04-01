#pragma once

#include <vector>
#include <string>
#include <map>

#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "Camera.h"
#include "LightSource.h"

#include "../Utilities/utilities.h"

namespace scene_obj {
	enum class ObjectType { Normal, Terrain, SkyBox, Fire };

	enum class DrawType { Normal, Wired };

	class SceneObject {
	public:
		// Class constructor
		SceneObject(res::Model* model, res::Shader* shader, std::vector<res::Texture*>* textures);
		SceneObject();

		virtual ~SceneObject();

		// Core functions
		void Init(int id, std::string name, ObjectType type, Vector3 position, Vector3 rotation, Vector3 scale, bool DEPTH_TEST);
		void Draw(ESContext* esContext);
		virtual void Update(ESContext* esContext, float deltaTime);

		// Function used when we have the follow property active
		void SetFollowCamera(char followAxes, camera::Camera* followCamera);
		void FollowCamera();

		// Function used for Light manipulation
		void SetMaterialProp(Vector3 material);
		void SetLightsIds(std::vector<int> ids);
		std::vector<int>* GetObjLights();
		void SetLights(std::map<int, light::LightSource*>* obj_lights);

		// Debug function
		friend std::ostream& operator<<(std::ostream& os, const SceneObject& object) {
			os << "Object : Name " + object.name;
			if (object.model != nullptr) {
				if (object.type == ObjectType::Terrain)
					os << "\n\tModel is Generated";
				else
					os << "\n\t" << *object.model;
			}
			if (object.shader != nullptr) os << "\n\t" << *object.shader;
			if (object.textures != nullptr) {
				os << "\n\tTextures :";
				for (auto& texture : *object.textures) {
					os << "\n\t\t" << *texture;
				}
			}
			return os;
		}

	protected:
		// Functions used by child classes
		virtual void DrawSpecific();

	private:
		// Rendering functions
		void DrawCommon();
		void DrawLights();
		void sendTexture(int TEXTURE, int id);

	protected:
		// Class variables
		int id;
		std::string name;
		ObjectType type;

		// Light properties
		int light_number;
		std::vector<int> lights_ids;
		std::map<int, light::LightSource*> *obj_lights;
		float material_kd; // Difuze component
		float material_ks; // Specular component
		float material_shininess; // Specular component

		// Scene properties
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;

		// Core rendering properties
		res::Model* model;
		res::Shader* shader;
		std::vector<res::Texture*>* textures;

		bool DEPTH_TEST;

		// Pointers to special cameras in the scene
		camera::Camera* mainCamera;
		camera::Camera* followCamera;

		// Follow state properties
		char followAxes;
		Vector3 followOffset;
	};
}