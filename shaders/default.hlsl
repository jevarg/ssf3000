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

cbuffer DXConstantBuffer {
    int time;
}

vs_out vs_main(vs_in input) {
  vs_out output = (vs_out)0; // zero the memory first
  output.color = input.color;

  float angle = (time / 100000.0f % 360.0f) * (M_PI / 180.0f);
//   float angle = 45 * (M_PI / 180.0f);
  float3x3 zRot = {
    {cos(angle), -sin(angle), 0},
    {sin(angle), cos(angle), 0},
    {0, 0, 1},
  };

  float3x3 yRot = {
    {cos(angle), 0, sin(angle)},
    {0, 1, 0},
    {-sin(angle), 0, cos(angle)},
  };


  output.pos = float4(mul(zRot, input.pos), 1.0);
//   output.pos = float4(input.pos, 1);

  return output;
}

float4 ps_main(vs_out input) : SV_TARGET {
    return float4(input.color, 1.0f);
}