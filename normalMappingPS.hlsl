

#include "common.hlsl"


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

Texture2D g_NormalMap : register(t1);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	
	float3 normal = In.Normal.xyz;
	
	float3 localNormal = g_NormalMap.Sample(g_SamplerState, In.TexCoord).xyz;
	
	localNormal = localNormal * 2.0f - 1.0f;
	
	normal = In.Tangent * -localNormal.x + In.BiNormal * -localNormal.y + normal * localNormal.z;

	normal = normalize(normal);

	float  light = -dot(normal, Light.Direction.xyz); //明るさを計算
	//float  light = 0.5f - 0.5f * dot(normal, Light.Direction.xyz);//ハーフランバートによる明るさの計算

	//outDiffuse.rgb *= localNormal * 0.5f + 0.5f;
	outDiffuse.rgb *= (light * In.Diffuse.rgb);
	
	outDiffuse.a = In.Diffuse.a; //αに明るさは関係ないので別計算


	//スペキュラー(フォン鏡面反射)
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;//視線ベクトル
	eyev = normalize(eyev);
	float3 refv = reflect(Light.Direction.xyz, normal);//光の反射ベクトル
	refv = normalize(refv);
	float specular = -dot(eyev, refv);	//鏡面反射の計算
	specular = saturate(specular);		//値をサチュレート



	specular = pow(specular, 30);		//30乗
	outDiffuse.rgb += specular;

	
}



