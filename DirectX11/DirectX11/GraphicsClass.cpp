
#include "GraphicsClass.h"



GraphicsClass::GraphicsClass() {

}


GraphicsClass::GraphicsClass(const GraphicsClass& other) {

}


GraphicsClass::~GraphicsClass() {

}





// 그래픽 클래스 초기화
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {

	// Direct3D 객체 생성
	m_Direct3D = new D3DClass;
	if (!m_Direct3D) {
		return false;
	}

	// Direct3D 객체 초기화
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR)) {
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}



	// m_Camera 객체 생성
	m_Camera = new CameraClass;
	if (!m_Camera) {
		return false;
	}
	// 카메라 포지션 설정
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);



	// m_Model 객체 생성
	m_Model = new ModelClass;
	if (!m_Model) {
		return false;
	}
	// m_Model 객체 초기화
	char fileName[] = "../DirectX11/Data/stone01.tga";
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), fileName)) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}



	// m_TextureShader 객체 생성
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader) {
		return false;
	}

	// m_TextureShader 객체 초기화
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}


	/*
	// m_ColorShader 객체 생성
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader) {
		return false;
	}
	// m_ColorShader 객체 초기화
	if (!m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}
	*/


	return true;
}


// 그래픽 클래스 종료
void GraphicsClass::Shutdown() {

	if (m_TextureShader) {
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	/*
	if (m_ColorShader) {
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}
	*/

	if (m_Model) {
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_Camera) {
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_Direct3D) {
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}



// 매 프레임 렌더할 내용
bool GraphicsClass::Frame() {
	return Render();
}


// 렌더링
bool GraphicsClass::Render() {

	// 씬을 그리기 위해 버퍼를 초기화
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치를 고려한 뷰 행렬을 생성
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옴
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// 모델(오브젝트)의 정점과 색인 버퍼를 그래픽 파이프 라인에 배치
	m_Model->Render(m_Direct3D->GetDeviceContext());


	// 텍스쳐 쉐이더를 사용하여 모델을 렌더링합니다.
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture())) {
		return false;
	}
	/*
	// 색상 쉐이더를 사용하여 모델을 렌더링
	if (!m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix)) {
		return false;
	}
	*/

	// 버퍼의 내용을 화면에 출력
	m_Direct3D->EndScene();

	return true;
}