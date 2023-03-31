#pragma once
#include "stdafx.h"

#include "Terrain.h"

using namespace scene_obj;

Terrain::Terrain(res::Shader* shader, std::vector<res::Texture*>* textures) :
	SceneObject(nullptr, shader, textures) {
	nr_cells = dim_cell = 0;
	offset_uv2 = Vector2(0, 0);

	// Set shader location for the terrain attributes
	glUseProgram(shader->program);
	u_nr_cells = glGetUniformLocation(shader->program, "u_nr_cells");
	u_offset_uv2 = glGetUniformLocation(shader->program, "u_offset_uv2");
	u_height = glGetUniformLocation(shader->program, "u_height");
}

Terrain::Terrain() : Terrain(nullptr, nullptr) {}

/*
* Make the terrain to seem infinite by translating it to the main
* camera position, and by moving the uv.
*/
void Terrain::Update(ESContext* esContext, float deltaTime) {
	Vector3 pos = mainCamera->getPosition();
	
	// change pos and uv if distance is greater then
	// the length of a cell.
	float dx = position.x - pos.x;
	float dz = position.z - pos.z;

	if (abs(dx) > dim_cell) {
		// Ox
		if (dx < 0) {
			position.x += dim_cell;
			offset_uv2.y += offset;
		}
		else {
			position.x -= dim_cell;
			offset_uv2.y -= offset;
		}
	}
	if (abs(dz) > dim_cell) {
		// Oy
		if (dz < 0) {
			position.z += dim_cell;
			offset_uv2.x += offset;
		}
		else {
			position.z -= dim_cell;
			offset_uv2.x -= offset;
		}
	}
}

/*
* Set need attributes for the terrain and create a model mesh using those
* attributes.
*/
void Terrain::SetDimensionsTerrain(int nr_cells, int dim_cell, Vector3 heights) {
	this->nr_cells = nr_cells;
	this->dim_cell = dim_cell;
	this->heigths = heights;

	offset = 1.f / nr_cells;

	model = GenerateModelTerrain();
}

/*
* Return a model mesh for the terrain. Model is a square made with
* nr cell x nr cell quads, with the center in (0, 0).
*/
res::Model* Terrain::GenerateModelTerrain() {
	if (nr_cells == 0 || dim_cell == 0) {
		return nullptr;
	}

	auto verticesData = std::vector<Vertex>((nr_cells + 1) * (nr_cells + 1));
	auto indicesData = std::vector<GLuint>(6 * nr_cells * nr_cells);
	
	// Vertices Data
	for (int i = 0; i < nr_cells + 1; i++) {
		for (int j = 0; j < nr_cells + 1; j++) {
			Vector3 pos = Vector3(((float)j - (float)nr_cells / 2) * dim_cell, 0, ((float)i - (float)nr_cells / 2) * dim_cell);
			Vector3 color = Vector3(i % 2, j % 2, 1);
			Vector2 uv = Vector2(i, j);
			verticesData[i * (nr_cells + 1) + j] = Vertex(pos, color, Vector3(), Vector3(), Vector3(), uv);
		}
	}

	// Indices data
	int k = 0;
	for (int i = 0; i < nr_cells; i++) {
		for (int j = 0; j < nr_cells; j++) {
			// Left half quad
			indicesData[k++] = j + i * (nr_cells + 1);
			indicesData[k++] = j + i * (nr_cells + 1) + 1;
			indicesData[k++] = j + (i + 1) * (nr_cells + 1);

			// Right half quad
			indicesData[k++] = j + i * (nr_cells + 1) + 1;
			indicesData[k++] = j + (i + 1) * (nr_cells + 1) + 1;
			indicesData[k++] = j + (i + 1) * (nr_cells + 1);
		}
	}

	return new res::Model(verticesData, indicesData);
}

/*
* Send terrain data to the current shader program.
*/
void Terrain::DrawSpecific() {
	if (u_nr_cells != -1) {
		glUniform1f(u_nr_cells, (float)nr_cells);
	}

	if (u_offset_uv2 != -1) {
		glUniform2f(u_offset_uv2, offset_uv2.x, offset_uv2.y);
	}

	if (u_height != -1) {
		glUniform3f(u_height, heigths.x, heigths.y, heigths.z);
	}
}

Terrain::~Terrain() {}