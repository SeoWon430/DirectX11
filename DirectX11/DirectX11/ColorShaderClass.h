#pragma once

#include "stdafx.h"


// �÷��� �����ϱ� ���� ���̴� Ŭ����
// Color.vs�� Color.ps�� ����Ͽ� ������Ʈ�� ������ ����, ������ ��
class ColorShaderClass {
private:

	// ������Ʈ�� ������ ����, ��, ���� ���
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
	// ���ؽ� ���̴�
	ID3D11VertexShader* m_vertexShader = nullptr;

	// �ȼ� ���̴�
	ID3D11PixelShader* m_pixelShader = nullptr;

	// ��ǲ ���̾ƿ�
	ID3D11InputLayout* m_layout = nullptr;

	// ��� ����
	ID3D11Buffer* m_matrixBuffer = nullptr;
};