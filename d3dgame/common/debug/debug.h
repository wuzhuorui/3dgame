#pragma once
#include<memory>
#include<ostream>
#include<iostream>

class Debug {
public:
	static std::shared_ptr<Debug> Instance();
	virtual  ~Debug();

	template<typename T>
	void loginfo(std::ostream& output, const T& out) 
	{
		output << out;
	};

private:
	Debug();
};

extern std::shared_ptr<Debug> GlobalDebug;
