#pragma once

#include "stdafx.h"

// ������ ����ϴ� ���̴� Ŭ����
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

		// �ֺ���
		XMFLOAT4 ambientColor;

		// �⺻��(���ݻ籤)
		XMFLOAT4 diffuseColor;

		// ���� ����
		XMFLOAT3 lightDirection;

		// �׷��� ī�忡 ������ ���޽� ũ��, ������ ���� �Ͽ� ���� ��
		// ũ�⸦ ���߱� ���� �������� 8bit�� ����
		//float padding;

		// ���ݻ緮
		float specularPower;

		// ���ݻ籤
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
	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D11SamplerState* m_sampleState = nullptr;
	ID3D11Buffer* m_matrixBuffer = nullptr;
	ID3D11Buffer* m_lightBuffer;


	ID3D11Buffer* m_cameraBuffer;
};