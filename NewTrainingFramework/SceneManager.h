#pragma once

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <fstream>

#include "../Utilities/rapidxml_utils.hpp"
#include "../Utilities/utilities.h"

#include "SceneObject.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Fire.h"
#include "LightSource.h"
#include "ResourceManager.h"
#include "InputController.h"
#include "Camera.h"
#include "Globals.h"

#undef near
#undef far

namespace manager {
	class SceneManager {
		// Class structures
	public:
		enum class VecType { RGB, XYZ, DSS};

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
			dimension, heights,
			followingCamera, material,

			// For camera
			target, up, translationSpeed, rotationSpeed, fov, near, far,

			// For fog
			fog, clearZone, transitionZone, fogColor,

			// For fire
			dispMax,

			// For light
			lights, ambientalLight, ratio,
			specularColor, diffuseColor,
			associatedObject,

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
		void createInput(std::string key, std::string action);
		Vector3 readVector3(rapidxml::xml_node<> const* pRoot, VecType type);
		std::vector<Vector3> readAxis(rapidxml::xml_node<> const* pRoot);
		std::vector<int> readTexturesId(rapidxml::xml_node<> const* pRoot);
		void readTerrainSize(rapidxml::xml_node<> const* pRoot, int& nr_cells, int& length_cell);
		void readFollowCamera(rapidxml::xml_node<> const* pRoot, char& axes, int& idCamera);
		void readFogFromFile(rapidxml::xml_node<> const* pRoot);
		void readAmbientalLightFromFile(rapidxml::xml_node<> const* pRoot);
		void readLightsFromFile(rapidxml::xml_node<> const* pRoot);
		std::vector<int> readObjLightsFromFile(rapidxml::xml_node<> const* pRoot);
		void setLightsToObjects();

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
	public:
		camera::Camera* mainCamera;

		// Fog
		float fogClear_r;
		float fogTrans_R;
		Vector3 fogColor;

		// Ambiental Light
		light::LightSource *ambientalLight;
		float al_ratio;

	private:
		// Resource Manager pointer
		ResourceManager* resourceManager;
		InputController* inputController;

		// File location and name to read scene resource from
		std::string file_path;
		std::string file_name;

		// SceneManager data
		std::map<int, camera::Camera*> cameras;
		std::map<int, scene_obj::SceneObject*> objects;
		std::map<int, light::LightSource*> scene_lights;

		std::map<std::string, SceneType> sceneInfo;
	};
}