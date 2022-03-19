#pragma once

#include"scene.h"

class Game : public Scene
{
private:


public:
	static void Load();
	static void Unload();

	void Init() override;
	void Uninit() override;
	void Update() override;
	//void Draw() override;
};