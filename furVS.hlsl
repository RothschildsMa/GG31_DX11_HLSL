
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp; //ワールドビュープロジェクション行列
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    float4 worldNormal, normal;
    normal = float4(In.Normal.xyz, 0.0); //法線をコピー（ｗ＝０にする）
    worldNormal = mul(normal, World); //法線をワールド変換しておく
    worldNormal = normalize(worldNormal);
    Out.Normal = worldNormal;

    //頂点を法線ベクトル*描画回数*0.02だけ移動させてモデルを拡大する
    In.Position += normal * Parameter.x * 0.02; //0.02は適当なスケール値
    Out.Position = mul(In.Position, wvp); //WVP行列を乗算する
    
    //光源計算 ハーフランバート
    float light = 0.5 - 0.5 * dot(Light.Direction.xyz, worldNormal.xyz);
    Out.Diffuse = light;
    Out.Diffuse.a = In.Diffuse.a;
    
    //テクスチャ座標
    Out.TexCoord = In.TexCoord;



}