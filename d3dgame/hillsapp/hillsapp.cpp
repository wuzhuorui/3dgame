#include"hillsapp.h"
#include"../GeometryGenerator/GeometryGenerator.h"
#include<cassert>
struct Vertex
{
	Point3f Pos;
	Color4f Color;
};

HillsApp::HillsApp(HINSTANCE hInstance)
	:D3DApp(hInstance), root(this),
	mFX(nullptr),mTech(nullptr),mfxWorldViewProj(nullptr),
	mInputLayout(nullptr),mGridIndexCount(0),mTheta(1.5f*pi),
	mPhi(0.1f*pi),mRadius(200.f)
{
	mMainWndCaption = L"Hills Demo";

	mLastMousePos.SetX(0);
	mLastMousePos.SetY(0);
}

HillsApp::~HillsApp()
{
	GlobalUtils->Release(mFX);
	GlobalUtils->Release(mInputLayout);
}

bool HillsApp::Init()
{
	if (!D3DApp::Init())
	{
		return false;
	}

	BuildGeometryBuffers();
	BuildFX();
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

	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mat4f ViewProj = mView*mProj;

	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		root.DrawScene(ViewProj, mat4f());
	}

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

float HillsApp::GetHeight(float x, float z)const
{
	return 0.3f*(z*sinf(0.1f*x) + x*cosf(0.1f*z));
}

void HillsApp::BuildGeometryBuffers()
{
	GeometryGenerator::MeshData grid;

	GeometryGenerator geoGen;

	geoGen.CreateGrid(160.f, 160.f, 50, 50, grid);


	mGridIndexCount = grid.Indices.size();

	std::vector<Vertex> vertices(grid.Vertices.size());
	for (size_t i = 0; i < grid.Vertices.size(); ++i)
	{
		Point3f p = grid.Vertices[i].Position;

		p[1] = GetHeight(p[0], p[2]);

		vertices[i].Pos = p;

		// Color the vertex based on its height.
		if (p[1] < -10.0f)
		{
			// Sandy beach color.
			vertices[i].Color = Color4f(1.0f, 0.96f, 0.62f, 1.0f);
		}
		else if (p[1] < 5.0f)
		{
			// Light yellow-green.
			vertices[i].Color = Color4f(0.48f, 0.77f, 0.46f, 1.0f);
		}
		else if (p[1] < 12.0f)
		{
			// Dark yellow-green.
			vertices[i].Color = Color4f(0.1f, 0.48f, 0.19f, 1.0f);
		}
		else if (p[1] < 20.0f)
		{
			// Dark brown
			vertices[i].Color = Color4f(0.45f, 0.39f, 0.34f, 1.0f);
		}
		else
		{
			// White snow.
			vertices[i].Color = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	root.InitVB(sizeof(Vertex), vertices.size(), &vertices[0]);
	root.InitIB(sizeof(UINT), mGridIndexCount, &grid.Indices[0]);
}

void HillsApp::BuildVertexLayout()
{
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA ,0},
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DX11_PASS_DESC passDesc;
	mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	md3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &mInputLayout
	);
}

void HillsApp::BuildFX()
{
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT hr = D3DX11CompileFromFile(L"D:\\c++\\3dgame\\trunk\\d3dgame\\hillsapp\\color.fx", 0, 0, 0, "fx_5_0", shaderFlags,
		0, 0, &compiledShader, &compilationMsgs, 0);
	// compilationMsgs can store errors or warnings.
	if (compilationMsgs != 0)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		if (compilationMsgs)compilationMsgs->Release();
	}
	assert(hr == S_OK);
	assert(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
		0, md3dDevice, &mFX) == S_OK);

	// Done with compiled shader.
	if (compiledShader)compiledShader->Release();
	mTech = mFX->GetTechniqueByName("ColorTech");
	mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}
