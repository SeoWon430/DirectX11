
#include "GraphicsClass.h"



GraphicsClass::GraphicsClass() {

}


GraphicsClass::GraphicsClass(const GraphicsClass& other) {

}


GraphicsClass::~GraphicsClass() {

}





// �׷��� Ŭ���� �ʱ�ȭ
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {

	// Direct3D ��ü ����
	m_Direct3D = new D3DClass;
	if (!m_Direct3D) {
		return false;
	}

	// Direct3D ��ü �ʱ�ȭ
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR)) {
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}



	// m_Camera ��ü ����
	m_Camera = new CameraClass;
	if (!m_Camera) {
		return false;
	}
	// ī�޶� ������ ����
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);



	// m_Model ��ü ����
	m_Model = new ModelClass;
	if (!m_Model) {
		return false;
	}
	// m_Model ��ü �ʱ�ȭ
	//char fileName[] = "../DirectX11/Data/stone01.tga";
	char modelFile[] = "../DirectX11/Data/cube.txt";
	char texFile[] = "../DirectX11/Data/stone01.tga";
	//char fileName[] = "../DirectX11/Data/seafloor.dds";
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFile, texFile)) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}



	// m_LightShader ��ü ����
	m_LightShader = new LightShaderClass;
	if (!m_LightShader) {
		return false;
	}

	// m_LightShader ��ü �ʱ�ȭ
	if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// m_Light ��ü ����
	m_Light = new LightClass;
	if (!m_Light) {
		return false;
	}

	// m_Light ��ü �ʱ�ȭ
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);



	/*
	// m_TextureShader ��ü ����
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader) {
		return false;
	}

	// m_TextureShader ��ü �ʱ�ȭ
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}
	*/

	/*
	// m_ColorShader ��ü ����
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader) {
		return false;
	}
	// m_ColorShader ��ü �ʱ�ȭ
	if (!m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}
	*/



	// m_TextureShader ��ü ����
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader) {
		return false;
	}

	// m_TextureShader ��ü �ʱ�ȭ
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the Texture shader object.", L"Error", MB_OK);
		return false;
	}

	// ��Ʈ�� ��ü ����
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap) {
		return false;
	}

	// ��Ʈ�� ��ü �ʱ�ȭ
	//char texFile2[] = "../DirectX11/Data/stone01.tga";
	char texFile2[] = "../DirectX11/Data/test.tga";
	if (!m_Bitmap->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, texFile2, 256, 256)) {
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}


	return true;
}


// �׷��� Ŭ���� ����
void GraphicsClass::Shutdown() {


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



// �� ������ ������ ����
bool GraphicsClass::Frame() {
	static float rotation = 0.0f;

	// �� �������� rotation ������ ������Ʈ
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f) {
		rotation -= 360.0f;
	}

	// �׷��� ������ ó��
	return Render(rotation);
}


// ������
bool GraphicsClass::Render(float rotation) {

	// ���� �׸��� ���� ���۸� �ʱ�ȭ
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ����� �� ����� ����
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� ������
	XMMATRIX worldRotateMatrix, worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	worldRotateMatrix = worldMatrix;



	// 3D���� ȸ���ϱ� ���� ���ȭ
	worldRotateMatrix = XMMatrixRotationY(rotation)* XMMatrixRotationX(rotation/2);


	// ��(������Ʈ)�� ������ ���� ���۸� �׷��� ������ ���ο� ��ġ
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// ���� ���̴��� ����Ͽ� ���� ������
	if (!m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldRotateMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), m_Camera->GetPosition())) {
		return false;
	}

	/*
	// �ؽ��� ���̴��� ����Ͽ� ���� ������
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture())) {
		return false;
	}
	
	// ���� ���̴��� ����Ͽ� ���� ������
	if (!m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix)) {
		return false;
	}
	*/






	// ��� 2D ������ ���� : Z���� ��
	m_Direct3D->TurnZBufferOff();

	// ��Ʈ ���� �׷��� ������ ���ο� ��ġ�Ͽ� ������
	if (!m_Bitmap->Render(m_Direct3D->GetDeviceContext(), 100, 100)) {
		return false;
	}

	// �ؽ�ó ���̴��� ��Ʈ ���� ������
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture())) {
		return false;
	}

	// ��� 2D ������ �Ϸ� : Z���� Ŵ
	m_Direct3D->TurnZBufferOn();


	// ������ ������ ȭ�鿡 ���
	m_Direct3D->EndScene();

	return true;
}