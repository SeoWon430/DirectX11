#pragma once

#include "stdafx.h"

#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "BitmapClass.h"
#include "TextClass.h"

//#include "ColorShaderClass.h"
#include "TextureShaderClass.h"

#include "Lightshaderclass.h"
#include "Lightclass.h"

// 전체 화면
const bool FULL_SCREEN = false;

/*	★ 수직 동기화 (Vertical Synchronization )
		- 매 프레임 마다 화면에 표시할 렌더 이미지를 그래픽카드가 계산하여 모니터로 전달
		- 그래픽 카드가 이미지를 전달하는 속도(수직 주파수, 수직 주사율)와 모니터가 받는 속도(주사율)의 차이로 인해 이미지 깨짐이 가능(테어링 현상)
		- 수직 동기화 버퍼를 두어 그래픽 카드와 모니터의 주사율(타이밍)을 맞춤(낮은 쪽으로 맞춤) 
		- 모니터의 주사율은 고정(60, 144)이지만, 그래픽카드는 연산에 따라 달라짐(상황에 따라 급격한 프레임 저하 가능)
		- 보통 그래픽 카드의 주사율이 높아 수직 동기화로 낮아진 만큼 쉬는 시간이 많아 지지만, 수직 동기화 버퍼를 거치면서 지연 시간이 발생 가능

		수직 주파수 : 초당 화면을 그리는 횟수 (60Hz : 초당 60번을 그림)
		수평 주파수 : 초당 수평으로 점을 찍는 횟수 (80kHz : 초당 80000개 점을 찍음)
			화면에 영항을 크게 주는건 수직 주파수이며, 주로 수평 주파수보다 수직 주파수를 설정함
		

	★ 테어링 현상 (Tearing)
		- 디스플레이 장치는 대부분 기본적으로 더블 버퍼 방식으로 처리함
			프론트 버퍼(front) : 현재 프레임에 해당하는 화면을 출력
			백 버퍼(back) : 다음 프레임에 해당하는 화면을 그려 놓음
			프레임 마다 백 버퍼와 프론트 버퍼의 화면을 교체(switch)하며 디스플레이에 출력
		- 위 방식에서 백 버퍼에 화면을 그릴 때 화면 교체가 되면 이전 프레임과 현재 프레임의 화면이 섞이는 현상이 발생 <- 테어링 현상
		- 테어링 현상을 방지하기 위해 수직동기화를 사용하거나, 이중 또는 삼중 버퍼를 사용

	# https://namu.wiki/w/%EC%88%98%EC%A7%81%EB%8F%99%EA%B8%B0%ED%99%94
*/
const bool VSYNC_ENABLED = true;

// 카메라의 최대 시야거리
// Frustum에서의 far값
const float SCREEN_DEPTH = 1000.0f;

// 카메라의 최소 시야 거리
// Frustum에서의 near값
const float SCREEN_NEAR = 0.1f;


// DirectX를 사용하여 윈도우에 렌더링 하는 클래스
class GraphicsClass {
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	bool Frame(int, int);
	bool Frame(int, int, int, int, float);

private:
	//bool Render();
	bool Render(float);

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;

	// 색상 셰이더(Color.vs와 Color.ps)를 사용
	//ColorShaderClass* m_ColorShader = nullptr;

	// 텍스쳐 셰이더(Texture.vs와 Texture.ps)를 사용
	//TextureShaderClass* m_TextureShader = nullptr;
	
	// 조명 셰이더(Light.vs와 Light.ps)를 사용
	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	// 2D이미지
	BitmapClass* m_Bitmap;
	// 2D에 적용할 텍스쳐 셰이더
	TextureShaderClass* m_TextureShader;


	// 텍스트
	TextClass* m_Text;
};