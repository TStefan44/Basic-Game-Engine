#pragma once
#include "stdafx.h"
#include "SceneManager.h"

using namespace manager;
using namespace camera;

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager() {
	resourceManager = ResourceManager::getInstance();

	initSceneInfo();
}

SceneManager* SceneManager::getInstance() {
	if (instance == nullptr) {
		instance = new SceneManager();
	}
	return instance;
}

void SceneManager::initSceneInfo() {
	// General
	sceneInfo["gameName"] = SceneType::gameName;
	sceneInfo["defaultScreenSize"] = SceneType::defaultScreenSize;
	sceneInfo["backgroundColor"] = SceneType::backgroundColor;
	sceneInfo["controls"] = SceneType::controls;
	sceneInfo["cameras"] = SceneType::cameras;
	sceneInfo["activeCamera"] = SceneType::activeCamera;
	sceneInfo["objects"] = SceneType::objects;
	sceneInfo["debugSettings"] = SceneType::debugSettings;

	// Objects
	sceneInfo["position"] = SceneType::position;
	sceneInfo["rotation"] = SceneType::rotation;
	sceneInfo["scale"] = SceneType::scale;
	sceneInfo["color"] = SceneType::color;
	sceneInfo["textures"] = SceneType::textures;
	sceneInfo["model"] = SceneType::model;
	sceneInfo["shader"] = SceneType::shader;
	sceneInfo["wired"] = SceneType::wired;
	sceneInfo["name"] = SceneType::name;

	// Camera
	sceneInfo["type"] = SceneType::type;
	sceneInfo["target"] = SceneType::target;
	sceneInfo["up"] = SceneType::up;
	sceneInfo["translationSpeed"] = SceneType::translationSpeed;
	sceneInfo["rotationSpeed"] = SceneType::rotationSpeed;
	sceneInfo["fov"] = SceneType::fov;
	sceneInfo["near"] = SceneType::near;
	sceneInfo["far"] = SceneType::far;

	// Debug Settings
	sceneInfo["objectAxes"] = SceneType::objectAxes;
	sceneInfo["camAxes"] = SceneType::camAxes;
	sceneInfo["OXColor"] = SceneType::OXColor;
	sceneInfo["OYColor"] = SceneType::OYColor;
	sceneInfo["OZColor"] = SceneType::OZColor;
}

/*
* Set file configuration location and start parsing it's content
*/
void SceneManager::Init(std::string const & file_path, std::string const & file_name) {
	this->file_path = file_path;
	this->file_name = file_name;

	parseDataFromFile();
}

/*
* Parse data from the configuration XML file. Call corresponding functions for parsing
* the different data type.
*/
void SceneManager::parseDataFromFile() {
	// Prepare file for opening
	std::string fullPath = file_path + file_name;
	auto* doc = new rapidxml::xml_document<>();
	std::ifstream file(fullPath.c_str());
	if (!file.is_open()) {
		std::cerr << "Resource Manager: Error opening " << fullPath << "\n";
		delete doc;
		return;
	}

	// Extract data and close file
	std::stringstream buffer;
	buffer << file.rdbuf(); file.close();
	std::string content(buffer.str());

	// Parse data in a xml_document and iterate through level 0 nodes
	doc->parse<0>(&content[0]);
	rapidxml::xml_node<> const* pRoot = doc->first_node();
	for (rapidxml::xml_node<> const* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling())
	{
		std::string dataType = pNode->name();

		switch (sceneInfo[dataType]) {
		case SceneType::gameName:
			readNameFromFile(pNode); break;
		case SceneType::defaultScreenSize:
			readResolutionFromFile(pNode); break;
		case SceneType::backgroundColor:
			readBGFromFile(pNode); break;
		case SceneType::controls:
			readControlsFromFile(pNode); break;
		case SceneType::cameras:
			readCamerasFromFile(pNode); break;
		case SceneType::activeCamera:
			readActiveCameraFromFile(pNode); break;
		case SceneType::objects:
			readObjectsFromFile(pNode); break;
		case SceneType::debugSettings:
			readDebugSettingFromFile(pNode); break;
		default: break;
		}
	}

	// Free memory
	delete doc;
}

void SceneManager::readNameFromFile(rapidxml::xml_node<> const* pRoot) {
	std::string gameName = pRoot->value();

	// Set game name
	Globals::setWindowName(gameName);
}

void SceneManager::readResolutionFromFile(rapidxml::xml_node<> const* pRoot) {
	bool fullscreen = false;
	int width = 0;
	int height = 0;

	for (rapidxml::xml_node<> const* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling()) {
		std::string dataType = pNode->name();

		if (dataType == "fullscreen") {
			fullscreen = true;
			break;
		}
		else if (dataType == "width") {
			width = atoi(pNode->value());
		}
		else if (dataType == "height") {
			height = atoi(pNode->value());
		}
	}

	// Set game resolution
	Globals::setScreenResolution(width, height);
}

