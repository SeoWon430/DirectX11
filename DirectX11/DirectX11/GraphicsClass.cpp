
#include "GraphicsClass.h"



GraphicsClass::GraphicsClass() {

}


GraphicsClass::GraphicsClass(const GraphicsClass& other) {

}


GraphicsClass::~GraphicsClass() {

}





// �׷��� Ŭ���� �ʱ�ȭ
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
	return true;
}


// �׷��� Ŭ���� ����
void GraphicsClass::Shutdown() {

}



// �� ������ ������ ����
bool GraphicsClass::Frame() {
	return true;
}


// ���� �� ����
bool GraphicsClass::Render() {
	return true;
}