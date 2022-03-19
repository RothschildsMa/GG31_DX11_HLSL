/*==============================================================================

	テクスチャ管理[texture.cpp]
													Author : 
													Date   :2021/08/16
-------------------------------------------------------------------------------

==============================================================================*/
#pragma once

#include <string>
#include "main.h"
#include "singleton.h"

#define TEXTURE_MAX (1000)
#define TEXTURE_INVALID_ID   (-1)

struct Texture_Data
{
	ID3D11ShaderResourceView* pTexture;
	unsigned long width;
	unsigned long height;
	std::string filename;

};

class Texture : public Singleton<Texture>
{
private:
	Texture_Data m_Textures[TEXTURE_MAX];

public:
	void Init();
	void Uninit();

	int SetLoadFile(std::string filename);
	void Load();
	void SetReleaseFile(int textureIds[], int count);
	void ReleaseAll();

	unsigned long GetWidth(int textureId) { return m_Textures[textureId].width; }
	unsigned long GetHeight(int textureId) { return m_Textures[textureId].height; }

	ID3D11ShaderResourceView* GetTexture(int textureId) 
	{
		if (textureId == TEXTURE_INVALID_ID)
		{
			return nullptr;
		}

		return m_Textures[textureId].pTexture;
	}


};


