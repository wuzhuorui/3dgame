#include"GeometryGenerator.h"

void GeometryGenerator::CreateGrid(float width, float depth, unsigned m, unsigned n, MeshData& meshData)
{
	unsigned vertexCount = m*n;
	unsigned faceCount = (m - 1)*(n - 1) * 2;

	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	meshData.Vertices.resize(vertexCount);

	for (unsigned i = 0; i < m; ++i)
	{
		float z = halfDepth - dz*i;
		for (unsigned j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;

			Vertex& _v	= meshData.Vertices[i*n + j];
			_v.Position = Point3f(x, 0.f, z);
			_v.Normal	= Dir3f(0.f, 1.f, 0.f);
			_v.TangentU = Dir3f(1.f, 0.f, 0.f);
			_v.TexC		= Vec2f(j*du, i*dv);
		}
	}

	meshData.Indices.resize(faceCount * 3);
	std::vector<unsigned>& t_index = meshData.Indices;
	unsigned k = 0;
	for (unsigned i = 0; i < m - 1; ++i)
	{
		for (unsigned j = 0; j < n - 1; j++)
		{
			t_index[k]		= i*n + j;
			t_index[k + 1]	= i*n + j + 1;
			t_index[k + 2]	= (i + 1)*n + j;

			t_index[k + 3] = (i + 1)*n + j;
			t_index[k + 4] = i*n + j + 1;
			t_index[k + 5] = (i + 1)*n + j + 1;

			k += 6;
		}
	}
}