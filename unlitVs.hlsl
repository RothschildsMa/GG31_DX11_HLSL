
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
	//1頂点分のデータを受け取り、処理して、出力する
	//頂点変換処理 この処理は必ず必要
	matrix wvp;								//行列変数を作成
	wvp = mul(World, View);					//wvp = ワールド行列 * カメラ行列
	wvp = mul(wvp, Projection);				//wvp = wvp * プロジェクション行列
	Out.Position = mul(In.Position, wvp);	//変換結果を出力する

}

