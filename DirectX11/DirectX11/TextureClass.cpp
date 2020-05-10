
#include "TextureClass.h"
#include <stdio.h>


TextureClass::TextureClass() {
	m_targaData = nullptr;
	m_texture = nullptr;
	m_textureView = nullptr;
}


TextureClass::TextureClass(const TextureClass& other) {
}


TextureClass::~TextureClass() {
}





// 초기화(dds)
bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename) {

	// .dds 파일 읽기
	if (FAILED(CreateDDSTextureFromFile(device, filename, nullptr, &m_textureView))) {
		return false;
	}

	return true;
}


// 초기화(targa)
bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename) {

	int width = 0;
	int height = 0;
	WCHAR te[] = L"DS";
	char s[] = "DS";

	// Targa 이미지를 메모리에 로드
	if (!LoadTarga(filename, height, width)) {
		return false;
	}

	//텍스처의 구조체를 설정
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// 빈 텍스처를 생성
	HRESULT hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hResult)) {
		return false;
	}

	//  targa 이미지 데이터의 너비 사이즈를 설정
	UINT rowPitch = (width * 4) * sizeof(unsigned char);

	// targa 이미지 데이터를 텍스처에 복사
	deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

	// 셰이더 리소스 뷰 구조체를 설정
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// 텍스처의 셰이더 리소스 뷰 생성
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if (FAILED(hResult))
	{
		return false;
	}

	// 이 텍스처에 대해 밉맵을 생성
	deviceContext->GenerateMips(m_textureView);

	// 이미지 데이터가 텍스처에 로드 되었으므로 targa 이미지 데이터를 해제
	delete[] m_targaData;
	m_targaData = 0;

	return true;
}


// Targa파일 읽기
bool TextureClass::LoadTarga(char* filename, int& height, int& width) {

	FILE* filePtr;
	if (fopen_s(&filePtr, filename, "rb") != 0) {
		return false;
	}

	// 파일 헤더를 읽음
	TargaHeader targaFileHeader;
	unsigned int count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1) {
		return false;
	}

	// 파일헤더에서 중요 정보를 설정
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	int bpp = (int)targaFileHeader.bpp;

	// 파일이 32bit 인지 24bit인지 체크
	if (bpp != 32) {
		return false;
	}

	// 32 비트 이미지 데이터의 크기를 계산
	int imageSize = width * height * 4;

	//  targa 이미지 데이터 용 메모리를 할당
	unsigned char* targaImage = new unsigned char[imageSize];
	if (!targaImage) {
		return false;
	}

	// targa 이미지 데이터를 읽음
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize) {
		return false;
	}

	// 파일 닫기
	if (fclose(filePtr) != 0) {
		return false;
	}

	// targa 대상 데이터에 대한 메모리를 할당
	m_targaData = new unsigned char[imageSize];
	if (!m_targaData) {
		return false;
	}

	// targa 대상 데이터 배열에 인덱스를 초기화
	int index = 0;

	// targa 이미지 데이터에 인덱스를 초기화
	int k = (width * height * 4) - (width * 4);


	// targaImage[]에 저장된 형식은 역순으로 이를 재설정하여 m_targaData[] 에 저장
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			m_targaData[index + 0] = targaImage[k + 2];  // 빨강
			m_targaData[index + 1] = targaImage[k + 1];  // 녹색
			m_targaData[index + 2] = targaImage[k + 0];  // 파랑
			m_targaData[index + 3] = targaImage[k + 3];  // 알파

			k += 4;
			index += 4;
		}

		k -= (width * 8);
	}

	// 대상 배열에 복사 된 targa 이미지 데이터를 해제
	delete[] targaImage;
	targaImage = 0;

	return true;
}





// 해제
void TextureClass::Shutdown() {

	if (m_textureView) {
		m_textureView->Release();
		m_textureView = 0;
	}

	if (m_texture) {
		m_texture->Release();
		m_texture = 0;
	}

	if (m_targaData) {
		delete[] m_targaData;
		m_targaData = 0;
	}
}




// 현재 클래스가 가진 텍스쳐 리턴
ID3D11ShaderResourceView* TextureClass::GetTexture() {
	return m_textureView;
}