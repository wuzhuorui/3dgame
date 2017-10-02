#include"hillsapp.h"
#include<cassert>
#include<functional>
#include"../common/shader/colorshader.h"

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

	mView = LookAt(Point4f(x, y, z, 1), Point4f(0.f, 0.f, 0.f, 1.f), Dir4f(0.f, 1.f, 0.f, 0.f));
}

void HillsApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, &Colors::LightSteelBlue[0]);
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f,0);

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
	floor.reset(new GameObject(this,mShader[0]));
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
	mShader.push_back(std::shared_ptr<Shader>(new ColorShader()));
	mShader[0]->BuildFX(md3dDevice);
}
