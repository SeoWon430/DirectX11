
#include "GraphicsClass.h"



GraphicsClass::GraphicsClass() {
	m_Direct3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;

	m_LightShader = nullptr;
	m_Light = nullptr;

	m_Bitmap = nullptr;
	m_TextureShader = nullptr;
	m_Text = nullptr;

}


GraphicsClass::GraphicsClass(const GraphicsClass& other) {

}


GraphicsClass::~GraphicsClass() {

}





// 초기화
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
	XMMATRIX baseViewMatrix;
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);




	// m_Model 객체 생성
	m_Model = new ModelClass;
	if (!m_Model) {
		return false;
	}
	// m_Model 객체 초기화
	//char fileName[] = "../DirectX11/Data/stone01.tga";
	char modelFile[] = "../DirectX11/Data/cube.txt";
	char texFile[] = "../DirectX11/Data/stone01.tga";
	//char fileName[] = "../DirectX11/Data/seafloor.dds";
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFile, texFile)) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}



	// m_LightShader 객체 생성
	m_LightShader = new LightShaderClass;
	if (!m_LightShader) {
		return false;
	}

	// m_LightShader 객체 초기화
	if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// m_Light 객체 생성
	m_Light = new LightClass;
	if (!m_Light) {
		return false;
	}

	// m_Light 객체 초기화
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);



	/*
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
	*/

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



	// m_TextureShader 객체 생성
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader) {
		return false;
	}

	// m_TextureShader 객체 초기화
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the Texture shader object.", L"Error", MB_OK);
		return false;
	}

	// 비트맵 객체 생성
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap) {
		return false;
	}

	// 비트맵 객체 초기화
	//char texFile2[] = "../DirectX11/Data/stone01.tga";
	char texFile2[] = "../DirectX11/Data/test.tga";
	if (!m_Bitmap->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, texFile2, 256, 256)) {
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}



	/*

	m_Text = new TextClass;
	if (!m_Text) {
		return false;
	}

	char fontFile[] = "";
	char text[] = "Hello";
	if (!m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), fontFile, text)) {
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	*/



	// m_Text 객체 생성
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// m_Text 객체 초기화
	if (!m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix))
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


// 그래픽 클래스 종료
void GraphicsClass::Shutdown() {

	if (m_Text) {
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	if (m_Bitmap) {
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	if (m_Light) {
		delete m_Light;
		m_Light = 0;
	}

	if (m_LightShader) {
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	/*

	if (m_TextureShader) {
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
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



// 프레임
bool GraphicsClass::Frame() {
	static float rotation = 0.0f;
	//m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// 각 프레임의 rotation 변수를 업데이트
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f) {
		rotation -= 360.0f;
	}

	// 그래픽 랜더링 처리
	return Render(rotation);
}

// 프레임
// 회전 추가
bool GraphicsClass::Frame(int x, int y) {
	static float rotation = 0.0f;
	//m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	x -= 300;
	
	// 각 프레임의 rotation 변수를 업데이트
	rotation += (float)XM_PI * 0.0001f * x;
	if (rotation > 360.0f) {
		rotation -= 360.0f;
	}

	// 그래픽 랜더링 처리
	return Render(rotation);
}

// 프레임
// 회전 + 타이머
bool GraphicsClass::Frame(int x, int y, int fps, int cpu, float frameTime) {

	// 초당 프레임 수를 설정
	if (!m_Text->SetFps(fps, m_Direct3D->GetDeviceContext())) {
		return false;
	}

	// cpu 사용을 설정
	if (!m_Text->SetCpu(cpu, m_Direct3D->GetDeviceContext())) {
		return false;
	}


	static float rotation = 0.0f;
	//m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	x -= 300;

	// 각 프레임의 rotation 변수를 업데이트
	rotation += (float)XM_PI * 0.0001f * x;
	if (rotation > 360.0f) {
		rotation -= 360.0f;
	}

	return Render(rotation);
}





// 렌더링
bool GraphicsClass::Render(float rotation=0) {

	// 씬을 그리기 위해 버퍼를 초기화
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치를 고려한 뷰 행렬을 생성
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옴
	XMMATRIX worldRotateMatrix, worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	worldRotateMatrix = worldMatrix;



	// 3D모델을 회전하기 위해 행렬화
	worldRotateMatrix = XMMatrixRotationY(rotation)* XMMatrixRotationX(rotation/2);


	// 모델(오브젝트)의 정점과 색인 버퍼를 그래픽 파이프 라인에 배치
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// 조명 셰이더를 사용하여 모델을 렌더링
	if (!m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldRotateMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), m_Camera->GetPosition())) {
		return false;
	}

	/*
	// 텍스쳐 쉐이더를 사용하여 모델을 렌더링
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture())) {
		return false;
	}
	
	// 색상 쉐이더를 사용하여 모델을 렌더링
	if (!m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix)) {
		return false;
	}
	*/






	// 모든 2D 렌더링 시작 : Z버퍼 끔
	m_Direct3D->TurnZBufferOff();

	// 비트 맵을 그래픽 파이프 라인에 배치하여 렌더링
	if (!m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 500, 300)) {
		return false;
	}

	// 텍스처 쉐이더로 비트 맵을 렌더링
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture())) {
		return false;
	}



	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	// Render the text strings.
	if (!m_Text->Render(m_Direct3D->GetDeviceContext(), worldMatrix, orthoMatrix))
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_Direct3D->TurnOffAlphaBlending();

	// 모든 2D 렌더링 완료 : Z버퍼 킴
	m_Direct3D->TurnZBufferOn();


	// 버퍼의 내용을 화면에 출력
	m_Direct3D->EndScene();

	return true;
}