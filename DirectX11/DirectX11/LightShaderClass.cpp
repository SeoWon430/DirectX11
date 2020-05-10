
#include "LightShaderClass.h"


LightShaderClass::LightShaderClass() {

	m_vertexShader = nullptr;
	m_pixelShader = nullptr;

	m_layout = nullptr;
	m_sampleState = nullptr;

	m_matrixBuffer = nullptr;
	m_lightBuffer = nullptr;
	m_cameraBuffer = nullptr;
}


LightShaderClass::LightShaderClass(const LightShaderClass& other) {
}


LightShaderClass::~LightShaderClass() {
}




// 초기화
bool LightShaderClass::Initialize(ID3D11Device* device, HWND hwnd) {

	WCHAR vs[] = L"../DirectX11/Light.vs";
	WCHAR ps[] = L"../DirectX11/Light.ps";
	// 조명에 관한 정점 및 픽셀 쉐이더를 초기화
	return InitializeShader(device, hwnd, vs, ps);
}


// 셰이더 초기화
bool LightShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) {

	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;



	// 버텍스 셰이더 코드를 컴파일
	ID3D10Blob* vertexShaderBuffer = nullptr;
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);

	if (FAILED(result)) {
		if (errorMessage) {
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else {
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}



	// 픽셀 셰이더 코드를 컴파일
	ID3D10Blob* pixelShaderBuffer = nullptr;
	result = D3DCompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);

	if (FAILED(result)) {
		if (errorMessage) {
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		else {
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}



	// 버퍼로부터 정점 셰이더를 생성
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result)) {
		return false;
	}

	// 버퍼에서 픽셀 셰이더를 생성
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result)) {
		return false;
	}




	// 정점 입력 레이아웃 구조체를 설정
	// 이 설정은 ModelClass와 셰이더의 VertexType 구조와 일치해야함
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result)) {
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result)) {
		return false;
	}


	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	

	D3D11_BUFFER_DESC cameraBufferDesc;
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
	if (FAILED(result)) {
		return false;
	}



	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;


	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result)) {
		return false;
	}

	return true;
}





// 셰이더를 적용하여 렌더링
bool LightShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor, XMFLOAT4 specularColor, float specularPower, XMFLOAT3 cameraPosition) {

	// 렌더링에 사용할 셰이더 매개 변수를 설정
	if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, diffuseColor, ambientColor, specularColor, specularPower, cameraPosition)) {
		return false;
	}

	// 설정된 버퍼를 셰이더로 렌더링
	RenderShader(deviceContext, indexCount);

	return true;
}


// 셰이더 적용
void LightShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) {

	// 정점 입력 레이아웃을 설정
	deviceContext->IASetInputLayout(m_layout);

	// 삼각형을 그릴 정점 셰이더와 픽셀 셰이더를 설정
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// 픽셀 쉐이더에서 샘플러 상태를 설정
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// 삼각형을 그림
	deviceContext->DrawIndexed(indexCount, 0, 0);
}


// 셰이더 파일에 파라미터를 설정
bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor, XMFLOAT4 specularColor, float specularPower, XMFLOAT3 cameraPosition) {

	// 셰이더에 맞게 행렬을 설정
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);



	// 상수 버퍼의 내용을 쓸 수 있도록 잠금
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	// 행렬 버퍼의 데이터에 대한 포인터를 가져옴
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	// 행렬 버퍼에 행렬을 복사
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// 상수 버퍼의 잠금을 해제
	deviceContext->Unmap(m_matrixBuffer, 0);

	// 셰이더에서의 버퍼의 위치를 설정 (첫번째 버퍼)
	unsigned int bufferNumber = 0;




	// 정점 셰이더의 행렬 버퍼를 가지고 바뀐 값으로 변경 (좌표변환 행렬 파라미터 설정)
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);




	// 상수 버퍼를 잠금
	if (FAILED(deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	// 카메라 버퍼에 대한 포인터 설정
	CameraBufferType* dataPtr3 = (CameraBufferType*)mappedResource.pData;

	// 카메라 버퍼에 값 대입
	dataPtr3->cameraPosition = cameraPosition;
	dataPtr3->padding = 0.0f;

	// 상수 버퍼 잠금 해제
	deviceContext->Unmap(m_cameraBuffer, 0);
	
	// 셰이더에서 버퍼의 위치 지정 (두번째)
	bufferNumber = 1;

	// 카메라 버퍼를 가지고 정점셰이더에 파라미터 값 대입
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);




	// 픽셀 셰이더에서 셰이더 텍스처 리소스를 설정
	deviceContext->PSSetShaderResources(0, 1, &texture);




	//상수 버퍼를 잠금
	if (FAILED(deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	// 조명 버퍼의 데이터에 대한 포인터를 설정
	LightBufferType* dataPtr2 = (LightBufferType*)mappedResource.pData;

	// 조명 변수를 상수 버퍼에 복사
	dataPtr2->ambientColor = ambientColor;
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->specularPower = specularPower;
	dataPtr2->specularColor = specularColor;
	//dataPtr2->padding = 0.0f;

	// 상수 버퍼의 잠금을 해제
	deviceContext->Unmap(m_lightBuffer, 0);

	// 픽셀 쉐이더에서 광원 상수 버퍼의 위치를 초기화
	bufferNumber = 0;

	// 마지막으로 업데이트 된 값으로 픽셀 쉐이더에서 광원 상수 버퍼를 설정
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);



	return true;
}





// 종료
void LightShaderClass::Shutdown() {
	// 버텍스 및 픽셀 쉐이더와 관련된 객체를 종료
	ShutdownShader();
}


// 모든 버퍼 해제
void LightShaderClass::ShutdownShader() {


	if(m_lightBuffer) {
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	if (m_cameraBuffer) {
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
	}

	if (m_matrixBuffer) {
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	if (m_sampleState) {
		m_sampleState->Release();
		m_sampleState = 0;
	}

	if (m_layout) {
		m_layout->Release();
		m_layout = 0;
	}

	if (m_pixelShader) {
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	if (m_vertexShader) {
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}



void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename) {

	OutputDebugStringA(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()));

	errorMessage->Release();
	errorMessage = 0;

	MessageBox(hwnd, L"Error compiling shader.", shaderFilename, MB_OK);
}