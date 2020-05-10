#pragma once

#include "stdafx.h"


// DirectX 실행 관련 클래스
class D3DClass {
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

private:

	// 수직 동기화
	bool m_vsync_enabled = false;

	// 그래픽카드 메모리
	int m_videoCardMemory = 0;

	//
	char m_videoCardDescription[128] = { 0, };

	/* ★ 교환 사슬 (Swap Chain)
		- 디스플레이에 그리기 위한 버퍼를 처리 할 때, 다중 버퍼링으로서 후면과 전면을 계속 바꿔가며 그리는 방식
		IDXGISwapChain : 교환 사슬에 대한 인터페이스
			- 버퍼에 접근, 버퍼의 크기 또는 갯수 등을 조정, 출력시 크기 조정 등
	*/
	IDXGISwapChain* m_swapChain = nullptr;

	// 현재의 장치(그래픽카드)
	ID3D11Device* m_device = nullptr;

	// 장치에 전달할 명령을 전달
	ID3D11DeviceContext* m_deviceContext = nullptr;

	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	ID3D11Texture2D* m_depthStencilBuffer = nullptr;
	ID3D11DepthStencilState* m_depthStencilState = nullptr;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11RasterizerState* m_rasterState = nullptr;

	// 투영 행렬
	XMMATRIX m_projectionMatrix;

	// 월드 행렬
	XMMATRIX m_worldMatrix;

	// 직교 행렬
	XMMATRIX m_orthoMatrix;

	// 깊이 스텐실
	ID3D11DepthStencilState* m_depthDisabledStencilState = nullptr;

	// 알파 블랜딩 상태
	ID3D11BlendState* m_alphaEnableBlendingState = nullptr;
	ID3D11BlendState* m_alphaDisableBlendingState = nullptr;
};