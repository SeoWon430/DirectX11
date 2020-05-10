#include "stdafx.h"
#include "CpuClass.h"


CpuClass::CpuClass() {
	m_canReadCpu = true;
	m_queryHandle;
	m_counterHandle;
	m_lastSampleTime = 0;
	m_cpuUsage = 0;
}


CpuClass::CpuClass(const CpuClass& other) {
}


CpuClass::~CpuClass() {
}




// 초기화
void CpuClass::Initialize() {

	// CPU 사용을 폴링하는 쿼리 개체를 생성
	// PHD(performancer data helper) : 현 시스템에 대한 성능을 얻어옴
	// 쿼리(Query) : 데이터베이스(DataBase)에 대해 특정 정보를 요청
	// 폴링(Polling) : 여러 장치에 대해 충돌 회피, 동기화 처리를 목적으로 상태를 주기적으로 체크하여 데이터를 송수신하는 처리 방식
	PDH_STATUS status = PdhOpenQuery(NULL, 0, &m_queryHandle);
	if (status != ERROR_SUCCESS) {
		m_canReadCpu = false;
	}

	// 시스템의 모든 CPU를 폴링하도록 쿼리 개체를 설정
	status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
	if (status != ERROR_SUCCESS) {
		m_canReadCpu = false;
	}

	m_lastSampleTime = GetTickCount();

	m_cpuUsage = 0;
}





// 해제
void CpuClass::Shutdown() {
	if (m_canReadCpu) {
		PdhCloseQuery(m_queryHandle);
	}
}





// 프레임
void CpuClass::Frame() {

	PDH_FMT_COUNTERVALUE value;

	if (m_canReadCpu) {
		if ((m_lastSampleTime + 1000) < GetTickCount()) {
			m_lastSampleTime = GetTickCount();

			PdhCollectQueryData(m_queryHandle);

			PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

			m_cpuUsage = value.longValue;
		}
	}
}





// 현재 점유율
int CpuClass::GetCpuPercentage() {
	int usage = 0;

	if (m_canReadCpu) {
		usage = (int)m_cpuUsage;
	}

	return usage;
}