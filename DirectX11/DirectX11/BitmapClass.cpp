
#include "BitmapClass.h"


BitmapClass::BitmapClass() {
}


BitmapClass::BitmapClass(const BitmapClass& other) {
}


BitmapClass::~BitmapClass() {
}




bool BitmapClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, char* textureFilename, int bitmapWidth, int bitmapHeight) {

	// ȭ�� ũ��
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// �̹��� ũ��
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// ���� ������ ��ġ �ʱ�ȭ
	// ���ۺκ����� ���� �������� ���⿡ ������ ���� ������ �ʱ�ȭ (��ġ�� ȭ�� ũ���� �� ���̿� ����)
	m_previousPosX = -1;
	m_previousPosY = -1;

	// ����, ���ι��� �ʱ�ȭ
	if (!InitializeBuffers(device)) {
		return false;
	}

	// ������ �ؽ��� �ε�
	return LoadTexture(device, deviceContext, textureFilename);
}


// 2D�𵨿� ������ ���� ����
bool BitmapClass::InitializeBuffers(ID3D11Device* device) {

	// ������ ������ ��
	// �ﰢ�� 2���� �簢���� ����� �ؽ��ĸ� ���� �� ��
	m_indexCount = m_vertexCount = 6;

	// ���� �迭 ����
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices) {
		return false;
	}

	// ���� �迭�� 0���� �ʱ�ȭ
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// ���� �迭 ����
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices) {
		return false;
	}

	//���� �迭 ����
	for (int i = 0; i < m_indexCount; i++) {
		indices[i] = i;
	}



	// ���� ���� ������ ����ü�� ����
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





// ������
bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY) {

	// ȭ���� �ٸ� ��ġ�� �������ϱ� ���� ���� ���� ���۸� �ٽ� ����
	// �� ������ ������ �� ��ġ�� �̵� �ϴ� ���� ���
	if (!UpdateBuffers(deviceContext, positionX, positionY)) {
		return false;
	}

	// �׷��� ������ ���ο� ������ ������ ���ۿ� ����
	RenderBuffers(deviceContext);

	return true;
}


// ���� ������Ʈ
bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY) {

	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;


	// �Ű������� ���� positionX,Y�� �̵� �� ��ǥ, m_previousPosX,Y�� ���� ��ǥ
	// �̵� ��ǥ ������ ������� ����
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY)) {
		return true;
	}



	// ��ġ ������Ʈ
	m_previousPosX = positionX;
	m_previousPosY = positionY;



	// ��Ʈ �� ������ ȭ�� ��ǥ
	left = (float)((m_screenWidth / 2) * -1) + (float)positionX;

	// ��Ʈ �� �������� ȭ�� ��ǥ
	right = left + (float)m_bitmapWidth;

	// ��Ʈ �� ����� ȭ�� ��ǥ
	top = (float)(m_screenHeight / 2) - (float)positionY;

	// ��Ʈ �� �Ʒ����� ȭ�� ��ǥ
	bottom = top - (float)m_bitmapHeight;



	// ���� �迭�� ����
	vertices = new VertexType[m_vertexCount];
	if (!vertices) {
		return false;
	}



	// ù ��° �ﰢ��
	vertices[0].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);



	// �� ��° �ﰢ��
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


// ���۸� ������
void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {

	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}





// �ݱ�
void BitmapClass::Shutdown() {

	ReleaseTexture();

	ShutdownBuffers();
}


// ���� ����
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
	// �ؽ�ó ������Ʈ�� �������Ѵ�.
	if (m_Texture) {
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}