
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "spotLight.h"
#include "mouseEvent.h"

#include "random.h"
#include "gameobject.h"
#include "camera.h"
#include "skybox.h"
#include "meshField.h"
#include "field.h"
#include "model.h"
#include "hp.h"
#include "bullet.h"
#include "enemyBullet.h"
#include "player.h"
#include "enemy.h"
#include "tree.h"
#include "explosion.h"
#include "sprite2D.h"
#include "fade.h"
#include "shadowDepthView.h"
#include "renderTexture.h"
#include "shadow.h"
#include "audio.h"
#include "title.h"
#include "result.h"
#include "game.h"
#include "fade.h"
#include "sphere.h"
#include "airplane.h"
#include "shaderSample.h"


void Game::Load()
{
	

}

void Game::Unload()
{
}

void Game::Init()
{
	//Player::Load();
	SphereDebugDraw::Load();
	//Enemy::Load();
	//Bullet::Load();
	//EnemyBullet::Load();
	//Explosion::Load();
	//Tree::Load();
	//HP::Load();
	//Shadow::Load();

	Fade::GetInstance().Fade_Start(FadeMode::FADE_IN, 0.0f, 0.0f, 0.0f, 60);

	AddGameObject<DefaultCamera>(0);
	//AddGameObject<SpotLight>(0);
	AddGameObject<Skybox>(1);
	//AddGameObject<Field>(1);
	AddGameObject<MeshField>(1);
	AddGameObject<ShaderSample>(1);

	AddGameObject<ShadowDepthView>(2);
	AddGameObject<RenderTexture>(2);
	/*Player* p1 = AddGameObject<Airplane>(1);
	p1->SetPosition({ 3.0f,1.0f,3.0f });
	Player* p2 = AddGameObject<Airplane>(1);
	p2->SetPosition({ -3.0f,1.0f,3.0f });
	Player* p3 = AddGameObject<Airplane>(1);
	AddGameObject<SphereDebugDraw>(1);

	for (int i = 0; i < Random::GetRandom(3, 5); i++)
	{
		Enemy* enemy = AddGameObject<Enemy>(1);

		D3DXVECTOR3 pos = Random::GetRandom(pos);

		enemy->SetPosition({ pos.x * 20, 0.0f, pos.z * 20 });
	}
	for (int i = 0; i < Random::GetRandom(8, 10); i++)
	{
		Tree* tree = AddGameObject<Tree>(1);

		D3DXVECTOR3 pos = Random::GetRandom(pos);

		tree->SetPosition({ pos.x * -30, 0.0f, pos.z * 40 });
		Shadow* shadow;
		shadow = AddGameObject<Shadow>(1);
		shadow->SetPosition(tree->GetPosition());
		shadow->SetScale({ 5.0f, 1.0f, 5.0f });
	}
	
	Audio *bgm = AddGameObject<Audio>(2);
	bgm->Load("asset\\audio\\01.wav");
	bgm->Play(true);*/

}

void Game::Uninit()
{
	Scene::Uninit(); //åpè≥
	
	//Unload();
	//Bullet::Unload(); //staticÉÅÉ\ÉbÉh
	//EnemyBullet::Unload();
	//Enemy::Unload();
	//Explosion::Unload();
	//Tree::Unload();
	//HP::Unload();
	//Shadow::Unload();
	//SphereDebugDraw::Unload();
	//Player::Unload();

	SphereDebugDraw::Unload();
}

void Game::Update()
{
	Scene::Update();

	MouseEvent::GetInstance().Update();

	/*std::vector<Enemy*> enemyList = GetGameObjects<Enemy>(1);
	std::vector<Airplane*> playerList = GetGameObjects<Airplane>(1);
	if (enemyList.size() == 0)
	{
		Resault::SetFlag(false);
		Manager::GetInstance().SetScene<Resault>();
	}

	if (playerList.size() == 0)
	{
		Resault::SetFlag(true);
		Manager::GetInstance().SetScene<Resault>();
	}*/

	
}
