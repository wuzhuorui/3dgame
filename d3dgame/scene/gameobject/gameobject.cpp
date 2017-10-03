#include"gameobject.h"
#include"../../common/utils/utils.h"
#include"../../d3dapp/d3dapp.h"
#include"../../GeometryGenerator/GeometryGenerator.h"
#include"../../common/shader/colorshader.h"
#include"../../common/lib/d3dx11effect.h"

struct Vertex
{
	Point3f Pos;
	Color4f Color;
};

GameObject::GameObject(D3DApp* mgr, std::shared_ptr<class Shader> shader)
	:mVB(0), mIB(0), mFlag(0),Mgr(mgr),mShader(shader)
{
	mRelativeWorld.reset(new mat4f());
	mFlag &= status::mRelativeWorldDirty;
}

GameObject::~GameObject()
{
	GlobalUtils->Release(mIB);
	GlobalUtils->Release(mVB);
	GlobalUtils->Release(mInputLayout);
}

void GameObject::BuildVertexLayout(UINT PassIndex)
{
	this->mPassIndex = PassIndex;
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA ,0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DX11_PASS_DESC passDesc;
	ColorShader* cshader = (ColorShader*)&*mShader;
	cshader->mTech->GetPassByIndex(mPassIndex)->GetDesc(&passDesc);
	Mgr->Getmd3dDevice()->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &mInputLayout
	);
}

void GameObject::BuildGeometryBuffers()
{
	GeometryGenerator::MeshData grid;

	GeometryGenerator geoGen;

	geoGen.CreateGrid(160.f, 160.f, 50, 50, grid);


	int mGridIndexCount = grid.Indices.size();

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
	InitVB(sizeof(Vertex), vertices.size(), &vertices[0]);
	InitIB(sizeof(UINT), mGridIndexCount, &grid.Indices[0]);
}

void GameObject::InitVB(size_t Vsize,size_t Vnum, const void *address)
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = Vsize*Vnum;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = address;
	Mgr->Getmd3dDevice()->CreateBuffer(&vbd, &vinitData, &mVB);

	VertexNum = Vnum;
	VertexSize = Vsize;
}

void GameObject::InitIB(size_t Isize,size_t Inum, const void *address)
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = Isize*Inum;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = address;
	Mgr->Getmd3dDevice()->CreateBuffer(&ibd, &iinitData, &mIB);

	IndexNum = Inum;
	IndexSize = Isize;
}

void GameObject::SetScale(float x, float y, float z)
{

}

void GameObject::SetScale(std::shared_ptr<mat4f>& scale)
{
}

void GameObject::ClearScale()
{
}

void GameObject::SetTranslation(float x, float y, float z)
{
	if (!Translation)
	{
		Translation.reset(new mat4f());
		mFlag |= status::mTranslation;
	}
	*Translation = ::Translation(x, y, z);
	mFlag |= status::mRelativeWorldDirty;
}

void GameObject::SetTranslation(std::shared_ptr<mat4f>& translation)
{
}

void GameObject::ClearTranslation()
{
	Translation.reset();
	mFlag &= status::mRelativeWorldDirty;
}

void GameObject::SetRotate(float theta)
{
	static float m_rotate = 0.f;
	m_rotate += theta;
	if (!Rotate)
	{
		Rotate.reset(new mat4f());
		mFlag |= status::mRotate;
	}
	*Rotate = ::RotateY(m_rotate);
	mFlag |= status::mRelativeWorldDirty;
}

void GameObject::ClearRotate()
{
}

std::shared_ptr<mat4f> GameObject::GetWorldMat()
{
	if (mFlag & status::mRelativeWorldDirty)
	{
		mat4f ret;
		if (mFlag & status::mScaleMat)
			ret = ret*(*Scale);
		if (mFlag & status::mRotate)
			ret = ret*(*Rotate);
		if (mFlag & status::mTranslation)
			ret = ret*(*Translation);
		*mRelativeWorld = ret;
		mFlag &= ~status::mRelativeWorldDirty;
	}

	return mRelativeWorld;
}

void GameObject::DrawScene(const mat4f& VP, const mat4f& ParentRelativeWorld)
{
	Mgr->Getmd3dImmediateContext()->IASetInputLayout(mInputLayout);
	Mgr->Getmd3dImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT offset = 0;
	Mgr->Getmd3dImmediateContext()->IASetVertexBuffers(0, 1, &mVB, &VertexSize, &offset);
	Mgr->Getmd3dImmediateContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	mat4f mWorld =(*this->GetWorldMat())*ParentRelativeWorld;
	mat4f WVP = mWorld*VP;
	ColorShader* cshader = (ColorShader*)&*mShader;

	cshader->mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&WVP));
	cshader->mTech->GetPassByIndex(this->mPassIndex)->Apply(0, Mgr->Getmd3dImmediateContext());

	Mgr->Getmd3dImmediateContext()->DrawIndexed(IndexNum, 0, 0);

	for (auto& child : mChildren)
		(child)->DrawScene(VP, mWorld);
}

void GameObject::UpdateScene(float dt)
{
	//this->SetRotate(dt);
	for (auto& child : mChildren)
		(child)->UpdateScene(dt);
}

void GameObject::AddChild(const std::shared_ptr<GameObject>& child)
{
	mChildren.insert(mChildren.end(), child);
}

void GameObject::DeleteChild(const std::shared_ptr<GameObject>& child)
{
	for (auto elem = mChildren.begin() ;elem!=mChildren.end(); elem++)
	{
		if (*elem == child)
		{
			mChildren.erase(elem);
		}
	}
}

float GameObject::GetHeight(float x, float z)const
{
	return 0.3f*(z*sinf(0.1f*x) + x*cosf(0.1f*z));
}
