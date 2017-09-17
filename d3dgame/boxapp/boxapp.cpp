#include"boxapp.h"
#include<cassert>


BoxApp::BoxApp(HINSTANCE hInstance)
	: D3DApp(hInstance), mBoxVB(0), mBoxIB(0), mFX(0), mTech(0),
	mfxWorldViewProj(0), mInputLayout(0),
	mTheta(1.5f*pi), mPhi(0.25f*pi), mRadius(5.0f)
{
	mMainWndCaption = L"Box Demo";

	mLastMousePos.SetX(0);
	mLastMousePos.SetY(0);
}

BoxApp::~BoxApp()
{
	if (mBoxVB)mBoxVB->Release();

	if (mBoxIB)mBoxIB->Release();
	
	if (mFX)mFX->Release();
	
	if (mInputLayout)mInputLayout->Release();
}

bool BoxApp::Init()
{
	if (!D3DApp::Init())
		return false;

	BuildGeometryBuffers();
	BuildFX();
	BuildVertexLayout();

	return true;
}

void BoxApp::BuildGeometryBuffers()
{
	// Create vertex buffer
	Vertex vertices[] =
	{
		{ Point3f(-1.0f, -1.0f, -1.0f),Colors::White },
		{ Point3f(-1.0f, +1.0f, -1.0f),Colors::Black },
		{ Point3f(+1.0f, +1.0f, -1.0f),Colors::Red },
		{ Point3f(+1.0f, -1.0f, -1.0f),Colors::Green },
		{ Point3f(-1.0f, -1.0f, +1.0f),Colors::Blue },
		{ Point3f(-1.0f, +1.0f, +1.0f),Colors::Yellow },
		{ Point3f(+1.0f, +1.0f, +1.0f),Colors::Cyan },
		{ Point3f(+1.0f, -1.0f, +1.0f),Colors::Magenta }
	};
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	assert(md3dDevice->CreateBuffer(&vbd, &vinitData, &mBoxVB) == S_OK);

	// Create the index buffer

	UINT indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	assert(md3dDevice->CreateBuffer(&ibd, &iinitData, &mBoxIB) == S_OK);
}

void BoxApp::BuildFX()
{
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT hr = D3DX11CompileFromFile(L"D:\\c++\\3dgame\\trunk\\d3dgame\\boxapp\\color.fx",0, 0, 0, "fx_5_0", shaderFlags,
		0, 0, &compiledShader, &compilationMsgs, 0);
	// compilationMsgs can store errors or warnings.
	if (compilationMsgs != 0)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		if (compilationMsgs)compilationMsgs->Release();
	}
	assert(hr == S_OK);
	assert(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
		0, md3dDevice, &mFX)==S_OK);

	// Done with compiled shader.
	if (compiledShader)compiledShader->Release();

	mTech = mFX->GetTechniqueByName("ColorTech");
	mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void BoxApp::BuildVertexLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	assert(md3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &mInputLayout) == S_OK);
}

/*-------------------------------------------------------------------------------------*/

void BoxApp::OnResize()
{
	D3DApp::OnResize();

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	mProj = Perspective(0.5f*(static_cast<float>(pi)),this->AspectRatio(), 1.0f, 1000.0f);
}

void BoxApp::UpdateScene(float dt)
{
	// Convert Spherical to Cartesian coordinates.
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);

	// Build the view matrix.
	Point4f pos = Point4f(x, y, z, 1.0f);
	Point4f target = Point4f(0.f, 0.f, 0.f, 1.f);
	Point4f up = Point4f(0.0f, 1.0f, 0.0f, 0.0f);

	mView = LookAt(pos, target, up);
}

void BoxApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::White[0]));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f, 0);

	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mBoxVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mBoxIB, DXGI_FORMAT_R32_UINT, 0);

	// Set constants
	mat4f worldViewProj = mWorld*mView*mProj;
	mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj[0][0]));

	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);

		// 36 indices for the box.
		md3dImmediateContext->DrawIndexed(36, 0, 0);
	}

	mSwapChain->Present(0, 0);
}

void BoxApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.SetX(x);
	mLastMousePos.SetY(y);

	SetCapture(mhMainWnd);
}

void BoxApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void BoxApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx =0.25f*static_cast<float>(x - mLastMousePos.x())*pi_unit;
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
		float dx = 0.005f*static_cast<float>(x - mLastMousePos.x());
		float dy = 0.005f*static_cast<float>(y - mLastMousePos.y());

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = mRadius < 3.f ? 3.f : (mRadius > 15.f ? 15.f : mRadius);
	}

	mLastMousePos.SetX(x);
	mLastMousePos.SetY(y);
}