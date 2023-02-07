#pragma once
#include "stdafx.h"
#include "ResourceManager.h"

using namespace manager;

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager::ResourceManager() {
	initResourceInfo();
}

ResourceManager* ResourceManager::getInstance() {
	if (instance == nullptr) {
		instance = new ResourceManager();
	}
	return instance;
}

/*
* Set file configuration location and start parsing it's content
*/
void ResourceManager::Init(std::string const& file_path, std::string const& file_name) {
	this->file_path = file_path;
	this->file_name = file_name;

	parseDataFromFile();
}

/*
* A template function used for debug purpose = printing data loaded in memory after
* parsing the XML file
*/
template<class T>
void ResourceManager::printMap(std::map<int, T*> myMap) const {
	if (myMap.empty())
		return;

	for (auto const& entry : myMap) {
		std::cout << "Id = " << entry.first << " " << *entry.second << "\n";
	}
}

void ResourceManager::DebugPrintResources() const {
	printMap<ModelResource>(modelMap);
	printMap<ShaderResource>(shaderMap);
	printMap<TextureResource>(textureMap);
}

res::Model* ResourceManager::LoadModel(int id) {
	res::Model* T = modelsMap[id];
	if (T == nullptr) {
		ModelResource* U = modelMap[id];
		if (U == nullptr) {
			std::cerr << "No model for given id = " << id << " read from resource file\n";
			return nullptr;
		}

		T = new res::Model(U);
		modelsMap[id] = T;
	}

	return T;
}

res::Shader* ResourceManager::LoadShader(int id) {
	res::Shader* T = shadersMap[id];
	if (T == nullptr) {
		ShaderResource* U = shaderMap[id];
		if (U == nullptr) {
			std::cerr << "No shader for given id = " << id << " read from resource file\n";
			return nullptr;
		}

		T = new res::Shader(U);
		shadersMap[id] = T;
	}

	return T;
}

res::Texture* ResourceManager::LoadTexture(int id) {
	res::Texture* T = texturesMap[id];
	if (T == nullptr) {
		TextureResource* U = textureMap[id];
		if (U == nullptr) {
			std::cerr << "No texture for given id = " << id << " read from resource file\n";
			return nullptr;
		}

		T = new res::Texture(U);
		texturesMap[id] = T;
	}

	return T;
}

/*
* A template function used to free the memory from one of the resource manager map
*/
template <class T>
void ResourceManager::freeDataFromMap(std::map<int, T*> myMap) {
	if (myMap.empty())
		return;

	for (auto const& entry : myMap) {
		delete entry.second;
	}
}

/*
* Parse data from the configuration XML file. Call corresponding functions for parsing
* the different data type : model, shader or texture
*/
void ResourceManager::parseDataFromFile() {
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

		if (dataType == "models") {
			readModelFromFile(pNode);
		}
		else if (dataType == "shaders") {
			readShaderFromFile(pNode);
		}
		else if (dataType == "textures") {
			readTextureFromFile(pNode);
		}
	}

	// Free memory
	delete doc;
}

void ResourceManager::readModelFromFile(rapidxml::xml_node<> const* pRoot) {
	for (rapidxml::xml_node<> const* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling())
	{
		std::string dataType = pNode->name();
		std::string folderPath;

		// Check folder case
		if (dataType.compare("folder") != 0)
			continue;

		folderPath = pNode->first_attribute("path", 0, false)->value();

		// Iterate through models
		for (rapidxml::xml_node<> const* modelNode = pNode->first_node(); modelNode; modelNode = modelNode->next_sibling()) {
			std::string dataType = modelNode->name();
			std::string typeModelFile;
			std::string modelName;
			int modelId;

			// Check model case
			if (dataType.compare("model") != 0)
				continue;

			modelId = atoi(modelNode->first_attribute("id", 0, false)->value());

			rapidxml::xml_node<> const* modelChild = modelNode->first_node();
			typeModelFile = modelChild->name();

			// Check file case
			if (typeModelFile.compare("file") != 0)
				continue;

			modelName = modelChild->value();

			// Save read model in memory
			modelMap[modelId] = new ModelResource(folderPath, modelName);
		}
	}
}

