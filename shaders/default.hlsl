#define M_PI 3.14159

/* vertex attributes go here to input to the vertex shader */
struct vs_in {
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
};

/* outputs from vertex shader go here. can be interpolated to pixel shader */
struct vs_out {
    float4 pos : SV_POSITION; // required output of VS
    float2 texCoord : TEXCOORD;
};

Texture2D tex : register(t0);
// SamplerState sampler : register(s0);
SamplerState MeshTextureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

cbuffer AppCB : register(b0) {
    matrix projectionMatrix;
}

cbuffer FrameCB : register(b1) {
    int time;
    matrix viewMatrix;
    matrix modelMatrix;
}

vs_out vs_main(vs_in input) {
  vs_out output; // zero the memory first
  output.texCoord = input.texCoord;

  matrix mvp = mul(projectionMatrix, mul(viewMatrix, modelMatrix));
  output.pos = mul(mvp, float4(input.pos, 1.0f));

  return output;
}

float4 ps_main(vs_out input) : SV_TARGET {
    return tex.Sample(MeshTextureSampler, input.texCoord);
//     return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
