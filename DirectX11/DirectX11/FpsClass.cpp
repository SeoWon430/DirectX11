#include "stdafx.h"
#include <mmsystem.h>
#include "FpsClass.h"


FpsClass::FpsClass() {
	m_fps = 0;
	m_count = 0;
	m_startTime = 0;
}


FpsClass::FpsClass(const FpsClass& other) {
}


FpsClass::~FpsClass() {
}





// 초기화
void FpsClass::Initialize() {
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
	return;
}





// 프레임
void FpsClass::Frame() {

	m_count++;

	if (timeGetTime() >= (m_startTime + 1000)) {
		m_fps = m_count;
		m_count = 0;

		m_startTime = timeGetTime();
	}
}





// 현재 fps리턴
int FpsClass::GetFps() {
	return m_fps;
}