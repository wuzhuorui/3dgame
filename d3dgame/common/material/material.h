#pragma once
#include<Windows.h>
#include"../math/vec4.h"

class Material
{
public:
	Material() { ZeroMemory(this, sizeof(this)); }

	Vec4f Ambient;
	Vec4f Diffuse;
	Vec4f Specular; // w = SpecPower
	Vec4f Reflect;
};