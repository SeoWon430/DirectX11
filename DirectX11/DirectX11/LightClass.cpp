
#include "LightClass.h"


LightClass::LightClass() {

	m_ambientColor = XMFLOAT4();
	m_diffuseColor = XMFLOAT4();
	m_direction = XMFLOAT3();

	m_specularPower = 0;
	m_specularColor = XMFLOAT4();
}


LightClass::LightClass(const LightClass& other) {
}


LightClass::~LightClass() {
}



// ���� �ֺ��� ����
void LightClass::SetAmbientColor(float red, float green, float blue, float alpha) {
	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
}



// ���� ���� ����
void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha) {
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
}


// ���� ���� ����
void LightClass::SetDirection(float x, float y, float z) {
	m_direction = XMFLOAT3(x, y, z);
}


// ���� ���ݻ� ����
void LightClass::SetSpecularColor(float red, float green, float blue, float alpha) {
	m_specularColor = XMFLOAT4(red, green, blue, alpha);
	return;
}


// ���� ���ݻ� ���� ����
void LightClass::SetSpecularPower(float power) {
	m_specularPower = power;
	return;
}



// ���� �ֺ��� ����
XMFLOAT4 LightClass::GetAmbientColor() {
	return m_ambientColor;
}


// ���� ���� ����
XMFLOAT4 LightClass::GetDiffuseColor() {
	return m_diffuseColor;
}


// ���� ���� ����
XMFLOAT3 LightClass::GetDirection() {
	return m_direction;
}


// ���� ���ݻ� ���� ����
XMFLOAT4 LightClass::GetSpecularColor() {
	return m_specularColor;
}


// ���� ���ݻ� ���� ����
float LightClass::GetSpecularPower() {
	return m_specularPower;
}

