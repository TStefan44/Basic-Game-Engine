#pragma once
#include "stdafx.h"
#include "Model.h"

using namespace res;

Model::Model(ModelResource const* mr) : mr(mr) {
	ibo = wire_ibo = vbo = 0;
	nr_index = nr_index_wired = 0;

	if (parseData() != 0) {
		std::cerr << "Can't load in memory model " << mr->file_name << "\n";
		return;
	}

	Load();
}

Model::Model(std::vector<Vertex> const& verticesData, std::vector<GLuint> const& indicesData) :
	verticesData(verticesData), indicesData(indicesData)
{
	ibo = wire_ibo = vbo = 0;
	nr_index = nr_index_wired = 0;

	Load();
}

Model::Model() : Model(nullptr) {}

void Model::Load() {
	nr_index = indicesData.size();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData[0]) * verticesData.size(), &verticesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData[0]) * nr_index, &indicesData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// TODO: create wired index and ibo
}

/*
* Parse data from given file with the .nfg format. Extract vertices
* and indices
*/
int Model::parseData() {
	// Extract file path
	if (mr == nullptr) {
		std::cerr << "NULL pointer to model resource!\n";
		return 1;
	}
	std::string filePath = mainFolder + mr->folder_path + mr->file_name;

	// Open file
	std::ifstream fin(filePath, std::ios::in);
	if (!fin) {
		std::cerr << "Cannot open " << filePath << "\n";
		fin.close();
		return 2;
	}

	std::string line;
	Action vertices = std::bind(&Model::verticesAction, this, std::placeholders::_1);
	Action indices = std::bind(&Model::indicesAction, this, std::placeholders::_1);

	parseSegmentData(fin, line, vertices);
	parseSegmentData(fin, line, indices);

	fin.close();
	return 0;
}

/*
* Used to parse segments of data like vertices data or indices configuration. After
* the info is extracted in an stringstream, we use an action to load data in designated
* variables
*/
void Model::parseSegmentData(std::ifstream& fin, std::string& line, Action& action) {
	int num;
	std::string aux;
	std::stringstream s_line;

	std::getline(fin, line);
	s_line.str(line);
	s_line >> aux >> num;

	if (aux == "NrIndices:")
		num /= 3;

	for (int i = 0; i < num; i++) {
		std::getline(fin, line);
		s_line = assemblyStream(line);

		action(s_line);
	}
}

void Model::verticesAction(std::stringstream& s_line) {
	Vector3 pos;
	Vector3 normal;
	Vector3 binormal;
	Vector3 tgt;
	Vector2 uv;

	s_line >> pos >> normal >> binormal >> tgt >> uv;
	verticesData.emplace_back(pos, binormal, binormal, normal, tgt, uv);
}

void Model::indicesAction(std::stringstream& s_line) {
	int x;
	int y;
	int z;

	s_line >> x >> y >> z;

	indicesData.push_back(x);
	indicesData.push_back(y);
	indicesData.push_back(z);
}

/*
* Function gets a string and construct a stringstream which contains just the
* numbers from the input string, separated by ' ', in float format, using
* regex.
* The input must has the format "number. data". The function
* will skip the first number because we consider it an iterator / index.
*/
std::stringstream Model::assemblyStream(std::string line) const {
	std::stringstream s_line;

	std::regex regexp("[+-]?([0-9]*[.])?[0-9]+");
	std::smatch m;

	// skip index
	std::regex_search(line, m, regexp);
	line = m.suffix();

	while (std::regex_search(line, m, regexp)) {
		s_line << m[0].str() << ' ';
		line = m.suffix().str();
	}

	return s_line;
}

Model::~Model() {}