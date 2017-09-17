#pragma once
#include"../common/math/vec4.h"
#include"../common/math/vec3.h"
#include"../common/math/vec2.h"
#include<vector>

class GeometryGenerator
{
public:
	struct Vertex
	{
		Vertex() {}
		Vertex(const Point3f& p, const Dir3f& n, const Dir3f& t, const Vec2f& uv)
			: Position(p), Normal(n), TangentU(t), TexC(uv) {}
		Vertex(
			float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v)
			: Position(px, py, pz), Normal(nx, ny, nz),
			TangentU(tx, ty, tz), TexC(u, v) {}

		Point3f Position;
		Dir3f Normal;
		Dir3f TangentU;
		Vec2f TexC;
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<unsigned> Indices;
	};

	void CreateGrid(float width, float depth, unsigned m, unsigned n, MeshData& meshData);
};