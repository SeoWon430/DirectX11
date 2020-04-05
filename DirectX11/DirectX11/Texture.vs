

/* ★ 버텍스 셰이더(Vertex Shader)
	3D오브젝트를 구성하는 정점(Vertex)를 월드 공간에 배치 하도록 계산
	로컬 상 좌표 -> 월드 상 좌표 -> 카메라 기준 좌표 -> 투시 적용
			(*worldMatrix)	(*viewMatrix)		(*projectionMatrix)
	결과적으로 물체를 원근감에 맞게 가상세계(3d)에 오브젝트를 배치하여 렌더링 하게 함
*/



cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType TextureVertexShader(VertexInputType input)
{
    PixelInputType output;
    

	// 적절한 행렬 계산을 위해 위치 벡터를 4 단위로 변경합니다.
    input.position.w = 1.0f;

	// 월드, 뷰 및 투영 행렬에 대한 정점의 위치를 ​​계산합니다.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// 픽셀 쉐이더의 텍스처 좌표를 저장한다.
    output.tex = input.tex;
    
    return output;
}