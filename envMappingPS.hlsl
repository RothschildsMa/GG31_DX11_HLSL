

#include "common.hlsl"


Texture2D g_Texture : register(t0);
Texture2D g_TextureEnv : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	

	//１ ピクセルの法線を正規化
	float4 normal = normalize(In.Normal);

	//float  light = -dot(normal, Light.Direction.xyz); //明るさを計算

	//outDiffuse.rgb *= (light * In.Diffuse.rgb);


	//２ 視線ベクトルを作成して正規化
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev);
	//３ 反射ベクトルを作成して正規化
	float3 refv = reflect(eyev, normal.xyz);
	refv = normalize(refv);
	//４ 反射ベクトルから環境マッピング用のテクスチャ座標を作成
	float2 envTexCoord;
	envTexCoord.x = -refv.x * 0.3 + 0.5;
	envTexCoord.y = -refv.y * 0.3 + 0.5;
	//５ テクスチャのデータを取得後、スフィアマップとベーステクスチャを加算して出力
	float4 EnvTex = g_TextureEnv.SampleBias(g_SamplerState, envTexCoord, 0.0f);
	float4 BaseTex = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb = (1.0f * EnvTex.rgb) + (1.0f * BaseTex.rgb);
	//６ α値の処理 ベースのテクスチャのα値と引数のピクセル色のα値を乗算
	outDiffuse.a = BaseTex.a * In.Diffuse.a;
}

