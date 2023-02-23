#pragma once
#include "stdafx.h"

#include "Terrain.h"

using namespace scene_obj;

Terrain::Terrain(res::Shader* shader, std::vector<res::Texture*>* textures) : 
	SceneObject(nullptr, shader, textures) {
	// TODO: create model;
}

Terrain::Terrain() : Terrain(nullptr, nullptr) {}

void Terrain::SetDimensionsTerrain(int nr_cells, int dim_cell) {
	this->nr_cells = nr_cells;
	this->dim_cell = dim_cell;
}

Terrain::~Terrain() {}
