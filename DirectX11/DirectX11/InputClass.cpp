
#include "InputClass.h"



InputClass::InputClass() {
}


InputClass::InputClass(const InputClass& other) {
}


InputClass::~InputClass() {
}





// �Է� ��ü �ʱ�ȭ
void InputClass::Initialize() {
	// Ű �迭�� �ʱ�ȭ�մϴ�
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}
}


// Ű�� ������ ��
void InputClass::KeyDown(unsigned int input) {
	m_keys[input] = true;
}


// Ű�� ���� ��
void InputClass::KeyUp(unsigned int input) {
	m_keys[input] = false;
}


// �ش� Ű�� ���� �ִ��� Ȯ��
bool InputClass::IsKeyDown(unsigned int key) {
	return m_keys[key];
}