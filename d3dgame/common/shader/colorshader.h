#include"shader.h"
#include"../../common/utils/utils.h"
#include"../../common/lib/d3dx11effect.h"
class ColorShader :public Shader
{
public:
	ColorShader():mTech(nullptr), mfxWorldViewProj(nullptr){};
	~ColorShader() {
		GlobalUtils->Release(mTech);
		GlobalUtils->Release(mfxWorldViewProj);
	};
	virtual bool BuildFX(struct ID3D11Device*);
	struct ID3DX11EffectTechnique* mTech;
	struct ID3DX11EffectMatrixVariable* mfxWorldViewProj;
};