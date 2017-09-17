#include"gameobject.h"
#include"../../common/utils/utils.h"
#include"../../d3dapp/d3dapp.h"
GameObject::GameObject(D3DApp* mgr)
	:mVB(0), mIB(0), mFlag(0),Mgr(mgr)
{
	mRelativeWorld.reset(new mat4f());
	mFlag &= status::mRelativeWorldDirty;
}

GameObject::~GameObject()
{
	GlobalUtils->Release(mIB);
	GlobalUtils->Release(mVB);
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

void GameObject::SetRotate()
{
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

void GameObject::DrawScene(const mat4f& VP, const mat4f& ParentRelativeWorld, std::function<void(ID3DX11EffectTechnique*, ID3D11DeviceContext*)> fun)
{
	UINT offset = 0;
	Mgr->Getmd3dImmediateContext()->IASetVertexBuffers(0, 1, &mVB, &VertexSize, &offset);
	Mgr->Getmd3dImmediateContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	mat4f mWorld = ParentRelativeWorld*(*this->GetWorldMat());
	mat4f WVP = mWorld*VP;

	Mgr->GetmfxWorldViewProj()->SetMatrix(reinterpret_cast<float*>(&WVP[0][0]));
	fun(Mgr->GetmTech(), Mgr->Getmd3dImmediateContext());

	Mgr->Getmd3dImmediateContext()->DrawIndexed(IndexNum, 0, 0);

	for (auto& child : mChildren)
		(child)->DrawScene(VP, mWorld,fun);
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
