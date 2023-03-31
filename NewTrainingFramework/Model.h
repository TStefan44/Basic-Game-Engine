#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <functional>

#include "Resource.h"
#include "Vertex.h"

#include "../Utilities/utilities.h"

namespace res {
	class Model {
		using Action = std::function<void(std::stringstream&)>;

	public:
		// Constructors
		Model();
		Model(ModelResource const* mr);
		Model(std::vector<Vertex> const& verticesData, std::vector<GLuint> const& indicesData);
		~Model();

		void Load();

	private:
		int parseData();
		std::stringstream assemblyStream(std::string line) const;
		void parseSegmentData(std::ifstream& fin, std::string& line, Action& action);
		void verticesAction(std::stringstream& s_line);
		void indicesAction(std::stringstream& s_line);

		friend std::ostream& operator<<(std::ostream& os, const Model& model) {
			os << *model.mr;
			return os;
		}

		// Class variables
	public:
		GLuint ibo;
		GLuint vbo;
		GLuint wire_ibo;
		GLuint nr_index;
		GLuint nr_index_wired;

	private:
		// Model specific variables
		ModelResource const* mr;

		// Model data
		std::vector<Vertex> verticesData;
		std::vector<GLuint> indicesData;
	};
}