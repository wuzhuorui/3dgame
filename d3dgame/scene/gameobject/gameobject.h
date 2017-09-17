#pragma
#include<vector>
#include<d3d11.h>
#include"../../common/math/transform.h"
#include"../../common/math/vec4.h"
#include"../../common/math/vec3.h"
#include"../../common/math/vec2.h"
#include<memory>

class D3DApp;

class GameObject
{
public:
	GameObject(D3DApp* mgr);
	virtual ~GameObject();
	void InitVB(size_t,size_t,const void*);
	void InitIB(size_t,size_t, const void*);
	
	void SetScale(float x,float y,float z);
	void SetScale(std::shared_ptr<mat4f>& scale);
	void ClearScale();

	void SetTranslation(float x, float y, float z);
	void SetTranslation(std::shared_ptr<mat4f>& translation);
	void ClearTranslation();

	void SetRotate();
	void ClearRotate();

	std::shared_ptr<mat4f> GetWorldMat();

	void DrawScene(const mat4f& VP,const mat4f& ParentRelativeWorld);

private:
	enum  status {
		mVBOK = 0x1, mIBOK = 0x2, mScaleMat = 0x4, mTranslation = 0x8, mRotate = 0x10,
		mRelativeWorldDirty = 0x20
	};
	std::vector<GameObject*> mChildren;
	
	ID3D11Buffer* mVB;
	size_t VertexNum;
	size_t VertexSize;
	ID3D11Buffer* mIB;
	size_t IndexNum;
	size_t IndexSize;

	unsigned mFlag;
	std::shared_ptr<mat4f> Scale;
	std::shared_ptr<mat4f> Translation;
	std::shared_ptr<mat4f> Rotate;
	std::shared_ptr<mat4f> mRelativeWorld;
	
	D3DApp* Mgr;
};