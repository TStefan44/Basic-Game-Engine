#pragma once
#include "../Utilities/Math.h"

struct Vertex
{
	Vertex(Vector3 pos, Vector3 color, Vector3 normal, Vector3 binormal, Vector3 tgt, Vector2 uv) :
		pos(pos), color(color), normal(normal), binormal(binormal), tgt(tgt), uv(uv) { }

	Vertex(Vector3 pos, Vector3 color) :
		Vertex(pos, color, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector2(0, 0)) { }

	Vertex() : Vertex(Vector3(0, 0, 0), Vector3(0, 0, 0)) { }

	friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex) {
		os << "Position = " << vertex.pos << " Color = " << vertex.color << " Normal = " << vertex.normal
			<< " Binormal = " << vertex.binormal << " tgt = " << vertex.tgt << " UV = " << vertex.uv << "\n";
		return os;
	}

	// vertex attributes
	Vector3 pos;
	Vector3 color;
	Vector3 normal;
	Vector3 binormal;
	Vector3 tgt;
	Vector2 uv;
};