#include"watershader.h"
#include<d3d11.h>
#include<cassert>
#include<D3DX11async.h>
#include"../../common/lib/d3dx11effect.h"
#include"../../hillsapp/hillsapp.h"

void WaterShader::UpdateScene(float dt, void* Mgr)
{
	mtime += dt;
	mEyePosW = ((HillsApp*)Mgr)->GetEyePos();
	// Circle light over the land surface.
	plight.Position[0] = 70.0f*cosf(0.2f*dt);
	plight.Position[2] = 70.0f*sinf(0.2f*dt);
	plight.Position[1] = 100.f;


	// The spotlight takes on the camera position and is aimed in the
	// same direction the camera is looking.  In this way, it looks
	// like we are holding a flashlight.
	slight.Position = mEyePosW;
	slight.Direction = -mEyePosW;

	mWave[0].L = 50.f;
	mWave[0].S = 5.f;
	mWave[0].k = 4.f;
	mWave[0].A = 1.f;
	mWave[0].d1 = 1.f;
	mWave[0].d2 = 1.f;

	mWave[1].L = 30.f;
	mWave[1].S = 3.f;
	mWave[1].k = 4.f;
	mWave[1].A = 0.5f;
	mWave[1].d1 = 1.f;
	mWave[1].d2 = 1.5f;

	mWave[2].L = 10.f;
	mWave[2].S = 1.f;
	mWave[2].k = 4.f;
	mWave[2].A = 0.1f;
	mWave[2].d1 = 1.5f;
	mWave[2].d2 = 1.f;
}

void WaterShader::DrawScene()
{
	mfxDirLight->SetRawValue(&dlight, 0, sizeof(dlight));
	mfxPointLight->SetRawValue(&plight, 0, sizeof(plight));
	mfxSpotLight->SetRawValue(&slight, 0, sizeof(slight));

	mfxEyePosW->SetRawValue(&mEyePosW, 0, sizeof(mEyePosW));
	mfxTime->SetRawValue(&mtime, 0, sizeof(float));
	mfxWave->SetRawValue(mWave, 0, sizeof(Wave)*3);
}

bool WaterShader::BuildFX(ID3D11Device* md3dDevice)
{
	this->InitFX(md3dDevice);
	mTech = mFX->GetTechniqueByName("WaterTech");
	mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	mfxWorld = mFX->GetVariableByName("gWorld")->AsMatrix();
	mfxWorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	mfxEyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	mfxDirLight = mFX->GetVariableByName("gDirLight");
	mfxPointLight = mFX->GetVariableByName("gPointLight");
	mfxSpotLight = mFX->GetVariableByName("gSpotLight");
	mfxMaterial = mFX->GetVariableByName("gMaterial");
	mfxTime = mFX->GetVariableByName("gTime");
	mfxWave = mFX->GetVariableByName("wave");
	return true;
}
