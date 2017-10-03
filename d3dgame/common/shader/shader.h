#pragma once
#include<string>

class Shader
{
public:
	Shader(const std::wstring& path);
	virtual  ~Shader();
	virtual bool BuildFX(struct ID3D11Device*) = 0;

	virtual void UpdateScene(float dt,void* Mgr) = 0;
	virtual void DrawScene() = 0;

	bool InitFX(struct ID3D11Device* md3dDevice);
	struct ID3DX11Effect* mFX;
	std::wstring mPath; 
};