
#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp; //ワールドビュープロジェクション行列作成
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    Out.Position = mul(In.Position, wvp); //頂点をWVP行列で変換して出力
    Out.TexCoord = In.TexCoord; //テクスチャ座標を出力
    Out.Diffuse = In.Diffuse; //頂点カラーを出力
    
}
