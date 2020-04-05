#pragma once

#include "stdafx.h"
#include "TextureClass.h"


// 3D오브젝트를 받아와 생성
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

	/* ★ 정점(Vertex)
		- 오브젝트(Model)를 구성하는 3차원 상의 점
		- 두 정점의 연결로 선분을, 선분 3개의 연결로 삼각형을, 삼각형 여러개의 연결로 오브젝트를 구성함

	   ★ 색인(Index)
		- 정점의 배열
		- 각 정점은 3차원 값으로 float3(24bit) 이지만 이 정점에 번호를 부여하여 관리하면 int(4bit)로 관리 가능


		정점과 색인의 정보를 토대로 DirectX에서 그리도록 하기위해 버퍼(정점 버퍼와 색인 버퍼)로 구성 하게 됨
	*/

	// 정점 버퍼
	ID3D11Buffer* m_vertexBuffer = nullptr;

	// 색인 버퍼
	ID3D11Buffer* m_indexBuffer = nullptr;

	// 정점 갯수
	int m_vertexCount = 0;

	// 색인 갯수
	int m_indexCount = 0;


	TextureClass* m_Texture = nullptr;
};