#pragma once

#include "stdafx.h"
#include "TextureClass.h"


// 2D 렌더링
// * ModelClass는 3D렌더링
class BitmapClass {
private:
	
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	BitmapClass();
	BitmapClass(const BitmapClass&);
	~BitmapClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int, char*, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

private:
	// 2D모델 (삼각형을 통해 2D도형 구성)
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;

	// 렌더링 할 이미지(텍스쳐)
	TextureClass* m_Texture = nullptr;

	// 화면 크기
	int m_screenWidth = 0;
	int m_screenHeight = 0;

	// 비트맵 크기
	int m_bitmapWidth = 0;
	int m_bitmapHeight = 0;

	// 이전 프레임 위치
	int m_previousPosX = 0;
	int m_previousPosY = 0;
};