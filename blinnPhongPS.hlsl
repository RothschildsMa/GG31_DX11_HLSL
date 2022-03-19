

#include "common.hlsl"


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normal = normalize(In.Normal); //ピクセルの法線を正規化
	//float  light = -dot(normal.xyz, Light.Direction.xyz); //明るさを計算
	float  light = 0.5f - 0.5f * dot(normal.xyz, Light.Direction.xyz);//ハーフランバートによる明るさの計算

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= (light * In.Diffuse.rgb);
	outDiffuse.a *= In.Diffuse.a; //αに明るさは関係ないので別計算


	//スペキュラー(ブリンフォン)Blinn-Phong
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;//視線ベクトル
	eyev = normalize(eyev);
	float3 halfv = eyev + Light.Direction.xyz;//ハーフベクトル
	halfv = normalize(halfv);
	float specular = -dot(halfv, normal.xyz);//内積を計算
	specular = saturate(specular);

	specular = pow(specular, 30);		//30乗
	outDiffuse.rgb += specular;



}

