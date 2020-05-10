#pragma once

#include "stdafx.h"

#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "BitmapClass.h"
#include "TextClass.h"

//#include "ColorShaderClass.h"
#include "TextureShaderClass.h"

#include "Lightshaderclass.h"
#include "Lightclass.h"

// ��ü ȭ��
const bool FULL_SCREEN = false;

/*	�� ���� ����ȭ (Vertical Synchronization )
		- �� ������ ���� ȭ�鿡 ǥ���� ���� �̹����� �׷���ī�尡 ����Ͽ� ����ͷ� ����
		- �׷��� ī�尡 �̹����� �����ϴ� �ӵ�(���� ���ļ�, ���� �ֻ���)�� ����Ͱ� �޴� �ӵ�(�ֻ���)�� ���̷� ���� �̹��� ������ ����(�׾ ����)
		- ���� ����ȭ ���۸� �ξ� �׷��� ī��� ������� �ֻ���(Ÿ�̹�)�� ����(���� ������ ����) 
		- ������� �ֻ����� ����(60, 144)������, �׷���ī��� ���꿡 ���� �޶���(��Ȳ�� ���� �ް��� ������ ���� ����)
		- ���� �׷��� ī���� �ֻ����� ���� ���� ����ȭ�� ������ ��ŭ ���� �ð��� ���� ������, ���� ����ȭ ���۸� ��ġ�鼭 ���� �ð��� �߻� ����

		���� ���ļ� : �ʴ� ȭ���� �׸��� Ƚ�� (60Hz : �ʴ� 60���� �׸�)
		���� ���ļ� : �ʴ� �������� ���� ��� Ƚ�� (80kHz : �ʴ� 80000�� ���� ����)
			ȭ�鿡 ������ ũ�� �ִ°� ���� ���ļ��̸�, �ַ� ���� ���ļ����� ���� ���ļ��� ������
		

	�� �׾ ���� (Tearing)
		- ���÷��� ��ġ�� ��κ� �⺻������ ���� ���� ������� ó����
			����Ʈ ����(front) : ���� �����ӿ� �ش��ϴ� ȭ���� ���
			�� ����(back) : ���� �����ӿ� �ش��ϴ� ȭ���� �׷� ����
			������ ���� �� ���ۿ� ����Ʈ ������ ȭ���� ��ü(switch)�ϸ� ���÷��̿� ���
		- �� ��Ŀ��� �� ���ۿ� ȭ���� �׸� �� ȭ�� ��ü�� �Ǹ� ���� �����Ӱ� ���� �������� ȭ���� ���̴� ������ �߻� <- �׾ ����
		- �׾ ������ �����ϱ� ���� ��������ȭ�� ����ϰų�, ���� �Ǵ� ���� ���۸� ���

	# https://namu.wiki/w/%EC%88%98%EC%A7%81%EB%8F%99%EA%B8%B0%ED%99%94
*/
const bool VSYNC_ENABLED = true;

// ī�޶��� �ִ� �þ߰Ÿ�
// Frustum������ far��
const float SCREEN_DEPTH = 1000.0f;

// ī�޶��� �ּ� �þ� �Ÿ�
// Frustum������ near��
const float SCREEN_NEAR = 0.1f;


// DirectX�� ����Ͽ� �����쿡 ������ �ϴ� Ŭ����
class GraphicsClass {
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	bool Frame(int, int);
	bool Frame(int, int, int, int, float);

private:
	//bool Render();
	bool Render(float);

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;

	// ���� ���̴�(Color.vs�� Color.ps)�� ���
	//ColorShaderClass* m_ColorShader = nullptr;

	// �ؽ��� ���̴�(Texture.vs�� Texture.ps)�� ���
	//TextureShaderClass* m_TextureShader = nullptr;
	
	// ���� ���̴�(Light.vs�� Light.ps)�� ���
	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	// 2D�̹���
	BitmapClass* m_Bitmap;
	// 2D�� ������ �ؽ��� ���̴�
	TextureShaderClass* m_TextureShader;


	// �ؽ�Ʈ
	TextClass* m_Text;
};