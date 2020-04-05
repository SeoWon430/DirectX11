#pragma once

#include "stdafx.h"
#include "TextureClass.h"


// 3D������Ʈ�� �޾ƿ� ����
class ModelClass {

private:
	struct VertexType {
		XMFLOAT3 position;
		//XMFLOAT4 color;
		XMFLOAT2 texture;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

private:

	/* �� ����(Vertex)
		- ������Ʈ(Model)�� �����ϴ� 3���� ���� ��
		- �� ������ ����� ������, ���� 3���� ����� �ﰢ����, �ﰢ�� �������� ����� ������Ʈ�� ������

	   �� ����(Index)
		- ������ �迭
		- �� ������ 3���� ������ float3(24bit) ������ �� ������ ��ȣ�� �ο��Ͽ� �����ϸ� int(4bit)�� ���� ����


		������ ������ ������ ���� DirectX���� �׸����� �ϱ����� ����(���� ���ۿ� ���� ����)�� ���� �ϰ� ��
	*/

	// ���� ����
	ID3D11Buffer* m_vertexBuffer = nullptr;

	// ���� ����
	ID3D11Buffer* m_indexBuffer = nullptr;

	// ���� ����
	int m_vertexCount = 0;

	// ���� ����
	int m_indexCount = 0;


	TextureClass* m_Texture = nullptr;
};