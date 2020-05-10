#pragma once

#include "stdafx.h"

// 조명을 계산하는 셰이더 클래스
class LightShaderClass {
private:
	struct MatrixBufferType {
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};



	struct CameraBufferType {

		XMFLOAT3 cameraPosition;
		float padding;
	};



	struct LightBufferType {

		// 주변광
		XMFLOAT4 ambientColor;

		// 기본광(난반사광)
		XMFLOAT4 diffuseColor;

		// 빛의 방향
		XMFLOAT3 lightDirection;

		// 그래픽 카드에 데이터 전달시 크기, 순서를 통일 하여 전달 함
		// 크기를 맞추기 위해 여분으로 8bit를 선언
		//float padding;

		// 정반사량
		float specularPower;

		// 정반사광
		XMFLOAT4 specularColor;
	};

public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4, XMFLOAT4, XMFLOAT4, float, XMFLOAT3);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4, XMFLOAT4, XMFLOAT4, float, XMFLOAT3);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;


	ID3D11Buffer* m_cameraBuffer;
};