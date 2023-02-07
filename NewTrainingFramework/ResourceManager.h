#pragma once

#include <string>
#include <map>
#include <sstream>
#include <fstream>

#include "Resource.h"
#include "Model.h"
#include "Texture.h"
#include "Shaders.h"

#include "../Utilities/rapidxml_utils.hpp"
#include "../Utilities/utilities.h"

namespace manager {
	class ResourceManager {
		// Class structures
	public:
		enum class ResorceType { MODEL, SHADER, TEXTURE };
		enum class ResourceInfo {
			file,										// general
			min_filter, mag_filter, wrap_s, wrap_t,		// for texture
			vs, fs										// for shader
		};

		// Class functions
	private:
		ResourceManager();
		void initResourceInfo();

		template <class T>
		void freeDataFromMap(std::map<int, T*> myMap);

		template <class T>
		void printMap(std::map<int, T*> myMap) const;

		// Parse data from given XML
		void parseDataFromFile();
		void readModelFromFile(rapidxml::xml_node<> const* pRoot);
		void readShaderFromFile(rapidxml::xml_node<> const* pRoot);
		void readTextureFromFile(rapidxml::xml_node<> const* pRoot);

	public:
		~ResourceManager();
		void Init(std::string const &file_path, std::string const &file_name);
		void freeResources();
		void DebugPrintResources() const ;

		// Return singleton of class ResourceManager
		static ResourceManager* getInstance();

		// Load Resource
		res::Model* LoadModel(int id);
		res::Shader* LoadShader(int id);
		res::Texture* LoadTexture(int id);

		// Getter functions
		std::string getFilePath() const;
		std::string getFileName() const;

		// Setter functions
		void setFile(std::string const &file_path, std::string const &file_name);

		// Class variables
	private:
		// Singleton class
		static ResourceManager* instance;

		// File location and name to read data resource from
		std::string file_path;
		std::string file_name;

		// ResourceManager data
		std::map<int, ModelResource*> modelMap;
		std::map<int, ShaderResource*> shaderMap;
		std::map<int, TextureResource*> textureMap;
		std::map<std::string, ResourceInfo> resourceInfo;

		std::map<int, res::Model*> modelsMap;
		std::map<int, res::Texture*> texturesMap;
		std::map<int, res::Shader*> shadersMap;
	};
}