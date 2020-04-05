#pragma once

#include "stdafx.h"


// ī�޶�(�þ�) ���� Ŭ����
class CameraClass {

public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);

private:

	// ī�޶��� ��ġ��
	XMFLOAT3 m_position;

	// ī�޶��� ȸ����
	XMFLOAT3 m_rotation;

	// ī�޶��� �� ���
	XMMATRIX m_viewMatrix;
};
