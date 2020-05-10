
#include "SystemClass.h"


SystemClass::SystemClass() {
	m_applicationName = nullptr;
	m_hinstance = nullptr;
	m_hwnd = nullptr;

	m_Input = nullptr;
	m_Graphics = nullptr;
	m_Sound = nullptr;

	m_Fps = nullptr;
	m_Cpu = nullptr;
	m_Timer = nullptr;
}


SystemClass::SystemClass(const SystemClass& other) {
}


SystemClass::~SystemClass() {
}




// �ʱ�ȭ
// �����쿡 ������ ���(�Է�, �׷��� ���� �����ϰ� ����)
bool SystemClass::Initialize() {

	// ������ â ����, ���� ���� ���� �ʱ�ȭ
	int screenWidth = 0;
	int screenHeight = 0;


	// ������ ���� �ʱ�ȭ
	InitializeWindows(screenWidth, screenHeight);


	// m_Input ��ü ����
	// ������� Ű���� �Է¿� ���
	m_Input = new InputClass;
	if (!m_Input) {
		return false;
	}
	// m_Input ��ü �ʱ�ȭ
	if (!m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight))
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}


	// m_Graphics ��ü ����
	// �׷��� �������� ó��
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// m_Graphics ��ü �ʱ�ȭ.
	if (!m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd))
	{
		return false;
	}



	// Create the sound object.
	m_Sound = new SoundClass;
	if (!m_Sound)
	{
		return false;
	}

	// Initialize the sound object.
	if (!m_Sound->Initialize(m_hwnd))
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}



	// fps ��ü�� ����ϴ�.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// fps ��ü�� �ʱ�ȭ�մϴ�.
	m_Fps->Initialize();

	// cpu ��ü�� ����ϴ�.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// cpu ��ü�� �ʱ�ȭ�մϴ�.
	m_Cpu->Initialize();

	// Ÿ�̸� ��ü�� ����ϴ�.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Ÿ�̸� ��ü�� �ʱ�ȭ�մϴ�.
	if (!m_Timer->Initialize())
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}


	return true;
}





// ����
// ������ ��ü �ݴ� ������ ����
// ���������� �޵��츦 ����
void SystemClass::Shutdown() {


	// Ÿ�̸� ��ü�� �����մϴ�.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// cpu ��ü�� �����մϴ�.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// fps ��ü�� �����մϴ�.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	if (m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}

	if (m_Graphics) {
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	if (m_Input) {
		delete m_Input;
		m_Input = 0;
	}

	ShutdownWindows();
}




// ����
// �����찡 ���� �ִ� ���� ������ ��ɵ��� ����
void SystemClass::Run() {

	// �޽��� ����ü ���� �� �ʱ�ȭ
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// ����ڷκ��� ���� �޽����� ���������� ���� ����
	while (true) {
		// ������ �޽����� ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// ���� �޽����� ���� ��� �޽��� ������ Ż���մϴ�
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// �� �ܿ��� Frame �Լ��� ó��
			if (!Frame())
			{
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				break;
			}
		}


		// ����ڰ� ESCŰ�� �������� Ȯ�� �� ���� ó����
		if (m_Input->IsEscapePressed() == true)
		{
			break;
		}

	}
}


// ������
// �� ������ ���� �� ���
bool SystemClass::Frame() {

	int mouseX = 0 , mouseY = 0;

	if (!m_Input->Frame())
	{
		return false;
	}

	m_Input->GetMouseLocation(mouseX, mouseY);
	
	/*
	if (!m_Graphics->Frame(mouseX, mouseY)) {
		return false;
	}
	*/
	/*
	// ESC Ű ����
	// �����ϸ� ����
	if (m_Input->IsKeyDown(VK_ESCAPE)) {
		return false;
	}
	*/
	// �׷��� ��ü�� Frame�� ����



	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// �Է� ������ ó���� �����մϴ�
	if (!m_Input->Frame())
	{
		return false;
	}

	// �׷��� ��ü�� ���� ������ ó���� �����մϴ�.
	if (!m_Graphics->Frame(mouseX, mouseY, m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Timer->GetTime()))
	{
		return false;
	}

	return true;
}


// ������ �ʱ�ȭ(����)
// ������ ������ ������ ����
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight) {

	// �ܺ� �����͸� ����
	ApplicationHandle = this;

	// ���� ���α׷��� �ν��Ͻ��� ����
	m_hinstance = GetModuleHandle(NULL);

	// ���α׷� �̸��� ����
	m_applicationName = L"Test";

	// windows Ŭ������ �Ʒ��� ���� ����
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// windows class�� ���
	RegisterClassEx(&wc);

	// ����� ȭ���� �ػ󵵸� ����
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX = 0;
	int posY = 0;

	// FULL_SCREEN ���� ���� ���� ȭ���� ����
	if (FULL_SCREEN) {
		// Ǯ��ũ�� ��� : �ػ󵵸� ����� �ػ󵵷� �����ϰ� ������ 32bit�� ����
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Ǯ��ũ������ ���÷��̸� ����
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else {
		// ������ ����� ��� 800 * 600 �ػ󵵷� ����
		screenWidth = 800;
		screenHeight = 600;

		// ������ â�� ����, ������ �� ��� ������ ������
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// �����츦 �����ϰ� �ڵ� ����
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� ����
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
}





// ������ �ݱ�
void SystemClass::ShutdownWindows() {

	// Ǯ��ũ�� ��忴�ٸ� ���÷��� ������ �ʱ�ȭ
	if (FULL_SCREEN) {
		ChangeDisplaySettings(NULL, 0);
	}

	// â�� ����
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ���α׷� �ν��Ͻ��� ����
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �ܺ������� ������ �ʱ�ȭ
	ApplicationHandle = NULL;
}





// �����쿡 ���� ���� ó��
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {

	switch (umessage) {

		// ������ ����
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}

		// �����찡 ���� ��
		case WM_CLOSE: {
			PostQuitMessage(0);
			return 0;
		}

		// �� ��
		default: {
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}





// �޼��� ó��
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {

	switch (umsg) {

		// Ű���尡 �������°� ó��
	case WM_KEYDOWN: {
		PostQuitMessage(0);
		// Ű ���� flag�� m_Input ��ü�� ó��
		//m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}

				   // Ű���尡 �������°� ó��
	case WM_KEYUP: {

		PostQuitMessage(0);

		// Ű ���� flag�� m_Input ��ü�� ó��
		return 0;
	}

				 // �� ���� ��� �޽������� �⺻ �޽��� ó���� �ѱ�ϴ�.
	default: {
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}