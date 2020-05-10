
#include "FontClass.h"

#include <fstream>
using namespace std;


FontClass::FontClass() {
	m_Font = nullptr;
	m_Texture = nullptr;
}

FontClass::FontClass(const FontClass& other) {
}

FontClass::~FontClass() {
}




// �ʱ�ȭ
bool FontClass::Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename) {

	// ��Ʈ ���� �ε�
	if (!LoadFontData(fontFilename)) {
		return false;
	}

	// �ؽ��� �ε�
	return LoadTexture(device, textureFilename);
}

// ��Ʈ ���� �ε�
bool FontClass::LoadFontData(char* filename) {

	ifstream fin;
	int i;
	char temp;

	// ��Ʈ ������ ���� ����
	m_Font = new FontType[95];
	if (!m_Font) {
		return false;
	}

	// ���� ����
	fin.open(filename);
	if (fin.fail()) {
		return false;
	}

	// ���� �б�
	for (i = 0; i < 95; i++) {
		fin.get(temp);
		while (temp != ' ') {
			fin.get(temp);
		}

		fin.get(temp);

		while (temp != ' ') {
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// ���� �ݱ�
	fin.close();

	return true;
}


// �ؽ��� ���� �ε�
bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* filename) {

	m_Texture = new TextureClass;
	if (!m_Texture) {
		return false;
	}

	return m_Texture->Initialize(device, filename);
}





// ����
void FontClass::Shutdown() {
	ReleaseTexture();

	ReleaseFontData();
}

void FontClass::ReleaseFontData() {

	if (m_Font) {
		delete[] m_Font;
		m_Font = 0;
	}
}

void FontClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}




// ���� �ؽ��� ����
ID3D11ShaderResourceView* FontClass::GetTexture() {
	return m_Texture->GetTexture();
}



// ��Ʈ�� ���� ���� �迭 ó��
void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY) {

	VertexType* vertexPtr;
	int numLetters, index, i, letter;


	vertexPtr = (VertexType*)vertices;

	numLetters = (int)strlen(sentence);

	index = 0;

	for (i = 0; i < numLetters; i++) {
		letter = ((int)sentence[i]) - 32;

		if (letter == 0) {
			drawX = drawX + 3.0f;
		}
		else
		{
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f); 
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f); 
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f); 
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f); 
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f); 
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}