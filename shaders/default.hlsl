#define M_PI 3.14159

/* vertex attributes go here to input to the vertex shader */
struct vs_in {
    float3 pos : POS;
    float3 color : COL;
};

/* outputs from vertex shader go here. can be interpolated to pixel shader */
struct vs_out {
    float4 pos : SV_POSITION; // required output of VS
    float3 color : COLOR;
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
  vs_out output = (vs_out)0; // zero the memory first
  output.color = input.color;

  matrix mvp = mul(projectionMatrix, mul(viewMatrix, modelMatrix));
  output.pos = mul(mvp, float4(input.pos, 1.0f));

  return output;
}

float4 ps_main(vs_out input) : SV_TARGET {
    return float4(input.color, 1.0f);
}