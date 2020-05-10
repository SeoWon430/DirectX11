#pragma once

#include "stdafx.h"


// 타이머 클래스
// 시간을 잼
class TimerClass {
public:
	TimerClass();
	TimerClass(const TimerClass&);
	~TimerClass();

	bool Initialize();
	void Frame();

	float GetTime();

private:

	// 빈도수
	INT64 m_frequency;

	// ms당 틱수
	// CPU의 연산 능력(?)
	float m_ticksPerMs;

	// 시작시간
	// 프로그램이 시작한 시간
	INT64 m_startTime;

	// 현재 프레임이 가진 시간
	float m_frameTime;
};