

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

	//スペキュラー(フォン鏡面反射)
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;//視線ベクトル
	eyev = normalize(eyev);
	float3 refv = reflect(Light.Direction.xyz, normal.xyz);//光の反射ベクトル
	refv = normalize(refv);
	float specular = -dot(eyev, refv);	//鏡面反射の計算
	specular = saturate(specular);		//値をサチュレート

	specular = pow(specular, 30);		//30乗
	outDiffuse.rgb += specular;


	//リムライト
	float rim = dot(eyev, normal.xyz) + 1.0f;//直角時が1.0になる
	rim = saturate(rim);//サチュレート
	rim = pow(rim, 3) * 2.0f;//見た感じで値を決める

	outDiffuse.rgb += rim;
}

