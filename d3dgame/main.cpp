//***************************************************************************************
// Init Direct3D.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates the sample framework by initializing Direct3D, clearing 
// the screen, and displaying frame stats.
//
//***************************************************************************************

#include "d3dapp/d3dApp.h"
#include "common\debug\debug.h"
#include<cassert>
#include"common/math/transform.h"
#include"common/color/color.h"
class InitDirect3DApp : public D3DApp
{
public:
	InitDirect3DApp(HINSTANCE hInstance);
	~InitDirect3DApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	mat4<double> p(Vec4<double>(1, 0, 0, 0), Vec4<double>(0, 1, 0, 0), Vec4<double>(0, 0, 1, 0), Vec4<double>(3, 4, 5, 1));
	Vec4d x(3, 4, 5, 0);
	auto p1 = Perspective(pi_unit * 90, 1., 5., 10.);
	GlobalDebug->loginfo(Colors::Red);
	InitDirect3DApp theApp(hInstance);
	
	if( !theApp.Init() )
		return 0;
	
	return theApp.Run();
}

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance)
: D3DApp(hInstance) 
{
}

InitDirect3DApp::~InitDirect3DApp()
{
}

bool InitDirect3DApp::Init()
{
	if(!D3DApp::Init())
		return false;

	return true;
}

void InitDirect3DApp::OnResize()
{
	D3DApp::OnResize();
}

void InitDirect3DApp::UpdateScene(float dt)
{

}

void InitDirect3DApp::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);
	float Silver[] = { 1.00f, 0.f, 0.f, 1.0f };
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Silver));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	auto p1 = LookAt(Vec4d(1, 0, 1, 1), Vec4d(0, 0, 0, 1));

	mSwapChain->Present(0, 0);
}
