
#include "ModelClass.h"


ModelClass::ModelClass() {
}


ModelClass::ModelClass(const ModelClass& other) {
}


ModelClass::~ModelClass() {
}





// 초기화
// 정점, 색인 버퍼를 초기화
bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename) {

	// 정점 및 색인 버퍼를 초기화
	if (!InitializeBuffers(device)) {
		return false;
	}

	// 모델에 텍스쳐를 적용하기 위해 불러옴
	return LoadTexture(device, deviceContext, textureFilename);
}


// 버퍼 초기화
bool ModelClass::InitializeBuffers(ID3D11Device* device) {

	// 정점 배열의 정점 수를 설정
	m_vertexCount = 3;

	// 섹인 배열의 인덱스 수를 설정
	m_indexCount = 3;

	// 정점 배열을 생성
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices) {
		return false;
	}

	// 색인 배열을 생성
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices) {
		return false;
	}

	

	// 정점 배열에 값을 설정
	// 삼각형 모양으로 설정
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	//vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[1].texture = XMFLOAT2(0.5f, 0.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);


	// 색인 배열의 값을 설정
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	// 정적 정점 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 정점 버퍼를 생성
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer))) {
		return false;
	}

	// 정적 색인 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 색인 데이터를 가리키는 보조 리소스 구조체를 작성
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 색인 버퍼를 생성
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer))) {
		return false;
	}

	// 생성되고 값이 할당되어 더이상 사용 하지 않은 정점 버퍼와 색인 버퍼를 해제
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


// 텍스쳐 불러오기
bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename) {

	// 텍스처 오브젝트를 생성
	m_Texture = new TextureClass;
	if (!m_Texture) {
		return false;
	}

	// 텍스처 오브젝트를 초기화
	return m_Texture->Initialize(device, deviceContext, filename);
}


void ModelClass::ReleaseTexture() {
	// 텍스처 오브젝트를 해제
	if (m_Texture) {
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}





// 모델(오브젝트)을 화면에 렌더링
void ModelClass::Render(ID3D11DeviceContext* deviceContext) {

	// 오브젝트 렌더링을 위해 그래픽 파이프 라인에 정점과 색인 버퍼를 넣음
	RenderBuffers(deviceContext);
}


// 
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {

	// 정점 버퍼의 단위와 오프셋을 설정
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼로 그릴 기본형을 설정합니다. 여기서는 삼각형으로 설정
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}





// 색인의 갯수 리턴
int ModelClass::GetIndexCount() {
	return m_indexCount;
}



// 현재 로드하여 저장한 텍스쳐를 리턴
ID3D11ShaderResourceView* ModelClass::GetTexture() {
	return m_Texture->GetTexture();
}




// 종료
// 정점, 색인 버퍼를 해제
void ModelClass::Shutdown() {

	// 모델 텍스쳐를 해제
	ReleaseTexture();

	// 버텍스 및 인덱스 버퍼를 종료
	ShutdownBuffers();
}


void ModelClass::ShutdownBuffers()
{
	// 인덱스 버퍼를 해제합니다.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// 정점 버퍼를 해제합니다.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}