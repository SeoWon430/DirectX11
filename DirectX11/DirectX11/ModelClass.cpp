
#include "ModelClass.h"


ModelClass::ModelClass() {
}


ModelClass::ModelClass(const ModelClass& other) {
}


ModelClass::~ModelClass() {
}





// �ʱ�ȭ
// ����, ���� ���۸� �ʱ�ȭ
bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename) {

	// ���� �� ���� ���۸� �ʱ�ȭ
	if (!InitializeBuffers(device)) {
		return false;
	}

	// �𵨿� �ؽ��ĸ� �����ϱ� ���� �ҷ���
	return LoadTexture(device, deviceContext, textureFilename);
}


// ���� �ʱ�ȭ
bool ModelClass::InitializeBuffers(ID3D11Device* device) {

	// ���� �迭�� ���� ���� ����
	m_vertexCount = 3;

	// ���� �迭�� �ε��� ���� ����
	m_indexCount = 3;

	// ���� �迭�� ����
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices) {
		return false;
	}

	// ���� �迭�� ����
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices) {
		return false;
	}

	

	// ���� �迭�� ���� ����
	// �ﰢ�� ������� ����
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	//vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[1].texture = XMFLOAT2(0.5f, 0.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);


	// ���� �迭�� ���� ����
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	// ���� ���� ������ ����ü�� ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� ����
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ���۸� ����
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer))) {
		return false;
	}

	// ���� ���� ������ ����ü�� ����
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ���� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ�
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// ���� ���۸� ����
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer))) {
		return false;
	}

	// �����ǰ� ���� �Ҵ�Ǿ� ���̻� ��� ���� ���� ���� ���ۿ� ���� ���۸� ����
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


// �ؽ��� �ҷ�����
bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename) {

	// �ؽ�ó ������Ʈ�� ����
	m_Texture = new TextureClass;
	if (!m_Texture) {
		return false;
	}

	// �ؽ�ó ������Ʈ�� �ʱ�ȭ
	return m_Texture->Initialize(device, deviceContext, filename);
}


void ModelClass::ReleaseTexture() {
	// �ؽ�ó ������Ʈ�� ����
	if (m_Texture) {
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}





// ��(������Ʈ)�� ȭ�鿡 ������
void ModelClass::Render(ID3D11DeviceContext* deviceContext) {

	// ������Ʈ �������� ���� �׷��� ������ ���ο� ������ ���� ���۸� ����
	RenderBuffers(deviceContext);
}


// 
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {

	// ���� ������ ������ �������� ����
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// ������ �� �� �ֵ��� �Է� ����������� ���� ���۸� Ȱ������ ����
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// ������ �� �� �ֵ��� �Է� ����������� �ε��� ���۸� Ȱ������ ����
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ������ ����
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}





// ������ ���� ����
int ModelClass::GetIndexCount() {
	return m_indexCount;
}



// ���� �ε��Ͽ� ������ �ؽ��ĸ� ����
ID3D11ShaderResourceView* ModelClass::GetTexture() {
	return m_Texture->GetTexture();
}




// ����
// ����, ���� ���۸� ����
void ModelClass::Shutdown() {

	// �� �ؽ��ĸ� ����
	ReleaseTexture();

	// ���ؽ� �� �ε��� ���۸� ����
	ShutdownBuffers();
}


void ModelClass::ShutdownBuffers()
{
	// �ε��� ���۸� �����մϴ�.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// ���� ���۸� �����մϴ�.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}