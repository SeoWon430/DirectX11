
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





// �ʱ�ȭ
bool TimerClass::Initialize() {

	// �� �ý����� ���� Ÿ�̸� ������ �ϴ��� Ȯ��
	// ���� Ÿ�̸Ӹ� �����ϸ� m_frequency �� ���� ��
	// m_frequency : �ʴ� Ÿ�̸Ӹ� üũ �ϴ� Ƚ��
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0) {
		return false;
	}

	// ����ũ�μ�ũ �� ƽ �� ���
	m_ticksPerMs = (float)(m_frequency / 1000);

	// ���� �ð� ���
	// QueryPerformanceCounter : ���� �ý����� ���� �ð��� �Ѱ���
	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}





// ������
void TimerClass::Frame() {

	// �ӽ� ���� �ð�
	INT64 currentTime = 0;

	// �� �ý����� ���� �ð��� �ӽ� ���� �ð��� ���
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	// �ð����� ���
	float timeDifference = (float)(currentTime - m_startTime);

	// ���α׷� ���� �ð� ���
	m_frameTime = timeDifference / m_ticksPerMs;

	// ���� �ð��� �缳��
	m_startTime = currentTime;
}





// ���� ���α׷��� ���� �ð��� ����
// ���α׷��� ���� �� �� �󸶳� ���������� ���� �ð�
float TimerClass::GetTime() {
	return m_frameTime;
}