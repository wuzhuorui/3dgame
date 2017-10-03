#include "waterobject.h"
#include"../../d3dapp/d3dapp.h"
#include"../../common/shader/watershader.h"
#include"../../common/lib/d3dx11effect.h"
#include"../../GeometryGenerator/GeometryGenerator.h"

struct VertexPN
{
	Point3f Pos;
	Dir3f Normal;
};
WaterObject::WaterObject(D3DApp * mgr, std::shared_ptr<class Shader> mshader)
	:GameObject(mgr, mshader)
{
	water_mat.Ambient = Vec4f(0.137f, 0.42f, 0.556f, 1.0f);
	water_mat.Diffuse = Vec4f(0.137f, 0.42f, 0.556f, 1.0f);
	water_mat.Specular = Vec4f(0.8f, 0.8f, 0.8f, 96.0f);
}

WaterObject::~WaterObject()
{

}

void WaterObject::UpdateScene(float dt)
{
	//this->SetRotate(dt);
	for (auto& child : mChildren)
		(child)->UpdateScene(dt);
}

void WaterObject::BuildVertexLayout(UINT PassIndex)
{
	this->mPassIndex = PassIndex;
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	WaterShader* cshader = (WaterShader*)&*mShader;
	cshader->mTech->GetPassByIndex(this->mPassIndex)->GetDesc(&passDesc);

	Mgr->Getmd3dDevice()->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &mInputLayout
	);
}

void WaterObject::BuildGeometryBuffers()
{
	GeometryGenerator::MeshData grid;

	GeometryGenerator geoGen;

	geoGen.CreateGrid(160.f, 160.f, 50, 50, grid);


	int mGridIndexCount = grid.Indices.size();

	std::vector<VertexPN> vertices(grid.Vertices.size());
	for (size_t i = 0; i < grid.Vertices.size(); ++i)
	{
		Point3f p = grid.Vertices[i].Position;

		p[1] = GetHeight(p[0], p[2]);

		vertices[i].Pos = p;
		vertices[i].Normal = grid.Vertices[i].Normal;
	}
	InitVB(sizeof(VertexPN), vertices.size(), &vertices[0]);
	InitIB(sizeof(UINT), mGridIndexCount, &grid.Indices[0]);
}

void WaterObject::DrawScene(const mat4f & VP, const mat4f & ParentRelativeWorld)
{
	Mgr->Getmd3dImmediateContext()->IASetInputLayout(mInputLayout);
	Mgr->Getmd3dImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT offset = 0;
	Mgr->Getmd3dImmediateContext()->IASetVertexBuffers(0, 1, &mVB, &VertexSize, &offset);
	Mgr->Getmd3dImmediateContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	mat4f mWorld = (*this->GetWorldMat())*ParentRelativeWorld;
	mat4f WVP = mWorld*VP;
	WaterShader* cshader = (WaterShader*)&*mShader;
	cshader->DrawScene();
	cshader->mfxMaterial->SetRawValue(&water_mat, 0, sizeof(water_mat));
	cshader->mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&WVP[0][0]));
	cshader->mfxWorld->SetMatrix(reinterpret_cast<float*>(&mWorld[0][0]));
	cshader->mfxWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&mWorld[0][0]));
	cshader->mTech->GetPassByIndex(this->mPassIndex)->Apply(0, Mgr->Getmd3dImmediateContext());

	Mgr->Getmd3dImmediateContext()->DrawIndexed(IndexNum, 0, 0);

	for (auto& child : mChildren)
		(child)->DrawScene(VP, mWorld);
}
