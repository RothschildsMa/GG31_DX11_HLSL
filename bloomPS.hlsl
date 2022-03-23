
#include "common.hlsl"

Texture2D g_Texture : register( t0 );
SamplerState g_SamplerState : register( s0 );

//ブルーム 輝度が強すぎて光が周りに漏れる現象
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //普通にテクスチャを取得して処理対象のピクセルの色と乗算しておく
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse *= In.Diffuse; //頂点色と合成して出力
    float4 bloom = 0.0; //ブルーム処理の結果を格納する変数
    
    //画面サイズ X960 Y540とした場合
    for (int y = -12; y < 13; y++) //上下は±12ピクセルの範囲
    {
        for (int x = -3; x < 4; x++) //左右は±3ピクセルの範囲
        { 
            //式中の値の3を調節すると光の範囲が変化する
            float2 texCoord = In.TexCoord + float2(x * 3.0 / 960.0, y * 3.5 / 540.0); //テクスチャ座標をずらす
            
            float4 color = g_Texture.Sample(g_SamplerState, texCoord);
            
            //明度を計算
            float bright = color.r * 0.299 + color.g * 0.587 + color.b * 0.114;
            
            //明度が閾値0.7より大きい、かつ処理対象のピクセルでない
            if (bright > 0.7 && (x != 0 || y != 0))
            {
                bloom += color / (x * x + y * y); //距離によって減衰（ブルームに丸みもできる）
            }
        }
    }
    
    outDiffuse.rgb += bloom; //結果を加算




}