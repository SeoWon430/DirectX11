
#include "TextClass.h"


TextClass::TextClass() {
	m_Font = nullptr;
	m_FontShader = nullptr;
	m_screenWidth = 0;
	m_screenHeight = 0;
	m_baseViewMatrix = XMMatrixIdentity();
	m_sentence1 = nullptr;
	m_sentence2 = nullptr;
}

TextClass::TextClass(const TextClass& other) {
}

TextClass::~TextClass() {
}





// 초기화
bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd
	, int screenWidth, int screenHeight, XMMATRIX baseViewMatrix) {

	bool result;

	// 화면 크기
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// 카메라 기준 행렬
	m_baseViewMatrix = baseViewMatrix;


	// 폰트 객체 생성
	m_Font = new FontClass;
	if (!m_Font) {
		return false;
	}

	// 폰트 파일을 읽어 옴 (폰트에 대한 형식)
	char fontFile[] = "../DirectX11/Font/fontdata.txt";
	WCHAR fontTex[] = L"../DirectX11/Font/font.dds";

	// 폰트 초기화
	result = m_Font->Initialize(device, fontFile, fontTex);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// 폰트 셰이더 생성
	m_FontShader = new FontShaderClass;
	if (!m_FontShader) {
		return false;
	}

	// 폰트 셰이더 초기화
	result = m_FontShader->Initialize(device, hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}



	// 텍스트를 쓸 객체 초기화
	result = InitializeSentence(&m_sentence1, 16, device);
	if (!result) {
		return false;
	}

	// 텍스트1 설정
	char text1[] = "Hello";
	result = UpdateSentence(m_sentence1, text1, 100, 100, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result) {
		return false;
	}



	// 텍스트를 쓸 객체 초기화
	result = InitializeSentence(&m_sentence2, 16, device);
	if (!result) {
		return false;
	}

	// 텍스트2 설정
	char text2[] = "Bye";
	result = UpdateSentence(m_sentence2, text2, 100, 200, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result) {
		return false;
	}

	return true;
}


// 문장 초기화
bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device) {

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	*sentence = new SentenceType;
	if (!*sentence) {
		return false;
	}

	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	(*sentence)->maxLength = maxLength;

	(*sentence)->vertexCount = 6 * maxLength;

	(*sentence)->indexCount = (*sentence)->vertexCount;

	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices) {
		return false;
	}

	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices) {
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	for (i = 0; i < (*sentence)->indexCount; i++) {
		indices[i] = i;
	}


	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;


	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;


	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result)) {
		return false;
	}


	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;


	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;


	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result)) {
		return false;
	}


	delete[] vertices;
	vertices = 0;


	delete[] indices;
	indices = 0;

	return true;
}


// 문장 업데이트
bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY
	, float red, float green, float blue, ID3D11DeviceContext* deviceContext) {

	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;


	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	numLetters = (int)strlen(text);

	if (numLetters > sentence->maxLength) {
		return false;
	}

	vertices = new VertexType[sentence->vertexCount];
	if (!vertices) {
		return false;
	}


	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));


	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);


	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);


	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}


	verticesPtr = (VertexType*)mappedResource.pData;


	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));


	deviceContext->Unmap(sentence->vertexBuffer, 0);


	delete[] vertices;
	vertices = 0;

	return true;
}





// 렌더링
bool TextClass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX orthoMatrix) {

	bool result;


	// 첫 문장 렌더링
	result = RenderSentence(deviceContext, m_sentence1, worldMatrix, orthoMatrix);
	if (!result) {
		return false;
	}

	// 두번째 문장 렌더링
	result = RenderSentence(deviceContext, m_sentence2, worldMatrix, orthoMatrix);
	if (!result) {
		return false;
	}

	return true;
}


// 문장 렌더링
bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence
	, XMMATRIX worldMatrix, XMMATRIX orthoMatrix) {

	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;


	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);

	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatrix, m_Font->GetTexture(), pixelColor);
	if (!result) {
		false;
	}

	return true;
}





// 해제
void TextClass::Shutdown() {

	ReleaseSentence(&m_sentence1);

	ReleaseSentence(&m_sentence2);

	if (m_FontShader) {
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	if (m_Font) {
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}


// 텍스츠 해제
void TextClass::ReleaseSentence(SentenceType** sentence) {

	if (*sentence) {

		if ((*sentence)->vertexBuffer) {
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		if ((*sentence)->indexBuffer) {
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		delete* sentence;
		*sentence = 0;
	}

	return;
}






// fps 표시
bool TextClass::SetFps(int fps, ID3D11DeviceContext* deviceContext) {

	if (fps > 9999) {
		fps = 9999;
	}

	// fps 정수를 문자열 형식으로 변환
	char tempString[16] = { 0, };
	_itoa_s(fps, tempString, 10);

	// fps 문자열을 설정
	char fpsString[16] = { 0, };
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	float red = 0;
	float green = 0;
	float blue = 0;

	// fps가 60 이상이면 fps 색상을 녹색으로 설정
	if (fps >= 60) {
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// fps가 60보다 작은 경우 fps 색상을 노란색으로 설정
	if (fps < 60) {
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// fps가 30 미만이면 fps 색상을 빨간색으로 설정
	if (fps < 30) {
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// 문장 정점 버퍼를 새 문자열 정보로 업데이트
	return UpdateSentence(m_sentence1, fpsString, 20, 20, red, green, blue, deviceContext);
}



// cpu할당량 표시
bool TextClass::SetCpu(int cpu, ID3D11DeviceContext* deviceContext) {

	// cpu 정수를 문자열 형식으로 변환
	char tempString[16] = { 0, };
	_itoa_s(cpu, tempString, 10);

	// cpu 문자열을 설정합
	char cpuString[16] = { 0, };
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	// 문장 정점 버퍼를 새 문자열 정보로 업데이트
	return UpdateSentence(m_sentence2, cpuString, 20, 40, 0.0f, 1.0f, 0.0f, deviceContext);
}