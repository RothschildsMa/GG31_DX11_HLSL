#pragma once

#include "singleton.h"




class LightManager : public Singleton<LightManager>
{
private:
	LIGHT m_DirectionLight;
	
	XMFLOAT3 m_Focus;
	XMFLOAT3 m_Up;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Clear();

};