void SceneManager::readBGFromFile(rapidxml::xml_node<> const* pRoot) {
	float r, g, b;
	r = g = b = 0.f;

	for (rapidxml::xml_node<> const* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling()) {
		std::string dataType = pNode->name();
		if (dataType == "r") {
			r = atof(pNode->value());
		}
		else if (dataType == "g") {
			g = atof(pNode->value());
		}
		else if (dataType == "b") {
			b = atof(pNode->value());
		}
	}

	// Set BG Color;
	Globals::setColorBG(Vector3(r, 1, b));
}

void SceneManager::readControlsFromFile(rapidxml::xml_node<> const* pRoot) {
	for (rapidxml::xml_node<> const* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling()) {
		std::string dataType = pNode->name();
		std::string key;
		std::string action;

		if (dataType != "control") {
			continue;
		}

		for (rapidxml::xml_node<> const* controlNode = pNode->first_node(); controlNode; controlNode = controlNode->next_sibling()) {
			std::string dataType = controlNode->name();

			if (dataType == "key") {
				key = controlNode->value();
			}
			else if (dataType == "action") {
				action = controlNode->value();
			}
		}

		// TODO: create action map
		// TODO: delete later
		std::cout << pRoot->name() << " : Control  Key = " + key + " Action = " + action + "\n";
	}
}

void SceneManager::readCamerasFromFile(rapidxml::xml_node<> const* pRoot) {
	for (rapidxml::xml_node<> const* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling()) {
		std::string dataType = pNode->name();
		std::string type;
		Vector3 position;
		Vector3 target;
		Vector3 up;
		Camera::CameraView camView;
		int cameraId;
		float translationSpeed;
		float rotationSpeed;
		float fov;
		float near;
		float far;

		translationSpeed = rotationSpeed = 0.f;
		fov = near = far = 0.f;

		if (dataType != "camera") {
			continue;
		}

		cameraId = atoi(pNode->first_attribute("id", 0, false)->value());

		for (rapidxml::xml_node<> const* cameraNode = pNode->first_node(); cameraNode; cameraNode = cameraNode->next_sibling()) {
			std::string dataType = cameraNode->name();

			switch (sceneInfo[dataType]) {
			case SceneType::type:
				type = cameraNode->value(); break;
			case SceneType::position:
				position = readVector3(cameraNode, VecType::XYZ); break;
			case SceneType::target:
				target = readVector3(cameraNode, VecType::XYZ); break;
			case SceneType::up:
				up = readVector3(cameraNode, VecType::XYZ); break;
			case SceneType::translationSpeed:
				translationSpeed = atof(cameraNode->value()); break;
			case SceneType::rotationSpeed:
				rotationSpeed = atof(cameraNode->value()); break;
			case SceneType::fov:
				fov = atof(cameraNode->value()); break;
			case SceneType::near:
				near = atof(cameraNode->value()); break;
			case SceneType::far:
				far = atof(cameraNode->value()); break;
			default: break;
			}
		}

		if (type == "firstPerson") {
			camView = Camera::CameraView::FIRST_PERSON;
		}
		else {
			camView = Camera::CameraView::THIRD_PERSON;
		}

		// Create camera and add it in vector
		auto* new_camera = new Camera(camView, target, position,
				up, near, far, fov, translationSpeed, rotationSpeed);
		cameras[cameraId] = new_camera;
	}
}

void SceneManager::readActiveCameraFromFile(rapidxml::xml_node<> const* pRoot) {
	int cameraId = atoi(pRoot->value());

	// Set active camera
	mainCamera = cameras[cameraId];
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T> & vec) {
	os << "[";
	for (const T & i : vec) {
		os << i << ",";
	}
	os << "]";
	return os;
}

