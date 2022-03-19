#pragma once

#include <string>
#include "gameobject.h"

typedef struct
{
	int handle;
	std::string fileName;
	ID3D11ShaderResourceView*	pTexture = nullptr;

}Texture_List;

class Explosion : public GameObject
{
public:
	enum TYPE
	{
		TYPE_NONE = -1,
		TYPE_EXP00,
		TYPE_EXP01,
		TYPE_EXP02,
		TYPE_EXP03,
		TYPE_EXP04,

		TYPE_MAX
	};
private:
	static ID3D11Buffer*				m_VertexBuffer;
	static ID3D11Buffer*				m_IndexBuffer;
	
	static ID3D11VertexShader*			m_VertexShader;
	static ID3D11PixelShader*			m_PixelShader;
	static ID3D11InputLayout*			m_VertexLayout;

	static Texture_List m_Texture[TYPE::TYPE_MAX];
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

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetTextureTag(int handle, int countMax, int countX, int countY, int frame = 0, float width = 0.0f, float height = 0.0f);
	

};
