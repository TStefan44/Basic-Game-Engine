#pragma once

#include "../Utilities/utilities.h"

#include "SceneObject.h"

namespace scene_obj {
	class Terrain : public SceneObject {
	public:
		Terrain(res::Shader* shader, std::vector<res::Texture*>* textures);
		Terrain();
		~Terrain() final;

		void SetDimensionsTerrain(int nr_cells, int dim_cell);

	private:
		int nr_cells;
		int dim_cell;
	};
}