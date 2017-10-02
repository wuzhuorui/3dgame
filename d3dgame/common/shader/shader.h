#pragma once

class Shader
{
public:
	Shader();
	virtual  ~Shader();
	virtual bool BuildFX(struct ID3D11Device*) = 0;
	bool InitFX(struct ID3D11Device* md3dDevice);
	struct ID3DX11Effect* mFX;
};