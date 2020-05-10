#pragma once

#include "stdafx.h"
#include "TextureClass.h"



// �۲�(��Ʈ) Ŭ����
class FontClass {
private:

	// ��Ʈ ����
	struct FontType {
		float left, right;
		int size;
	};

	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, char*, float, float);

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	FontType* m_Font;
	TextureClass* m_Texture;
};