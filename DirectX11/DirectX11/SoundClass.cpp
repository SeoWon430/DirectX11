
#include "SoundClass.h"
#include <fstream>


SoundClass::SoundClass() {
	m_DirectSound = nullptr;
	m_primaryBuffer = nullptr;
	m_secondaryBuffer1 = nullptr;
}


SoundClass::SoundClass(const SoundClass& other) {
}


SoundClass::~SoundClass() {
}



// 초기화
bool SoundClass::Initialize(HWND hwnd) {

	/* 사운드 재생
		전면 버퍼(Primary Buffer) : 후면 버퍼의 사운드 데이터를 모두 가져와 합친 후 장치를 통해 실제 사운드를 재생
		후면 버퍼(Secondary Buffer) : 사운드 파일이 담긴 버퍼
			여러개가 존재 할수 있으며, 각 사운드 파일의 형식을 가짐
	*/

	// 사운드 초기화
	if (!InitializeDirectSound(hwnd)) {
		return false;
	}

	// 전면 버퍼에 저장할 사운드 파일 로드
	char fileName[] = "../DirectX11/Data/sound01.wav";
	if (!LoadWaveFile(fileName, &m_secondaryBuffer1)) {
		return false;
	}

	return PlayWaveFile();
}


// 사운드 초기화
bool SoundClass::InitializeDirectSound(HWND hwnd) {

	// 장치에 대한 사운드 설정
	if (FAILED(DirectSoundCreate8(NULL, &m_DirectSound, NULL))) {
		return false;
	}



	/*	사운드 협력 수준

		DSSCL_EXCLUSIVE : 포커스 된 프로그램이 사운드카드를 독점
			여러 프로그램 중 포커스 된 것에서만 재생

		DSSCL_NORMAL : 프로그램이 열려 있을 때 재생


		DSSCL_PRIORITY : 윈도우가 비활성(최소화) 상태에서도 재생
			기본적으론 DSSCL_EXCLUSIVE와 동일

		DSSCL_WRITEPRIMARY : 프로그래머가 정의
			주로 사용자 오디오 엔진을 만들 때 사용


		출처 : http://blog.naver.com/PostView.nhn?blogId=khk6435&logNo=50135221569
	*/


	// 사운드의 협력 수준 설정
	if (FAILED(m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY))) {
		return false;
	}



	// 전면 버퍼의 구조체를 채움
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL))) {
		return false;
	}

	// 후면 버퍼의 구조체를 채움
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// 전면 버퍼의 형색을 후면 버퍼를 가지고 구체적으로 설정
	if (FAILED(m_primaryBuffer->SetFormat(&waveFormat))) {
		return false;
	}

	return true;
}


