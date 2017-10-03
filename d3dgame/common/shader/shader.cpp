#include"shader.h"
#include<fstream>
using std::ifstream;
#include<vector>
using std::vector;
#include<d3d11.h>
#include<D3DX11async.h>
#include"../../common/lib/d3dx11effect.h"
#include"../../common/utils/utils.h"

#include<cassert>


Shader::Shader(const std::wstring& path):mFX(nullptr),mPath(path)
{
}

Shader::~Shader()
{
	GlobalUtils->Release(mFX);
}

bool Shader::InitFX(ID3D11Device* md3dDevice)
{
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT hr = D3DX11CompileFromFile(mPath.c_str(), 0, 0, 0, "fx_5_0", shaderFlags,
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
	return true;
}
