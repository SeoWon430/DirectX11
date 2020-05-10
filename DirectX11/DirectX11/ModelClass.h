#pragma once

#include "stdafx.h"
#include "TextureClass.h"

#include <fstream>
using namespace std;

// 3D������Ʈ�� �޾ƿ� ����
class ModelClass {

private:
	struct VertexType {

		// �� ���� ��ġ
		XMFLOAT3 position;

		// �� ���� ����� ������
		//XMFLOAT4 color;

		// �� ���� �ؽ��� ��ǥ
		XMFLOAT2 texture;

		// �� ���� ����(�븻����)
		XMFLOAT3 normal;
	};


	struct ModelType {

		// ��ġ
		float x, y, z;

		// �ؽ�����ǥ
		float tu, tv;

		// ���
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	// 06. Texture����
	//bool Initialize(ID3D11Device*, char*);

	// 07. 3D Model
	bool Initialize(ID3D11Device*, ID3D11DeviceContext* deviceContext, char*, char*);

	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	// 06. Texture
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext* deviceContext, char*);
	void ReleaseTexture();

	// 07. 3D Model
	bool LoadModel(char*);
	void ReleaseModel();

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
	ID3D11Buffer* m_vertexBuffer;

	// ���� ����
	ID3D11Buffer* m_indexBuffer;

	// ���� ����
	int m_vertexCount;

	// ���� ����
	int m_indexCount;


	TextureClass* m_Texture;

	ModelType* m_model;
};