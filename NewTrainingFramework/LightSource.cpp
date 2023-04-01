#pragma once
#include "stdafx.h"

#include "LightSource.h"

using namespace light;

LightSource::LightSource(int id, light_type type, Vector3 spec_light_color, Vector3 difuze_light_color) :
	id(id), spec_light_color(spec_light_color), difuze_light_color(difuze_light_color)
{
	light_color = Vector3();
	setUp(type);
}

LightSource::LightSource(int id, light_type type, Vector3 light_color) :
	id(id), light_color(light_color)
{
	spec_light_color = Vector3();
	difuze_light_color = Vector3();
	setUp(type);
}

LightSource::LightSource(int id, light_type type) :
	id(id)
{
	light_color = Vector3();
	spec_light_color = Vector3();
	difuze_light_color = Vector3();
	setUp(type);
}

LightSource::LightSource() : LightSource(0, light_type::PUNCTIFORM) {}

void LightSource::setUp(light_type type) {
	switch (type) {
	case light_type::DIRECTIONAL: this->type = 0; break;
	case light_type::PUNCTIFORM: this->type = 1; break;
	case light_type::SPOTLIGHT: this->type = 2;
	}
}

LightSource::~LightSource() {

}