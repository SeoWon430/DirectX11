
#include "InputClass.h"



InputClass::InputClass() {
}


InputClass::InputClass(const InputClass& other) {
}


InputClass::~InputClass() {
}





// 입력 객체 초기화
void InputClass::Initialize() {
	// 키 배열을 초기화합니다
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}
}


// 키를 눌렀을 때
void InputClass::KeyDown(unsigned int input) {
	m_keys[input] = true;
}


// 키를 뗏을 때
void InputClass::KeyUp(unsigned int input) {
	m_keys[input] = false;
}


// 해당 키가 눌려 있는지 확인
bool InputClass::IsKeyDown(unsigned int key) {
	return m_keys[key];
}