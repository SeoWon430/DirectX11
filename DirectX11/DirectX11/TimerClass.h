#pragma once

#include "stdafx.h"


// Ÿ�̸� Ŭ����
// �ð��� ��
class TimerClass {
public:
	TimerClass();
	TimerClass(const TimerClass&);
	~TimerClass();

	bool Initialize();
	void Frame();

	float GetTime();

private:

	// �󵵼�
	INT64 m_frequency;

	// ms�� ƽ��
	// CPU�� ���� �ɷ�(?)
	float m_ticksPerMs;

	// ���۽ð�
	// ���α׷��� ������ �ð�
	INT64 m_startTime;

	// ���� �������� ���� �ð�
	float m_frameTime;
};