#pragma once

#include "stdafx.h"


// 컬러를 적용하기 위한 셰이더 클래스
// Color.vs와 Color.ps를 사용하여 오브젝트에 색상을 적용, 렌더링 함
class ColorShaderClass {
private:

	// 오브젝트에 적용할 월드, 뷰, 투영 행렬
	struct MatrixBufferType {
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);
	~ColorShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	// 버텍스 셰이더
	ID3D11VertexShader* m_vertexShader;

	// 픽셀 셰이더
	ID3D11PixelShader* m_pixelShader;

	// 인풋 레이아웃
	ID3D11InputLayout* m_layout;

	// 행렬 버퍼
	ID3D11Buffer* m_matrixBuffer;
};