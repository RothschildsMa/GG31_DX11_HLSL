#pragma once

#include "gameobject.h"
#include "explosion.h"

class HP : public GameObject
{
public:
	enum Type
	{
		EnemyHp,
		PlayerHp,
		//RockHp,
		TYPE_MAX
	};

private:
	static ID3D11Buffer*				m_VertexBuffer;

	static ID3D11VertexShader*			m_VertexShader;
	static ID3D11PixelShader*			m_PixelShader;
	static ID3D11InputLayout*			m_VertexLayout;

	//class TexAnimation* m_TexAnimation;

	int m_type;
	float m_HpRef;

	static Texture_List m_Texture[Type::TYPE_MAX];
	int m_Handle;
	int m_TexCount;
	int m_CountMax;
	int m_CountX;
	int m_CountY;
	int m_CurrentFrame;
	int m_UpdateFrame;
	float m_Width;
	float m_Height;

	void UpdateAnimation();

public:

	static void Load();
	static void Unload();

	
	void SetHp(float hp) { m_HpRef = hp; }

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetTextureTag(int handle, int countMax, int countX, int countY, int frame = 0, float width = 0.0f, float height = 0.0f);

};
