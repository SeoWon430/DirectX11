#pragma once



#define _CRT_SECURE_NO_WARNINGS

#include "stb_truetype.h"
#include <fstream>


#include "stdafx.h"

// .dds파일 로드 헤더
#include "DDSTextureLoader.h"



// 텍스쳐(이미지) 정보를 가질 클래스
class TextureClass {
private:

	// 이미지 형식은 Targa
	struct TargaHeader {
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, WCHAR*);
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadTarga(char*, int&, int&);

private:
	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
};