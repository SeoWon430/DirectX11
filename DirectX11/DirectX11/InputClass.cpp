
#include "InputClass.h"


InputClass::InputClass() {
	m_directInput = nullptr;
	m_keyboard = nullptr;
	m_mouse = nullptr;

	m_screenWidth = 0;
	m_screenHeight = 0;
	m_mouseX = 0;
	m_mouseY = 0;
}


InputClass::InputClass(const InputClass& other) {
}


InputClass::~InputClass() {
}





// 초기화
bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight) {

	// 마우스의 이동 가능 범위는 해상도 값으로 설정
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// DirectInput 인터페이스를 초기화
	HRESULT result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result)) {
		return false;
	}

	// 키보드에 대한 DirectInput 인터페이스를 생성
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result)) {
		return false;
	}

	// 키보드에 대한 형식 설정
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result)) {
		return false;
	}



	/* ★ 협력수준 설정 SetCooperativeLevel()
		: 여러 프로그램 실행 중일 때 키보드와 마우스 입력을 어떻게 처리 할지를 결정
		현재 켜진 윈도우 창에 대한 장치(마우스, 키보드)의 메세지(입력)을 처리

		◆DISCL_BACKGROUND : 백그라운드, 프로그램이 포커스가 안되어도 처리

		◆DISCL_FOREGROUND : 포그라운드, 프로그램이 포커스가 되어 있을때만 처리

		◆DISCL_EXCLUSIVE : 운영체제로부터 받은 장치 메세지를 처리하지 않음
			마우스의 경우 위치값은 처리 되지만 커서가 보이지 않음
			프로그램 종료시 마우스 커서 위치는 프로그램키기 직전 위치 그대로 있음

		◆DISCL_NONEXCLUSIVE : 운영체제로부터 받은 장치 메세지를 무시
			마우스의 경우 위치값 처리가 되며 커서도 보임
			프로그램 종료시 마우스 커서 위치는 프로그램 내에서 마지막 위치에 그대로 있음

		◆DISCL_NOWINKEY : window 로고 키를 무시
	*/

	// 키보드의 협력 수준 설정
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result)) {
		return false;
	}

	// 키보드를 할당
	// 여기서 못해도 매 프레임 할당 받음
	result = m_keyboard->Acquire();
	if (FAILED(result)) {
		return false;
	}



	// 마우스 DirectInput 인터페이스를 생성
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// 마우스의 형식 설정
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result)) {
		return false;
	}

	// 마우스의 협력 수준 설정
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result)) {
		return false;
	}

	// 마우스를 할당
	result = m_mouse->Acquire();
	if (FAILED(result)) {
		return false;
	}

	return true;
}





// 프레임
bool InputClass::Frame() {

	// 키보드의 현재 상태를 읽음
	if (!ReadKeyboard()) {
		return false;
	}

	// 마우스의 현재 상태를 읽음
	if (!ReadMouse()) {
		return false;
	}

	// 키보드와 마우스의 변경 사항을 처리
	ProcessInput();

	return true;
}


// 키보드 확인
bool InputClass::ReadKeyboard() {

	// 키보드 장치를 얻음
	HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);

	if (FAILED(result)) {
		// 키보드가 포커스를 잃었거나 할당 받지 못한 경우 재 할당 받음
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)) {
			m_keyboard->Acquire();
		}
		else {
			return false;
		}
	}

	return true;
}


// 마우스 확인
bool InputClass::ReadMouse() {

	// 마우스 장치를 얻음
	HRESULT result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);

	if (FAILED(result)) {
		// 마우스가 포커스를 잃었거나 할당 받지 못한 경우 재 할당 받음
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)) {
			m_mouse->Acquire();
		}
		else {
			return false;
		}
	}

	return true;
}


// 마우스 이동 처리
void InputClass::ProcessInput() {

	// 프레임 동안 마우스 위치의 변경을 기반으로 마우스 커서의 위치를 ​​업데이트
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// 마우스 위치가 화면 너비 또는 높이를 초과하지 않는지 확인
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
}





// 해제
void InputClass::Shutdown() {

	if (m_mouse) {
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	if (m_keyboard) {
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_directInput) {
		m_directInput->Release();
		m_directInput = 0;
	}
}





// ESC 처리
bool InputClass::IsEscapePressed() {

	// escape 키가 현재 눌려지고 있는지 bit값을 계산하여 확인한다.
	if (m_keyboardState[DIK_ESCAPE] & 0x80) {
		return true;
	}

	return false;
}



// 현재 마우스 위치 리턴
// 프로그램(윈도우) 상 위치를 리턴
void InputClass::GetMouseLocation(int& mouseX, int& mouseY) {
	mouseX = m_mouseX;
	mouseY = m_mouseY;
}


