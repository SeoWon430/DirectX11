
#include "BitmapClass.h"


BitmapClass::BitmapClass() {
}


BitmapClass::BitmapClass(const BitmapClass& other) {
}


BitmapClass::~BitmapClass() {
}




bool BitmapClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, char* textureFilename, int bitmapWidth, int bitmapHeight) {

	// 화면 크기
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// 이미지 크기
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// 이전 렌더링 위치 초기화
	// 시작부분으로 이전 렌더링은 없기에 있을수 없는 값으로 초기화 (위치는 화면 크기의 값 사이에 존재)
	m_previousPosX = -1;
	m_previousPosY = -1;

	// 정점, 색인버퍼 초기화
	if (!InitializeBuffers(device)) {
		return false;
	}

	// 적용할 텍스쳐 로드
	return LoadTexture(device, deviceContext, textureFilename);
}


// 2D모델에 적용할 버퍼 생성
bool BitmapClass::InitializeBuffers(ID3D11Device* device) {

	// 정점과 색인의 수
	// 삼각형 2개로 사각형을 만들어 텍스쳐를 적용 할 것
	m_indexCount = m_vertexCount = 6;

	// 정점 배열 생성
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices) {
		return false;
	}

	// 정점 배열을 0으로 초기화
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// 색인 배열 생성
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices) {
		return false;
	}

	//색인 배열 설정
	for (int i = 0; i < m_indexCount; i++) {
		indices[i] = i;
	}



	// 정적 정점 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer))) {
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer))) {
		return false;
	}


	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}





// 렌더링
bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY) {

	// 화면의 다른 위치로 렌더링하기 위해 동적 정점 버퍼를 다시 빌드
	// 매 프레임 렌더링 시 위치가 이동 하는 것을 고려
	if (!UpdateBuffers(deviceContext, positionX, positionY)) {
		return false;
	}

	// 그래픽 파이프 라인에 정점과 색인을 버퍼에 넣음
	RenderBuffers(deviceContext);

	return true;
}


// 버퍼 업데이트
bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY) {

	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;


	// 매개변수로 받은 positionX,Y는 이동 할 좌표, m_previousPosX,Y는 현재 좌표
	// 이동 좌표 변동이 없을경우 생략
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY)) {
		return true;
	}



	// 위치 업데이트
	m_previousPosX = positionX;
	m_previousPosY = positionY;



	// 비트 맵 왼쪽의 화면 좌표
	left = (float)((m_screenWidth / 2) * -1) + (float)positionX;

	// 비트 맵 오른쪽의 화면 좌표
	right = left + (float)m_bitmapWidth;

	// 비트 맵 상단의 화면 좌표
	top = (float)(m_screenHeight / 2) - (float)positionY;

	// 비트 맵 아래쪽의 화면 좌표
	bottom = top - (float)m_bitmapHeight;



	// 정점 배열을 생성
	vertices = new VertexType[m_vertexCount];
	if (!vertices) {
		return false;
	}



	// 첫 번째 삼각형
	vertices[0].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);



	// 두 번째 삼각형
	vertices[3].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);




	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}

	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	deviceContext->Unmap(m_vertexBuffer, 0);

	delete[] vertices;
	vertices = 0;

	return true;
}


// 버퍼를 렌더링
void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {

	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}





// 닫기
void BitmapClass::Shutdown() {

	ReleaseTexture();

	ShutdownBuffers();
}


// 버퍼 해제
void BitmapClass::ShutdownBuffers() {

	if (m_indexBuffer) {
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer) {
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}





int BitmapClass::GetIndexCount() {
	return m_indexCount;
}



ID3D11ShaderResourceView* BitmapClass::GetTexture() {
	return m_Texture->GetTexture();
}



bool BitmapClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename) {

	m_Texture = new TextureClass;
	if (!m_Texture) {
		return false;
	}

	return m_Texture->Initialize(device, deviceContext, filename);
}


void BitmapClass::ReleaseTexture() {
	// 텍스처 오브젝트를 릴리즈한다.
	if (m_Texture) {
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}