#pragma once
#include"shader.h"
#include"../../common/utils/utils.h"
#include"../../common/light/light.h"

class WaterShader :public Shader
{
public:
	WaterShader(const std::wstring path) :Shader(path),mTech(nullptr), mfxWorldViewProj(nullptr) {
		dlight.Ambient = Vec4f(0.2f, 0.2f, 0.2f, 1.0f);
		dlight.Diffuse = Vec4f(0.5f, 0.5f, 0.5f, 1.0f);
		dlight.Specular = Vec4f(0.5f, 0.5f, 0.5f, 1.0f);
		dlight.Direction = Vec3f(0.57735f, -0.57735f, 0.57735f);

		// Point light--position is changed every frame to animate in UpdateScene function.
		plight.Ambient = Vec4f(0.3f, 0.3f, 0.3f, 1.0f);
		plight.Diffuse = Vec4f(0.7f, 0.7f, 0.7f, 1.0f);
		plight.Specular = Vec4f(0.7f, 0.7f, 0.7f, 1.0f);
		plight.Att = Vec3f(0.0f, 0.1f, 0.0f);
		plight.Range = 1000.0f;

		// Spot light--position and direction changed every frame to animate in UpdateScene function.
		slight.Ambient = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
		slight.Diffuse = Vec4f(1.0f, 1.0f, 0.0f, 1.0f);
		slight.Specular = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		slight.Att = Vec3f(1.0f, 0.0f, 0.0f);
		slight.Spot = 96.0f;
		slight.Range = 10000.0f;

		mtime = 0;
	};
	~WaterShader() {
		GlobalUtils->Release(mTech);
		GlobalUtils->Release(mfxWorldViewProj);
		GlobalUtils->Release(mfxWorld);
		GlobalUtils->Release(mfxWorldInvTranspose);
		GlobalUtils->Release(mfxEyePosW);
		GlobalUtils->Release(mfxPointLight);
		GlobalUtils->Release(mfxSpotLight);
		GlobalUtils->Release(mfxMaterial);
		GlobalUtils->Release(mfxWave);
	};
	virtual void UpdateScene(float dt, void* Mgr);
	virtual void DrawScene();
	virtual bool BuildFX(struct ID3D11Device*);
	struct ID3DX11EffectTechnique* mTech;
	struct ID3DX11EffectMatrixVariable* mfxWorldViewProj;
	struct ID3DX11EffectMatrixVariable* mfxWorld;
	struct ID3DX11EffectMatrixVariable* mfxWorldInvTranspose;
	struct ID3DX11EffectVectorVariable* mfxEyePosW;
	struct ID3DX11EffectVariable* mfxDirLight;
	struct ID3DX11EffectVariable* mfxPointLight;
	struct ID3DX11EffectVariable* mfxSpotLight;
	struct ID3DX11EffectVariable* mfxMaterial;
	struct ID3DX11EffectVariable* mfxTime;
	struct ID3DX11EffectVariable* mfxWave;

	DirectionalLight dlight;
	PointLight plight;
	SpotLight slight;
	struct Wave {
		float L;
		float S;
		float k;
		float A;
		float d1;
		float d2;
		float t1;
		float t2;
	};
	Wave mWave[4];
	Vec3f mEyePosW;
	float mtime;
};