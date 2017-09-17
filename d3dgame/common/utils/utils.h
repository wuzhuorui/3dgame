#pragma once
#include<memory>

class CommonUtils
{
public:
	static std::shared_ptr<CommonUtils> Instance();
	template<typename T>
	void Release(T* ptr)
	{
		ptr->Release();
	}
	virtual  ~CommonUtils();
private:
	CommonUtils();
};

extern std::shared_ptr<CommonUtils> GlobalUtils;
