
#include "common.hlsl"

//DirectXのテクスチャーの設定を受け継ぐ
Texture2D		g_Texture : register(t0);//テクスチャー0番
//DirectXのサンプラーステート設定を受け継ぐ
SamplerState	g_SamplerState : register(s0);//テクスチャーサンプラー0番

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	
	//このピクセルに使われるテクスチャーの色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

	//このピクセルのデフューズ色を乗算
	outDiffuse *= In.Diffuse;

	if (outDiffuse.a < 0.01)
	{
		discard;
	}

	////グレースケール化 Y = 0.299 * R + 0.587 * G + 0.114 * B
	//outDiffuse.rgb = (0.299 * outDiffuse.r) + (0.587 * outDiffuse.g) + (0.114 * outDiffuse.b);

	////セピア調変換
	//float4 sepia = float4(0.960, 0.784, 0.580, 1.0);
	//outDiffuse *= sepia;

	//ネガ反転
	//outDiffuse.rgb = 1.0f - outDiffuse.rgb;

	//名前不明処理
	//outDiffuse.rgb = pow(outDiffuse.rgb, 4);

	//float2 len = float2(0.5, 0.5);	//中心座標
	//len -= In.TexCoord.xy;			//中心と自ピクセルとのベクトル
	//len = 1.0 - length(len);		//ベクトルから距離を計算

	//outDiffuse.rgb *= pow(len.x, 2);

}
