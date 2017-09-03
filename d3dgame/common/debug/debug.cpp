#include"debug.h"

std::shared_ptr<Debug> GlobalDebug = Debug::Instance();

std::shared_ptr<Debug> Debug::Instance()
{
	static std::shared_ptr<Debug> tmp(new Debug());
	return tmp;
}

Debug::Debug() {};

Debug::~Debug() {};
