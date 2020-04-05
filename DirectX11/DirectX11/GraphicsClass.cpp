
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
	char fileName[] = "../DirectX11/Data/stone01.tga";
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), fileName)) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}



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


	return true;
}


// �׷��� Ŭ���� ����
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



// �� ������ ������ ����
bool GraphicsClass::Frame() {
	return Render();
}


// ������
bool GraphicsClass::Render() {

	// ���� �׸��� ���� ���۸� �ʱ�ȭ
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶��� ��ġ�� ����� �� ����� ����
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� ������
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// ��(������Ʈ)�� ������ ���� ���۸� �׷��� ������ ���ο� ��ġ
	m_Model->Render(m_Direct3D->GetDeviceContext());


	// �ؽ��� ���̴��� ����Ͽ� ���� �������մϴ�.
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture())) {
		return false;
	}
	/*
	// ���� ���̴��� ����Ͽ� ���� ������
	if (!m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix)) {
		return false;
	}
	*/

	// ������ ������ ȭ�鿡 ���
	m_Direct3D->EndScene();

	return true;
}