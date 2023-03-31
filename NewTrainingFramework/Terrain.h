#pragma once

#include "SceneObject.h"

#include "../Utilities/utilities.h"

namespace scene_obj {
	class Terrain : public SceneObject {
	public:
		// Constructors
		Terrain(res::Shader* shader, std::vector<res::Texture*>* textures);
		Terrain();
		~Terrain();

		// Core functions
		void SetDimensionsTerrain(int nr_cells, int dim_cell, Vector3 heights);
		void Update(ESContext* esContext, float deltaTime) override;

	protected:
		void DrawSpecific() override;

	private:
		res::Model* GenerateModelTerrain();

	private:
		// Class variables
		int nr_cells;
		int dim_cell;
		Vector3 heigths;

		float offset;
		Vector2 offset_uv2;

		// Shader locations
		GLint u_height;
		GLint u_nr_cells;
		GLint u_offset_uv2;
	};
}