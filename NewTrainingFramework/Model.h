#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <functional>

#include "Resource.h"
#include "Vertex.h"

#include "../Utilities/utilities.h"

using Action = std::function<void(std::stringstream&)>;

namespace res {
	class Model {
	public:
		// Constructors
		Model();
		Model(ModelResource const* mr);
		~Model();

		void Load();

	private:
		int parseData();
		std::stringstream assemblyStream(std::string line) const;
		void parseSegmentData(std::ifstream& fin, std::string& line, Action action);
		void verticesAction(std::stringstream& s_line);
		void indicesAction(std::stringstream& s_line);

		friend std::ostream& operator<<(std::ostream& os, const Model& model) {
			os << *model.mr;
			return os;
		}

		// Class variables
	private:
		// Model specific variables
		ModelResource const* mr;
		GLuint ibo;
		GLuint wire_ibo;
		GLuint vbo;
		GLuint nr_index;
		GLuint nr_index_wired;

		// Model data
		std::vector<Vertex> verticesData;
		std::vector<GLuint> indicesData;
	};
}