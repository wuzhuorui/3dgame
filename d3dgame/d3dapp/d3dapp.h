#pragma once
#include<Windowsx.h>
#include<d3d11.h>
#include<D3DX11async.h>
#include<string>
#include<memory>
#include"../common/debug/debug.h"
#include"../common/utils/utils.h"
#include"../scene/gameobject/gameobject.h"

class GameTimer;

class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);
	virtual		~D3DApp();
	HINSTANCE	AppInst()const;
	HWND		MainWnd()const;
	float		AspectRatio()const;
	int			Run();

	virtual bool Init();
	virtual void OnResize();
	virtual void UpdateScene(float dt) = 0;
	virtual void DrawScene() = 0;
	virtual LRESULT MsgProc (HWND hwnd, UINT msg,WPARAM wParam,LPARAM lParam);
	
	virtual void OnMouseDown(WPARAM btnState,int x,int y){};
	virtual void OnMouseUp(WPARAM btnState,int x,int y){};
	virtual void OnMouseMove(WPARAM btnState,int x,int y){};

	ID3D11Device* Getmd3dDevice() { return md3dDevice; };
	ID3D11DeviceContext* Getmd3dImmediateContext() { return md3dImmediateContext; };

protected:
	bool InitMainWindow();
	bool IniDirect3D();
	void CalculateFrameStats();
protected:
	HINSTANCE mhAppInst;
	HWND	  mhMainWnd;
	bool 	  mAppPaused;
	bool 	  mMinimized;
	bool 	  mMaximized;
	bool	  mResizing;
	UINT	  m4xMsaaQuality;
	std::vector<std::shared_ptr<class Shader>> mShader;
	std::shared_ptr<GameTimer> mTimer;
	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mScreenViewport;
	
	std::wstring mMainWndCaption;
	D3D_DRIVER_TYPE md3dDriverType;
	int mClientWidth;
	int mClientHeight;
	bool mEnable4xMsaa;

	std::shared_ptr<class GameObject> root;

};