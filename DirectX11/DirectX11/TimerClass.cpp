
#include "TimerClass.h"


TimerClass::TimerClass() {
	m_frequency = 0;
	m_ticksPerMs = 0;
	m_startTime = 0;
	m_frameTime = 0;
}


TimerClass::TimerClass(const TimerClass& other) {
}


TimerClass::~TimerClass() {
}





// 초기화
bool TimerClass::Initialize() {

	// 현 시스템이 고성능 타이머 지원을 하는지 확인
	// 고성능 타이머를 지원하면 m_frequency 가 설정 됨
	// m_frequency : 초당 타이머를 체크 하는 횟수
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0) {
		return false;
	}

	// 마이크로세크 당 틱 수 계산
	m_ticksPerMs = (float)(m_frequency / 1000);

	// 현재 시간 기록
	// QueryPerformanceCounter : 현재 시스템이 가진 시간을 넘겨줌
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}





// 프레임
void TimerClass::Frame() {

	// 임시 현재 시간
	INT64 currentTime = 0;

	// 현 시스템이 가진 시간을 임시 현재 시간에 기록
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	// 시간차이 계산
	float timeDifference = (float)(currentTime - m_startTime);

	// 프로그램 실행 시간 계산
	m_frameTime = timeDifference / m_ticksPerMs;

	// 현재 시간을 재설정
	m_startTime = currentTime;
}





// 현재 프로그램이 가진 시간을 리턴
// 프로그램이 실행 된 후 얼마나 지났는지에 대한 시간
float TimerClass::GetTime() {
	return m_frameTime;
}