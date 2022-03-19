
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	Out.Position = mul(In.Position, wvp);

	//光源計算
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0);	//normal.wは0にする(平行移動はしない)
	worldNormal = mul(normal, World);		//ワールド変換
	worldNormal = normalize(worldNormal);	//回転後の法線を正規化する
	Out.Normal = worldNormal;				//回転後の法線出力 In.Normalでなく回転後の法線を出力

	//明るさの計算
	//光ベクトルと法線 XYZ要素のみで計算
	//float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	//light = saturate(light);

	
	Out.Diffuse = In.Diffuse;
	
	
	Out.TexCoord = In.TexCoord;

	//ワールド変換した頂点座標を出力
	Out.WorldPosition = mul(In.Position, World);

}

