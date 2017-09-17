//***************************************************************************************
// Init Direct3D.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates the sample framework by initializing Direct3D, clearing 
// the screen, and displaying frame stats.
//
//***************************************************************************************

#include "d3dapp/d3dApp.h"
#include "common\debug\debug.h"
#include<cassert>
//#include"boxapp\boxapp.h"
#include"hillsapp\hillsapp.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	mat4<double> p(Vec4<double>(1, 0, 0, 0), Vec4<double>(0, 1, 0, 0), Vec4<double>(0, 0, 1, 0), Vec4<double>(3, 4, 5, 1));
	Vec4d x(3, 4, 5, 0);
	HillsApp theApp(hInstance);
	//BoxApp theApp(hInstance);
	if( !theApp.Init() )
		return 0;
	
	return theApp.Run();
}
