#pragma once

#include "stdafx.h"

// ����Ʈ(����, ��) Ŭ����
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

	// ���� �ֺ���
	XMFLOAT4 m_ambientColor;

	// ���� ����(���ݻ� ����)
	XMFLOAT4 m_diffuseColor;

	// ���� ����
	XMFLOAT3 m_direction;

	// ���� ���ݻ� ����
	float m_specularPower;

	// ���� ���ݻ� ����
	XMFLOAT4 m_specularColor;
};