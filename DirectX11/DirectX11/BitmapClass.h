#pragma once

#include "stdafx.h"
#include "TextureClass.h"


// 2D ������
// * ModelClass�� 3D������
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
	bool LoadText(ID3D11Device*, ID3D11DeviceContext*, char*, const char*);
	void ReleaseTexture();

private:
	// 2D�� (�ﰢ���� ���� 2D���� ����)
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount;
	int m_indexCount;

	// ������ �� �̹���(�ؽ���)
	TextureClass* m_Texture;

	// ȭ�� ũ��
	int m_screenWidth;
	int m_screenHeight;

	// ��Ʈ�� ũ��
	int m_bitmapWidth;
	int m_bitmapHeight;

	// ���� ������ ��ġ
	int m_previousPosX;
	int m_previousPosY;
};