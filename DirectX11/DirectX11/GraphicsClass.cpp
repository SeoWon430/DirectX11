
#include "GraphicsClass.h"



GraphicsClass::GraphicsClass() {

}


GraphicsClass::GraphicsClass(const GraphicsClass& other) {

}


GraphicsClass::~GraphicsClass() {

}





// 그래픽 클래스 초기화
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
	return true;
}


// 그래픽 클래스 종료
void GraphicsClass::Shutdown() {

}



// 매 프레임 렌더할 내용
bool GraphicsClass::Frame() {
	return true;
}


// 렌더 할 내용
bool GraphicsClass::Render() {
	return true;
}