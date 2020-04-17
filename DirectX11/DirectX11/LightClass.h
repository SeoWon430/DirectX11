#pragma once

#include "stdafx.h"

// 라이트(조명, 빛) 클래스
class LightClass {
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularPower(float);
	void SetSpecularColor(float, float, float, float);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();
	float GetSpecularPower();
	XMFLOAT4 GetSpecularColor();

private:

	// 빛의 주변광
	XMFLOAT4 m_ambientColor;

	// 빛의 색상(난반사 색상)
	XMFLOAT4 m_diffuseColor;

	// 빛의 방향
	XMFLOAT3 m_direction;

	// 빛의 정반사 정도
	float m_specularPower;

	// 빛의 정반사 색상
	XMFLOAT4 m_specularColor;
};