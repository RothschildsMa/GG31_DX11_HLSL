
#include "common.hlsl"

//DirectXのテクスチャーの設定を受け継ぐ
Texture2D g_Texture : register(t0); //テクスチャー0番
//DirectXのサンプラーステート設定を受け継ぐ
SamplerState g_SamplerState : register(s0); //テクスチャーサンプラー0番

float Rand(float2 coord)
{
    //frac(x)=xの小数部を返す 乱数は0.0 <= rand < 1.0の範囲
    return frac(sin(dot(coord.xy, float2(12.9898f, 78.233f))) * 43758.5453f);
}

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	
	//このピクセルに使われるテクスチャーの色を取得
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
	//このピクセルのデフューズ色を乗算
    outDiffuse *= In.Diffuse;
    
    //テクスチャデータ取得
    float4 Tex = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
    //テクスチャ座標とC言語からのパラメータ値をSeedにして乱数を作成
    float noise = clamp(0.2f, 0.5f, Rand(In.TexCoord * Parameter.y));
    
    //乱数値を輝度値としてテクスチャのデータへ加算する
    Tex.rgb += noise;

    //頂点（ピクセル）色と合成
    outDiffuse.rgb = saturate(Tex.rgb) * In.Diffuse.rgb;
    outDiffuse.a = Tex.a * In.Diffuse.a; //αは別処理
    
    
    //グレースケール化 Y = 0.299 * R + 0.587 * G + 0.114 * B
    outDiffuse.rgb = (0.299 * outDiffuse.r) + (0.587 * outDiffuse.g) + (0.114 * outDiffuse.b);
}
