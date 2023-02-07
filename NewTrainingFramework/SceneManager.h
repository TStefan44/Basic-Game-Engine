#pragma once

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <fstream>

#include "../Utilities/rapidxml_utils.hpp"
#include "../Utilities/utilities.h"

#include "ResourceManager.h"
#include "Camera.h"
#include "SceneObject.h"
#include "Globals.h"

#undef near
#undef far

namespace manager {
	class SceneManager {
		// Class structures
	public:
		enum class VecType { RGB, XYZ };

		enum class SceneType {
			// General
			gameName,
			defaultScreenSize,
			backgroundColor,
			controls,
			cameras,
			activeCamera,
			objects,
			debugSettings,

			// For objects
			model, shader, type, name, wired,
			position, rotation, scale, color, textures,

			// For camera
			target, up, translationSpeed, rotationSpeed, fov, near, far,

			// For debug
			objectAxes, camAxes,
			OXColor, OYColor, OZColor
		};

		// Class functions
	private:
		static SceneManager* instance;
		SceneManager();

		void initSceneInfo();

		// Parse data from given XML
		void parseDataFromFile();
		void readNameFromFile(rapidxml::xml_node<> const* pRoot);
		void readResolutionFromFile(rapidxml::xml_node<> const* pRoot);
		void readBGFromFile(rapidxml::xml_node<> const* pRoot);
		void readControlsFromFile(rapidxml::xml_node<> const* pRoot);
		void readCamerasFromFile(rapidxml::xml_node<> const* pRoot);
		void readActiveCameraFromFile(rapidxml::xml_node<> const* pRoot);
		void readObjectsFromFile(rapidxml::xml_node<> const* pRoot);
		void readDebugSettingFromFile(rapidxml::xml_node<> const* pRoot);
		Vector3 readVector3(rapidxml::xml_node<> const* pRoot, VecType type);
		std::vector<Vector3> readAxis(rapidxml::xml_node<> const* pRoot);
		std::vector<int> readTexturesId(rapidxml::xml_node<> const* pRoot);

	public:
		~SceneManager();
		void Init(std::string const & file_path, std::string const & file_name);

		// Debug functions
		void DebugPrintAll();

		// Return singleton of class SceneManager
		static SceneManager* getInstance();

		// Core class functions
		void Draw(ESContext* esContext);
		void Update(ESContext* esContext, float deltaTime);

		void freeResources();

		// Class variables
	private:
		// Resource Manager pointer
		ResourceManager* resourceManager;

		// File location and name to read scene resource from
		std::string file_path;
		std::string file_name;

		// SceneManager data
		camera::Camera* mainCamera;
		std::map<int, camera::Camera*> cameras;
		std::map<int, scene_obj::SceneObject*> objects;

		std::map<std::string, SceneType> sceneInfo;
	};
}