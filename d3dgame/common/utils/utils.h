#pragma

namespace CommonUtils
{
	template<typename T>
	void Release(T* ptr)
	{
		ptr->Release();
	}
}