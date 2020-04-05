#pragma once

#include "stdafx.h"


// 카메라(시야) 관련 클래스
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

	// 카메라의 위치값
	XMFLOAT3 m_position;

	// 카메라의 회전값
	XMFLOAT3 m_rotation;

	// 카메라의 뷰 행렬
	XMMATRIX m_viewMatrix;
};