void ResourceManager::readShaderFromFile(rapidxml::xml_node<> const* pRoot) {
	for (rapidxml::xml_node<> const* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling())
	{
		std::string dataType = pNode->name();
		std::string folderPath;

		// Check folder case
		if (dataType.compare("folder") != 0)
			continue;

		folderPath = pNode->first_attribute("path", 0, false)->value();

		// Iterate through shaders
		for (rapidxml::xml_node<> const* shaderNode = pNode->first_node(); shaderNode; shaderNode = shaderNode->next_sibling()) {
			std::string dataType = shaderNode->name();
			std::string vertexShaderName;
			std::string fragmentShaderName;
			int shaderId;

			// Check shader case
			if (dataType.compare("shader") != 0)
				continue;

			shaderId = atoi(shaderNode->first_attribute("id", 0, false)->value());

			// Extract shaders name
			for (rapidxml::xml_node<> const* shaderType = shaderNode->first_node(); shaderType; shaderType = shaderType->next_sibling()) {
				std::string dataType = shaderType->name();

				switch (resourceInfo[dataType]) {
				case ResourceInfo::vs: vertexShaderName = shaderType->value(); break;
				case ResourceInfo::fs: fragmentShaderName = shaderType->value(); break;
				default:break;
				}
			}

			// Save read shader in memory
			shaderMap[shaderId] = new ShaderResource(folderPath, vertexShaderName, fragmentShaderName);
		}
	}
}

void ResourceManager::readTextureFromFile(rapidxml::xml_node<> const* pRoot) {
	for (rapidxml::xml_node<> const* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling())
	{
		std::string dataType = pNode->name();
		std::string folderPath;

		// Check folder case
		if (dataType.compare("folder") != 0)
			continue;

		folderPath = pNode->first_attribute("path", 0, false)->value();

		// Iterate through textures
		for (rapidxml::xml_node<> const* textureNode = pNode->first_node(); textureNode; textureNode = textureNode->next_sibling()) {
			std::string dataType = textureNode->name();
			std::string textureType;
			std::string textureName;
			std::string min_filter;
			std::string mag_filter;
			std::string wrap_s;
			std::string wrap_t;
			int textureId;

			if (dataType.compare("texture") != 0)
				continue;

			textureId = atoi(textureNode->first_attribute("id", 0, false)->value());
			textureType = textureNode->first_attribute("type", 0, false)->value();

			// Extract texture attributes
			for (rapidxml::xml_node<> const* textureAttr = textureNode->first_node(); textureAttr; textureAttr = textureAttr->next_sibling()) {
				std::string dataType = textureAttr->name();

				switch (resourceInfo[dataType]) {
				case ResourceInfo::file:
					textureName = textureAttr->value(); break;
				case ResourceInfo::min_filter:
					min_filter = textureAttr->value(); break;
				case ResourceInfo::mag_filter:
					mag_filter = textureAttr->value(); break;
				case ResourceInfo::wrap_s:
					wrap_s = textureAttr->value(); break;
				case ResourceInfo::wrap_t:
					wrap_t = textureAttr->value(); break;
				default:break;
				}
			}

			// saved read texture in memory
			textureMap[textureId] = new TextureResource(folderPath, textureName, textureType, min_filter, mag_filter, wrap_s, wrap_t);
		}
	}
}

void ResourceManager::initResourceInfo() {
	resourceInfo["file"] = ResourceInfo::file;

	resourceInfo["min_filter"] = ResourceInfo::min_filter;
	resourceInfo["mag_filter"] = ResourceInfo::mag_filter;
	resourceInfo["wrap_s"] = ResourceInfo::wrap_s;
	resourceInfo["wrap_t"] = ResourceInfo::wrap_t;

	resourceInfo["vs"] = ResourceInfo::vs;
	resourceInfo["fs"] = ResourceInfo::fs;
}

void ResourceManager::freeResources() {
	freeDataFromMap<ModelResource>(modelMap);
	freeDataFromMap<ShaderResource>(shaderMap);
	freeDataFromMap<TextureResource>(textureMap);
}

ResourceManager::~ResourceManager() {
	freeResources();
}

/*
* Setter and Getter functions
*/
void ResourceManager::setFile(std::string const& file_path, std::string const& file_name) {
	this->file_name = file_name;
	this->file_path = file_path;
}

std::string ResourceManager::getFilePath() const {
	return file_path;
}

std::string ResourceManager::getFileName() const {
	return file_name;
}