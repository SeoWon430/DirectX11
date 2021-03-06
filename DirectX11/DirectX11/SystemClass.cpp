
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




// 초기화
// 윈도우에 적용할 기능(입력, 그래픽 등을 설정하고 관리)
bool SystemClass::Initialize() {

	// 윈도우 창 가로, 세로 넓이 변수 초기화
	int screenWidth = 0;
	int screenHeight = 0;


	// 윈도우 생성 초기화
	InitializeWindows(screenWidth, screenHeight);


	// m_Input 객체 생성
	// 사용자의 키보드 입력에 사용
	m_Input = new InputClass;
	if (!m_Input) {
		return false;
	}
	// m_Input 객체 초기화
	if (!m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight))
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}


	// m_Graphics 객체 생성
	// 그래픽 랜더링을 처리
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// m_Graphics 객체 초기화.
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



	// fps 객체를 만듭니다.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// fps 객체를 초기화합니다.
	m_Fps->Initialize();

	// cpu 객체를 만듭니다.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// cpu 객체를 초기화합니다.
	m_Cpu->Initialize();

	// 타이머 객체를 만듭니다.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// 타이머 객체를 초기화합니다.
	if (!m_Timer->Initialize())
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}


	return true;
}





// 종료
// 생성한 객체 반대 순서로 해제
// 마지막으로 왼도우를 닫음
void SystemClass::Shutdown() {


	// 타이머 객체를 해제합니다.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// cpu 객체를 해제합니다.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// fps 객체를 해제합니다.
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




// 동작
// 윈도우가 켜져 있는 동안 동작할 기능들을 수행
void SystemClass::Run() {

	// 메시지 구조체 생성 및 초기화
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// 사용자로부터 종료 메시지를 받을때까지 루프 실행
	while (true) {
		// 윈도우 메시지를 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// 종료 메시지를 받을 경우 메시지 루프를 탈출합니다
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// 그 외에는 Frame 함수를 처리
			if (!Frame())
			{
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				break;
			}
		}


		// 사용자가 ESC키를 눌렀는지 확인 후 종료 처리함
		if (m_Input->IsEscapePressed() == true)
		{
			break;
		}

	}
}


// 프레임
// 매 프레임 실행 할 기능
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
	// ESC 키 감지
	// 감지하면 종료
	if (m_Input->IsKeyDown(VK_ESCAPE)) {
		return false;
	}
	*/
	// 그래픽 객체의 Frame을 실행



	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// 입력 프레임 처리를 수행합니다
	if (!m_Input->Frame())
	{
		return false;
	}

	// 그래픽 객체에 대한 프레임 처리를 수행합니다.
	if (!m_Graphics->Frame(mouseX, mouseY, m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Timer->GetTime()))
	{
		return false;
	}

	return true;
}


// 윈도우 초기화(생성)
// 윈도우 설정과 포인터 설정
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight) {

	// 외부 포인터를 설정
	ApplicationHandle = this;

	// 현재 프로그램의 인스턴스를 설정
	m_hinstance = GetModuleHandle(NULL);

	// 프로그램 이름을 설정
	m_applicationName = L"Test";

	// windows 클래스를 아래와 같이 설정
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

	// windows class를 등록
	RegisterClassEx(&wc);

	// 모니터 화면의 해상도를 읽음
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX = 0;
	int posY = 0;

	// FULL_SCREEN 변수 값에 따라 화면을 설정
	if (FULL_SCREEN) {
		// 풀스크린 모드 : 해상도를 모니터 해상도로 지정하고 색상을 32bit로 지정
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 풀스크린으로 디스플레이를 설정
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else {
		// 윈도우 모드의 경우 800 * 600 해상도로 지정
		screenWidth = 800;
		screenHeight = 600;

		// 윈도우 창을 가로, 세로의 정 가운데 오도록 조정함
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// 윈도우를 생성하고 핸들 설정
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// 윈도우를 화면에 표시하고 포커스를 지정
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
}





// 윈도우 닫기
void SystemClass::ShutdownWindows() {

	// 풀스크린 모드였다면 디스플레이 설정을 초기화
	if (FULL_SCREEN) {
		ChangeDisplaySettings(NULL, 0);
	}

	// 창을 닫음
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 프로그램 인스턴스를 제거
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// 외부포인터 참조를 초기화
	ApplicationHandle = NULL;
}





// 윈도우에 대한 반응 처리
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {

	switch (umessage) {

		// 윈도우 종료
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}

		// 윈도우가 닫힐 때
		case WM_CLOSE: {
			PostQuitMessage(0);
			return 0;
		}

		// 그 외
		default: {
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}





// 메세지 처리
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {

	switch (umsg) {

		// 키보드가 눌러졌는가 처리
	case WM_KEYDOWN: {
		PostQuitMessage(0);
		// 키 눌림 flag를 m_Input 객체에 처리
		//m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}

				   // 키보드가 떨어졌는가 처리
	case WM_KEYUP: {

		PostQuitMessage(0);

		// 키 해제 flag를 m_Input 객체에 처리
		return 0;
	}

				 // 그 외의 모든 메시지들은 기본 메시지 처리로 넘깁니다.
	default: {
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}