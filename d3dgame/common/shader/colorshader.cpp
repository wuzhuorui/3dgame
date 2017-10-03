#include "colorshader.h"
#include<d3d11.h>
#include<cassert>


void ColorShader::UpdateScene(float dt, void* Mgr)
{
}

void ColorShader::DrawScene()
{
}

bool ColorShader::BuildFX(ID3D11Device* md3dDevice)
{
	this->InitFX(md3dDevice);
	mTech = mFX->GetTechniqueByName("ColorTech");
	mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	return true;
}
