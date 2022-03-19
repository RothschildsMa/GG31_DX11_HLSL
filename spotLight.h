#pragma once

#include "renderer.h"
#include "gameobject.h"

class SpotLight : public GameObject
{
private:
	LIGHT m_Light;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};

