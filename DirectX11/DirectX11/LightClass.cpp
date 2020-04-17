
#include "LightClass.h"


LightClass::LightClass() {
}


LightClass::LightClass(const LightClass& other) {
}


LightClass::~LightClass() {
}



// 빛의 주변광 설정
void LightClass::SetAmbientColor(float red, float green, float blue, float alpha) {
	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
}



// 빛의 색상 설정
void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha) {
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
}


// 빛의 방향 설정
void LightClass::SetDirection(float x, float y, float z) {
	m_direction = XMFLOAT3(x, y, z);
}


// 빛의 정반사 설정
void LightClass::SetSpecularColor(float red, float green, float blue, float alpha) {
	m_specularColor = XMFLOAT4(red, green, blue, alpha);
	return;
}


// 빛의 정반사 정도 설정
void LightClass::SetSpecularPower(float power) {
	m_specularPower = power;
	return;
}



// 빛의 주변광 리턴
XMFLOAT4 LightClass::GetAmbientColor() {
	return m_ambientColor;
}


// 빛의 색상 리턴
XMFLOAT4 LightClass::GetDiffuseColor() {
	return m_diffuseColor;
}


// 빛의 방향 리턴
XMFLOAT3 LightClass::GetDirection() {
	return m_direction;
}


// 빛의 정반사 색상 리턴
XMFLOAT4 LightClass::GetSpecularColor() {
	return m_specularColor;
}


// 빛의 정반사 정도 리턴
float LightClass::GetSpecularPower() {
	return m_specularPower;
}