void SceneManager::readObjectsFromFile(rapidxml::xml_node<> const* pRoot) {
	for (rapidxml::xml_node<> const* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling()) {
		std::string dataType = pNode->name();
		std::string type;
		std::string name;
		scene_obj::ObjectType obj_type;
		int objectId;
		int modelId;
		int shaderId;
		bool wired;
		std::vector<int> texturesId;
		Vector3 color;
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;

		objectId = modelId = shaderId = -1;
		wired = false;

		if (dataType != "object") {
			continue;
		}

		objectId = atoi(pNode->first_attribute("id", 0, false)->value());

		for (rapidxml::xml_node<> const* objectNode = pNode->first_node(); objectNode; objectNode = objectNode->next_sibling()) {
			std::string dataType = objectNode->name();

			switch (sceneInfo[dataType]) {
			case SceneType::model:
				modelId = atoi(objectNode->value()); break;
			case SceneType::shader:
				shaderId = atoi(objectNode->value()); break;
			case SceneType::type:
				type = objectNode->value(); break;
			case SceneType::name:
				name = objectNode->value(); break;
			case SceneType::wired:
				wired = true;
			case SceneType::position:
				position = readVector3(objectNode, VecType::XYZ); break;
			case SceneType::rotation:
				rotation = readVector3(objectNode, VecType::XYZ); break;
			case SceneType::scale:
				scale = readVector3(objectNode, VecType::XYZ); break;
			case SceneType::color:
				color = readVector3(objectNode, VecType::RGB); break;
			case SceneType::textures:
				texturesId = readTexturesId(objectNode); break;
			default: break;
			}
		}

		if (type == "normal") {
			obj_type = scene_obj::ObjectType::Normal;
		}
		else {
			obj_type = scene_obj::ObjectType::Terrain;
		}

		// Create object with read data and load it in memory
		res::Model * model = resourceManager->LoadModel(modelId);
		res::Shader * shader = resourceManager->LoadShader(shaderId);
		auto* textures = new std::vector<res::Texture*>;
		for (auto textureId : texturesId) {
			res::Texture * new_texture = resourceManager->LoadTexture(textureId);
			textures->push_back(new_texture);
		}

		auto * new_object = new scene_obj::SceneObject(model, shader, textures);
		new_object->Init(objectId, name, obj_type, position, rotation, scale, TRUE);
		objects[objectId] = new_object;

		// TODO: model can be string for type != normal
		
	}
}

void SceneManager::readDebugSettingFromFile(rapidxml::xml_node<> const* pRoot) {
	std::vector<Vector3> objectAxes;
	std::vector<Vector3> camAxes;

	for (rapidxml::xml_node<> const* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling()) {
		std::string dataType = pNode->name();

		switch (sceneInfo[dataType]) {
		case SceneType::camAxes:
			camAxes = readAxis(pNode);
			break;
		case SceneType::objectAxes:
			objectAxes = readAxis(pNode);
			break;
		default: break;
		}
	}

	//TODO: use those debug settings
	//TODO: Delete later
	std::cout << "Debug Settings : ObjectAxes = " << objectAxes << " CamAxes =  " << camAxes << "\n";
}

std::vector<Vector3> SceneManager::readAxis(rapidxml::xml_node<> const* pRoot) {
	std::vector<Vector3> axis;

	for (rapidxml::xml_node<> const* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling()) {
		std::string dataType = pNode->name();
		Vector3 axisColor;

		switch (sceneInfo[dataType]) {
		case SceneType::OXColor:
			axisColor = readVector3(pNode, VecType::RGB);
			break;
		case SceneType::OYColor:
			axisColor = readVector3(pNode, VecType::RGB);
			break;
		case SceneType::OZColor:
			axisColor = readVector3(pNode, VecType::RGB);
			break;
		default: break;
		}

		axis.push_back(axisColor);
	}

	return axis;
}

std::vector<int> SceneManager::readTexturesId(rapidxml::xml_node<> const* pRoot) {
	std::vector<int> texturesIds;

	for (rapidxml::xml_node<> const* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling()) {
		std::string dataType = pNode->name();
		int textureId;

		if (dataType != "texture") {
			continue;
		}

		textureId = atoi(pNode->first_attribute("id", 0, false)->value());

		texturesIds.push_back(textureId);
	}

	return texturesIds;
}

Vector3 SceneManager::readVector3(rapidxml::xml_node<> const* pRoot, VecType type) {
	std::string c1, c2, c3;
	float a, b, c;

	a = b = c = 0.f;

	switch (type) {
	case VecType::RGB:
		c1 = "r"; c2 = "g", c3 = "b"; break;
	case VecType::XYZ:
		c1 = "x", c2 = "y", c3 = "z"; break;
	}

	for (rapidxml::xml_node<> const* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling()) {
		std::string dataType = pNode->name();

		if (dataType == c1) {
			a = atof(pNode->value());
		}
		else if (dataType == c2) {
			b = atof(pNode->value());
		}
		else if (dataType == c3) {
			c = atof(pNode->value());
		}
	}

	return Vector3(a, b, c);
}

void SceneManager::Draw(ESContext* esContext) {
}

void SceneManager::Update(ESContext* esContext, float deltaTime) {
}

void SceneManager::freeResources() {
}

SceneManager::~SceneManager() {}

void SceneManager::DebugPrintAll() {
	// Print cameras
	std::cout << "\n\nScene Manager debug info\nCameras : \n";
	for (auto &camera : cameras) {
		std::cout << "Id = " << camera.first << " Info -->  " << *camera.second << "\n";
	}

	std::cout << "\nMain camera --> " << *mainCamera << "\n\n";

	for (auto& object : objects) {
		std::cout << *object.second << "\n";
	}
	std::cout << "\n";
}