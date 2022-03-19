
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	//光源計算
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0);	//normal.wは0にする(平行移動はしない)
	worldNormal = mul(normal, World);		//ワールド変換
	worldNormal = normalize(worldNormal);	//回転後の法線を正規化する
	Out.Normal = worldNormal;				//回転後の法線出力 In.Normalでなく回転後の法線を出力



	Out.Diffuse = In.Diffuse;
	Out.Position = mul(In.Position, wvp);
	Out.TexCoord = In.TexCoord;

}

