#pragma once
#include<vector>
#include<d3d11.h>
#include"../../common/math/transform.h"
#include"../../common/math/vec4.h"
#include"../../common/math/vec3.h"
#include"../../common/math/vec2.h"
#include<memory>
#include<functional>
class D3DApp;

class GameObject
{
public:
	GameObject(D3DApp* mgr, std::shared_ptr<class Shader>);
	virtual ~GameObject();
	virtual void BuildVertexLayout(UINT PassIndex);
	virtual void BuildGeometryBuffers();
	
	void SetScale(float x,float y,float z);
	void SetScale(std::shared_ptr<mat4f>& scale);
	void ClearScale();

	void SetTranslation(float x, float y, float z);
	void SetTranslation(std::shared_ptr<mat4f>& translation);
	void ClearTranslation();

	void SetRotate();
	void ClearRotate();

	std::shared_ptr<mat4f> GetWorldMat();

	virtual void DrawScene(const mat4f& VP,const mat4f& ParentRelativeWorld);

	void AddChild(const std::shared_ptr<GameObject>& child);
	void DeleteChild(const std::shared_ptr<GameObject>& child);
private:
	float GetHeight(float x, float z)const;

	void InitVB(size_t, size_t, const void*);
	void InitIB(size_t, size_t, const void*);

	enum  status {
		mVBOK = 0x1, mIBOK = 0x2, mScaleMat = 0x4, mTranslation = 0x8, mRotate = 0x10,
		mRelativeWorldDirty = 0x20
	};
	std::vector<std::shared_ptr<GameObject>> mChildren;
	
	ID3D11Buffer* mVB;
	size_t VertexNum;
	size_t VertexSize;
	ID3D11Buffer* mIB;
	size_t IndexNum;
	size_t IndexSize;

	std::shared_ptr<class Shader> mShader;

	UINT mPassIndex;
	ID3D11InputLayout* mInputLayout;

	unsigned mFlag;
	std::shared_ptr<mat4f> Scale;
	std::shared_ptr<mat4f> Translation;
	std::shared_ptr<mat4f> Rotate;
	std::shared_ptr<mat4f> mRelativeWorld;
	
	D3DApp* Mgr;
};