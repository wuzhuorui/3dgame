// BoxDemo.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates rendering a colored box.
//
// Controls:
//		Hold the left mouse button down and move the mouse to rotate.
//      Hold the right mouse button down to zoom in and out.
//
//***************************************************************************************

#pragma once
#include"../d3dapp/d3dapp.h"
#include"../common/math/vec2.h"
#include"../common/math/vec3.h"
#include"../common/math/vec4.h"
#include"../common/math/mat4.h"
#include"../common/math/transform.h"
#include"../common/color/color.h"

class HillsApp :public D3DApp
{
public:
	HillsApp(HINSTANCE hInstance);
	~HillsApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	ID3DX11EffectMatrixVariable* GetmfxWorldViewProj() {
		return mfxWorldViewProj;
	};
	ID3DX11EffectTechnique* GetmTech() {
		return mTech;
	}
private:
	float GetHeight(float x, float z)const;
	void BuildGeometryBuffers();
	void BuildFX();
	void BuildVertexLayout();
private:

	ID3DX11Effect* mFX;
	ID3DX11EffectTechnique* mTech;
	ID3DX11EffectMatrixVariable* mfxWorldViewProj;

	ID3D11InputLayout* mInputLayout;

	mat4f mView;
	mat4f mProj;

	float mTheta;
	float mPhi;
	float mRadius;

	UINT mGridIndexCount;

	Point2i mLastMousePos;
};