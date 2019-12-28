
#include "Share.fx"

cbuffer ColliderColor	: register(b13)
{
	float4	g_vColliderColor;
}

VS_OUTPUT_COLOR ColliderVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR	output = (VS_OUTPUT_COLOR)0;

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);
	output.vColor = g_vColliderColor;

	return output;
}

struct PS_OUTPUT_SINGLE
{
	float4	vColor	: SV_Target;
};

PS_OUTPUT_SINGLE ColliderPS(float4 vColor : COLOR)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor = vColor;
	output.vColor.a = 1.f;

	return output;
}