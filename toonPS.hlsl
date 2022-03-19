

#include "common.hlsl"


Texture2D g_Texture : register(t0);
Texture2D g_TextureToon : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

	float4 normal = normalize(In.Normal);
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);//テクスチャを取得
	outDiffuse.a *= In.Diffuse.a; //α値の処理

	float light = -dot(normal.xyz, Light.Direction.xyz);


	/*if (light > 0.6f)
	{
		light = 1.0f;
	}
	else if (light > 0.4f)
	{
		light = 0.8f;
	}
	else
	{
		light = 0.6f;
	}

	outDiffuse.rgb *= saturate(In.Diffuse.rgb * light); //テクスチャ＊頂点色＊明るさ
	*/
	
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz; //視線ベクトルを作る
	eyev = normalize(eyev); //視線ベクトルを正規化する
	float d = dot(eyev, normal.xyz); //法線と視線ベクトルの内積

	if (d > -0.3) //dの値を判定
	{
		outDiffuse.rgb *= 0.2; //適当な閾値で明度を落とす
	}
	


	//テクスチャー版
	float2 uv = light;
	float3 col;
	uv = clamp(uv, 0.1f, 0.9);
	col = g_TextureToon.Sample(g_SamplerState, uv).xyz; //明るさを取得
	outDiffuse.rgb *= saturate(In.Diffuse.rgb * col); //テクスチャ＊頂点色＊明るさ


}

