#include"hillsapp.h"
#include<cassert>
#include<functional>
#include"../common/shader/colorshader.h"
#include"../common/shader/watershader.h"
#include"../scene/waterobject/waterobject.h"
HillsApp::HillsApp(HINSTANCE hInstance)
	:D3DApp(hInstance),
	mTheta(1.5f*pi),
	mPhi(0.1f*pi),mRadius(200.f)
{
	mMainWndCaption = L"Hills Demo";

	mLastMousePos.SetX(0);
	mLastMousePos.SetY(0);
}

HillsApp::~HillsApp()
{
	GlobalUtils->Release(mWireframeRS);
}

bool HillsApp::Init()
{
	if (!D3DApp::Init())
	{
		return false;
	}
	BuildFX();
	BuildGeometryBuffers();
	BuildVertexLayout();

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	(md3dDevice->CreateRasterizerState(&wireframeDesc, &mWireframeRS));

	return true;
}

void HillsApp::OnResize()
{
	D3DApp::OnResize();

	mProj = Perspective(0.25f*pi, AspectRatio(), 1.f, 1000.f);
}

void HillsApp::UpdateScene(float dt)
{
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);
	mEyePos = Vec3f(x, y, z);
	mView = LookAt(Point4f(x, y, z, 1), Point4f(0.f, 0.f, 0.f, 1.f), Dir4f(0.f, 1.f, 0.f, 0.f));

	root->UpdateScene(dt);
	mShader[0]->UpdateScene(dt, this);
	mShader[1]->UpdateScene(dt, this);

}

void HillsApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, &Colors::LightSteelBlue[0]);
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f,0);
	
	//md3dImmediateContext->RSSetState(mWireframeRS);
	
	mShader[0]->DrawScene();
	mShader[1]->DrawScene();
	mat4f ViewProj = mView*mProj;

	root->DrawScene(ViewProj, mat4f());

	mSwapChain->Present(0, 0);
}

void HillsApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.SetX(x);
	mLastMousePos.SetY(y);

	SetCapture(mhMainWnd);
}

void HillsApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void HillsApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = 0.25f*static_cast<float>(x - mLastMousePos.x())*pi_unit;
		float dy = 0.25f*static_cast<float>(y - mLastMousePos.y())*pi_unit;

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi += dy;

		// Restrict the angle mPhi.
		mPhi = mPhi < 0.1f ? 0.1f : (mPhi > pi - 0.1f ? pi - 0.1f : mPhi);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.2f*static_cast<float>(x - mLastMousePos.x());
		float dy = 0.2f*static_cast<float>(y - mLastMousePos.y());

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = mRadius < 50.f ? 50.f : (mRadius > 500.f ? 500.f : mRadius);
	}

	mLastMousePos.SetX(x);
	mLastMousePos.SetY(y);
}


void HillsApp::BuildGeometryBuffers()
{
	root.reset(new GameObject(this,mShader[0]));
	root->BuildGeometryBuffers();
	floor.reset(new WaterObject(this,mShader[1]));
	floor->SetTranslation(160.f, 0.f, 0.f);
	floor->BuildGeometryBuffers();
	root->AddChild(floor);
}

void HillsApp::BuildVertexLayout()
{
	root->BuildVertexLayout(0);
	floor->BuildVertexLayout(0);
}

void HillsApp::BuildFX()
{
	mShader.push_back(std::shared_ptr<Shader>(new ColorShader(L"D:\\c++\\3dgame\\trunk\\d3dgame\\hillsapp\\color.fx")));
	mShader[0]->BuildFX(md3dDevice);
	mShader.push_back(std::shared_ptr<Shader>(new WaterShader(L"D:\\c++\\3dgame\\trunk\\d3dgame\\shader\\Lighting.fx")));
	mShader[1]->BuildFX(md3dDevice);

}
