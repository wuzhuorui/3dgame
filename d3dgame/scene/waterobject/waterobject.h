#pragma once

#include"../gameobject/gameobject.h"
#include"../../common/material/material.h"

class WaterObject: public GameObject
{
public:
	WaterObject(D3DApp* mgr, std::shared_ptr<class Shader>);
	virtual ~WaterObject();
	virtual void BuildVertexLayout(UINT PassIndex);
	virtual void BuildGeometryBuffers();
	virtual void DrawScene(const mat4f& VP, const mat4f& ParentRelativeWorld);
	virtual void UpdateScene(float dt);
	Material water_mat;
};