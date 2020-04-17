


cbuffer MatrixBuffer {
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer {
    float3 cameraPosition;
	float padding;
};



struct VertexInputType {
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType {
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};




PixelInputType LightVertexShader(VertexInputType input) {

    PixelInputType output;
    

    input.position.w = 1.0f;
    
    // 픽셀의 위치를 월드 좌표계, 뷰 좌표계, 투영 좌표계로 변환
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // 텍스쳐는 그대로 출력
	output.tex = input.tex;
    
    // 픽셀의 방향도 월드 좌표계로 변환
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // 방향 단위화(크기를 1로)
    output.normal = normalize(output.normal);

    
    // 월드 좌표계 기준으로 픽셀의 위치 계산
    float4 worldPosition = mul(input.position, worldMatrix);

    
    // 픽셀을 바라보는 방향을 계산(카메라 위치 - 픽셀의 위치)
    output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);
	

    return output;
}