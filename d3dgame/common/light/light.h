#pragma once
#include<Windows.h>
#include"../math/vec4.h"
#include"../math/vec3.h"

struct DirectionalLight
{
public:
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	Vec4f Ambient;
	Vec4f Diffuse;
	Vec4f Specular;
	Vec3f Direction;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	Vec4f Ambient;
	Vec4f Diffuse;
	Vec4f Specular;

	// Packed into 4D vector: (Position, Range)
	Vec3f Position;
	float Range;

	// Packed into 4D vector: (A0, A1, A2, Pad)
	Vec3f Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	Vec4f Ambient;
	Vec4f Diffuse;
	Vec4f Specular;

	// Packed into 4D vector: (Position, Range)
	Vec3f Position;
	float Range;

	// Packed into 4D vector: (Direction, Spot)
	Vec3f Direction;
	float Spot;

	// Packed into 4D vector: (Att, Pad)
	Vec3f Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};