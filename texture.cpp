
#include "texture.h"
#include "renderer.h"

void Texture::Init()
{
	//テクスチャ管理テーブルの初期化
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		m_Textures[i].pTexture = nullptr;
		m_Textures[i].width = 0;
		m_Textures[i].height = 0;
		m_Textures[i].filename[0] = 0;
	}

}

void Texture::Uninit()
{
	this->ReleaseAll();
}

int Texture::SetLoadFile(std::string filename)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		//テーブルが空いていたらスキップ
		if (m_Textures[i].filename[0] == 0)
		{
			continue;
		}
		//すでに指定のファイルが予約されているか?
		if (strcmp(filename.c_str(), m_Textures[i].filename.c_str()) == 0)
		{
			//予約されている
			return i;
		}
	}

	//新規予約
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		//テーブルが空いていたらスキップ
		if (m_Textures[i].filename[0] != 0)
		{
			continue;
		}

		m_Textures[i].filename = filename;
		
		return i;
	}

	return TEXTURE_INVALID_ID;
}

void Texture::Load()
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (m_Textures[i].filename[0] == 0)
		{
			//空のテーブルなのでスキップ
			continue;
		}
		if (m_Textures[i].pTexture != nullptr)
		{
			//すでに読み込まれている
			continue;
		}

	
		D3DX11CreateShaderResourceViewFromFile(
			Renderer::GetInstance().GetDevice(),
			m_Textures[i].filename.c_str(),
			NULL,
			NULL,
			&m_Textures[i].pTexture,
			NULL);

		//テクスチャの解像度（幅・高さ）の調査
		D3DXIMAGE_INFO info;
		D3DXGetImageInfoFromFile(m_Textures[i].filename.c_str(), &info);
		m_Textures[i].width = info.Width;
		m_Textures[i].height = info.Height;
		
		
	}

}

void Texture::SetReleaseFile(int textureIds[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (m_Textures[textureIds[i]].pTexture)
		{
			m_Textures[textureIds[i]].pTexture->Release();
			m_Textures[textureIds[i]].pTexture = nullptr;
		}

		m_Textures[textureIds[i]].filename[0] = 0;
		m_Textures[textureIds[i]].width = 0;
		m_Textures[textureIds[i]].height = 0;

	}
}

void Texture::ReleaseAll()
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (m_Textures[i].pTexture)
		{
			m_Textures[i].pTexture->Release();
			m_Textures[i].pTexture = nullptr;
		}
		m_Textures[i].filename[0] = 0;
		m_Textures[i].width = 0;
		m_Textures[i].height = 0;

	}
}
