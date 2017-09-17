#include"utils.h"
std::shared_ptr<CommonUtils> GlobalUtils = CommonUtils::Instance();

std::shared_ptr<CommonUtils> CommonUtils::Instance()
{
	static std::shared_ptr<CommonUtils> tmp(new CommonUtils());
	return tmp;
}

CommonUtils::CommonUtils() {};

CommonUtils::~CommonUtils() {};