// 사운드 파일 로드
// 로드 된 파일을 후면 버퍼(secondary buffer)에 저장됨
bool SoundClass::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer) {

	// 파일 열기
	FILE* filePtr = nullptr;
	int error = fopen_s(&filePtr, filename, "rb");
	if (error != 0) {
		return false;
	}

	// 파일 헤더 읽기
	WaveHeaderType waveFileHeader;
	unsigned int count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1) {
		return false;
	}

	// 파일 내 chunk ID가 RIFF 형식인지 확인
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F')) {
		return false;
	}

	// 파일 내 chunk ID가 WAVE 형식인지 확인
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E')) {
		return false;
	}

	// 파일 내 chunk ID가 fmt 형식인지 확인
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' ')) {
		return false;
	}

	// 형식이 WAVE_FORMAT_PCM 인지 확인
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM) {
		return false;
	}

	// 형식이 스테레오 인지 확인
	// 사운드 파일의 출력 채널 수 = 사운드 출력 장치의 수
	// ★ 스테레오(Stereo) : 2개의 스피커를 이용하여 입체적인 음향을 냄
	if (waveFileHeader.numChannels != 2) {
		return false;
	}



	/* ★ 샘플레이트(Sample Rate)
		오디오 추출 시 초당 추출 수
		실제 사운드(아날로그)는 연속적이지만 프로그램 상에서의 사운드(디지털)은 비연속적임
		디지털 사운드로 변환 하면서 아날로그의 사운드 중 일부를 추출하게 되는데 이를 샘플링이라 하며,
		샘플링 정도를 샘플레이트라 함
		일반적으로 44100Hz(44.1KHz)로 추출하게 됨 (초당 44100번)
	*/
	// 샘플레이트 확인 (44100Hz)
	if (waveFileHeader.sampleRate != 44100) {
		return false;
	}


	
	// 비트레이트 확인 (16bit)
	// ★ 비트레이트 (Bit Rate): 초당 전송되는 데이터의 양
	if (waveFileHeader.bitsPerSample != 16) {
		return false;
	}


	// 파일 내 철크 데이터 data 인지 확인 
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a')) {
		return false;
	}


	// 후면 버퍼의 형식 설정
	// 위에서 로드한 파일의 형식에 따라 설정
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;


	// 위 설정 한 값을 가지고 확인용 임시 버퍼 생성
	IDirectSoundBuffer* tempBuffer = nullptr;
	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL))) {
		return false;
	}

	// 임시 버퍼를 DX에 맞게 설정 되었는지 확인
	if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer))) {
		return false;
	}

	// 임시 버퍼 해제
	tempBuffer->Release();
	tempBuffer = 0;


	// 이후는 사운드 파일(헤더)의 문제가 없으므로 파일의 내용(사운드)을 처리

	// 사운드 파일의 일기 위치 이동 (헤더 다음 위치)
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// 사운드 데이터 생성
	unsigned char* waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData) {
		return false;
	}

	// 사운드 데이터를 읽음
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize) {
		return false;
	}

	// 파일 닫기
	error = fclose(filePtr);
	if (error != 0) {
		return false;
	}


	// 전면 버퍼에 사운드 데이터를 쓰기 위해 전면 버퍼를 잠금
	unsigned char* bufferPtr = nullptr;
	unsigned long bufferSize = 0;
	if (FAILED((*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0))) {
		return false;
	}

	// 전면 버퍼에 쓰기
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// 잠금 해제
	if (FAILED((*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0))) {
		return false;
	}

	// 임시 데이터 해제
	delete[] waveData;
	waveData = 0;

	return true;
}





// 해제
void SoundClass::Shutdown() {

	ShutdownWaveFile(&m_secondaryBuffer1);

	ShutdownDirectSound();
}


// 파일 해제
void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer) {

	if (*secondaryBuffer) {
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
}


// 사운드 자원 해제
void SoundClass::ShutdownDirectSound() {

	if (m_primaryBuffer) {
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	if (m_DirectSound) {
		m_DirectSound->Release();
		m_DirectSound = 0;
	}
}






// 사운드 재생
bool SoundClass::PlayWaveFile() {

	// 사운드 재생 위치 설정
	if (FAILED(m_secondaryBuffer1->SetCurrentPosition(0))) {
		return false;
	}

	// 볼륨 설정
	// DSBVOLUME_MAX : 최대
	if (FAILED(m_secondaryBuffer1->SetVolume(DSBVOLUME_MAX))) {
		return false;
	}

	/*
		IDirectSoundBuffer8::GetVolume()	// 사운드 크기 설정
			DSBVOLUME_MIN, DSBVOLUME_MAX
		
		IDirectSoundBuffer8::GetFrequency()	// 사운드 속도를 설정
			DSBFREQUENCY_MIN,  DSBFREQUENCY_MAX

		IDirectSoundBuffer8::GetPan()		// 사운드 위치(스테레오) 설정
			DSBPAN_LEFT, DSBPAN_CENTER, DSBPAN_RIGHT
	*/



	// 현재 후면 버퍼에 있는 사운드를 재생
	if (FAILED(m_secondaryBuffer1->Play(0, 0, 0))) {
		return false;
	}

	return true;
}