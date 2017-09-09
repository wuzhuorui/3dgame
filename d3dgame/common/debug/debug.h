#pragma once
#include<memory>
#include<ostream>
#include<iostream>

class Debug {
public:
	static std::shared_ptr<Debug> Instance();
	virtual  ~Debug();

	template<typename T>
	void loginfo(const T& out,std::ostream& output = std::cout) 
	{
		output << out;
	};

private:
	Debug();
};

extern std::shared_ptr<Debug> GlobalDebug;
