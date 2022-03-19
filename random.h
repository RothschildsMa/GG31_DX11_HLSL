#pragma once

#include"main.h"

class Random
{
private:
	static unsigned int d;

public:

	static unsigned int FastNRand(void);		//整数の乱数取得
	static float FastFRand(void);				//浮動小数の乱数取得
	static int GetRandom(int a, int b);			//整数の乱数取得(範囲指定版a ～ b-1)
	static float GetRandom(float a, float b);	//浮動小数の乱数取得(範囲指定版)
	static D3DXVECTOR3& GetRandom(D3DXVECTOR3& out);	//ベクトルの乱数取得

};