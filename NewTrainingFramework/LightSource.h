#pragma once

#include "../Utilities/utilities.h"

namespace light {
	enum class light_type { DIRECTIONAL = 0, PUNCTIFORM = 1, SPOTLIGHT = 2 };

	class LightSource {
	public:
		LightSource(int id, light_type type);
		LightSource(int id, light_type type, Vector3 light_color);
		LightSource(int id, light_type type, Vector3 spec_light_color, Vector3 difuze_light_color);
		LightSource();
		~LightSource();

	protected:
		inline void setUp(light_type type);

	public:
		int id;
		int type;
		Vector3 light_position;
		Vector3 light_direction;
		Vector3 light_color;
		Vector3 spec_light_color;
		Vector3 difuze_light_color;

		// Spotlight component
		float cut_off;
	};
}