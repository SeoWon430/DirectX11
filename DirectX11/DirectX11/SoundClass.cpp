
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



// �ʱ�ȭ
bool SoundClass::Initialize(HWND hwnd) {

	/* ���� ���
		���� ����(Primary Buffer) : �ĸ� ������ ���� �����͸� ��� ������ ��ģ �� ��ġ�� ���� ���� ���带 ���
		�ĸ� ����(Secondary Buffer) : ���� ������ ��� ����
			�������� ���� �Ҽ� ������, �� ���� ������ ������ ����
	*/

	// ���� �ʱ�ȭ
	if (!InitializeDirectSound(hwnd)) {
		return false;
	}

	// ���� ���ۿ� ������ ���� ���� �ε�
	char fileName[] = "../DirectX11/Data/sound01.wav";
	if (!LoadWaveFile(fileName, &m_secondaryBuffer1)) {
		return false;
	}

	return PlayWaveFile();
}


// ���� �ʱ�ȭ
bool SoundClass::InitializeDirectSound(HWND hwnd) {

	// ��ġ�� ���� ���� ����
	if (FAILED(DirectSoundCreate8(NULL, &m_DirectSound, NULL))) {
		return false;
	}



	/*	���� ���� ����

		DSSCL_EXCLUSIVE : ��Ŀ�� �� ���α׷��� ����ī�带 ����
			���� ���α׷� �� ��Ŀ�� �� �Ϳ����� ���

		DSSCL_NORMAL : ���α׷��� ���� ���� �� ���


		DSSCL_PRIORITY : �����찡 ��Ȱ��(�ּ�ȭ) ���¿����� ���
			�⺻������ DSSCL_EXCLUSIVE�� ����

		DSSCL_WRITEPRIMARY : ���α׷��Ӱ� ����
			�ַ� ����� ����� ������ ���� �� ���


		��ó : http://blog.naver.com/PostView.nhn?blogId=khk6435&logNo=50135221569
	*/


	// ������ ���� ���� ����
	if (FAILED(m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY))) {
		return false;
	}



	// ���� ������ ����ü�� ä��
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

	// �ĸ� ������ ����ü�� ä��
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// ���� ������ ������ �ĸ� ���۸� ������ ��ü������ ����
	if (FAILED(m_primaryBuffer->SetFormat(&waveFormat))) {
		return false;
	}

	return true;
}


// ���� ���� �ε�
// �ε� �� ������ �ĸ� ����(secondary buffer)�� �����
bool SoundClass::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer) {

	// ���� ����
	FILE* filePtr = nullptr;
	int error = fopen_s(&filePtr, filename, "rb");
	if (error != 0) {
		return false;
	}

	// ���� ��� �б�
	WaveHeaderType waveFileHeader;
	unsigned int count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1) {
		return false;
	}

	// ���� �� chunk ID�� RIFF �������� Ȯ��
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F')) {
		return false;
	}

	// ���� �� chunk ID�� WAVE �������� Ȯ��
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E')) {
		return false;
	}

	// ���� �� chunk ID�� fmt �������� Ȯ��
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' ')) {
		return false;
	}

	// ������ WAVE_FORMAT_PCM ���� Ȯ��
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM) {
		return false;
	}

	// ������ ���׷��� ���� Ȯ��
	// ���� ������ ��� ä�� �� = ���� ��� ��ġ�� ��
	// �� ���׷���(Stereo) : 2���� ����Ŀ�� �̿��Ͽ� ��ü���� ������ ��
	if (waveFileHeader.numChannels != 2) {
		return false;
	}



	/* �� ���÷���Ʈ(Sample Rate)
		����� ���� �� �ʴ� ���� ��
		���� ����(�Ƴ��α�)�� ������������ ���α׷� �󿡼��� ����(������)�� �񿬼�����
		������ ����� ��ȯ �ϸ鼭 �Ƴ��α��� ���� �� �Ϻθ� �����ϰ� �Ǵµ� �̸� ���ø��̶� �ϸ�,
		���ø� ������ ���÷���Ʈ�� ��
		�Ϲ������� 44100Hz(44.1KHz)�� �����ϰ� �� (�ʴ� 44100��)
	*/
	// ���÷���Ʈ Ȯ�� (44100Hz)
	if (waveFileHeader.sampleRate != 44100) {
		return false;
	}


	
	// ��Ʈ����Ʈ Ȯ�� (16bit)
	// �� ��Ʈ����Ʈ (Bit Rate): �ʴ� ���۵Ǵ� �������� ��
	if (waveFileHeader.bitsPerSample != 16) {
		return false;
	}


	// ���� �� öũ ������ data ���� Ȯ�� 
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a')) {
		return false;
	}


	// �ĸ� ������ ���� ����
	// ������ �ε��� ������ ���Ŀ� ���� ����
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


	// �� ���� �� ���� ������ Ȯ�ο� �ӽ� ���� ����
	IDirectSoundBuffer* tempBuffer = nullptr;
	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL))) {
		return false;
	}

	// �ӽ� ���۸� DX�� �°� ���� �Ǿ����� Ȯ��
	if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer))) {
		return false;
	}

	// �ӽ� ���� ����
	tempBuffer->Release();
	tempBuffer = 0;


	// ���Ĵ� ���� ����(���)�� ������ �����Ƿ� ������ ����(����)�� ó��

	// ���� ������ �ϱ� ��ġ �̵� (��� ���� ��ġ)
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// ���� ������ ����
	unsigned char* waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData) {
		return false;
	}

	// ���� �����͸� ����
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize) {
		return false;
	}

	// ���� �ݱ�
	error = fclose(filePtr);
	if (error != 0) {
		return false;
	}


	// ���� ���ۿ� ���� �����͸� ���� ���� ���� ���۸� ���
	unsigned char* bufferPtr = nullptr;
	unsigned long bufferSize = 0;
	if (FAILED((*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0))) {
		return false;
	}

	// ���� ���ۿ� ����
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// ��� ����
	if (FAILED((*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0))) {
		return false;
	}

	// �ӽ� ������ ����
	delete[] waveData;
	waveData = 0;

	return true;
}





// ����
void SoundClass::Shutdown() {

	ShutdownWaveFile(&m_secondaryBuffer1);

	ShutdownDirectSound();
}


// ���� ����
void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer) {

	if (*secondaryBuffer) {
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
}


// ���� �ڿ� ����
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






// ���� ���
bool SoundClass::PlayWaveFile() {

	// ���� ��� ��ġ ����
	if (FAILED(m_secondaryBuffer1->SetCurrentPosition(0))) {
		return false;
	}

	// ���� ����
	// DSBVOLUME_MAX : �ִ�
	if (FAILED(m_secondaryBuffer1->SetVolume(DSBVOLUME_MAX))) {
		return false;
	}

	/*
		IDirectSoundBuffer8::GetVolume()	// ���� ũ�� ����
			DSBVOLUME_MIN, DSBVOLUME_MAX
		
		IDirectSoundBuffer8::GetFrequency()	// ���� �ӵ��� ����
			DSBFREQUENCY_MIN,  DSBFREQUENCY_MAX

		IDirectSoundBuffer8::GetPan()		// ���� ��ġ(���׷���) ����
			DSBPAN_LEFT, DSBPAN_CENTER, DSBPAN_RIGHT
	*/



	// ���� �ĸ� ���ۿ� �ִ� ���带 ���
	if (FAILED(m_secondaryBuffer1->Play(0, 0, 0))) {
		return false;
	}

	return true;
}