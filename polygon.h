#pragma once


class CPolygon
{
private:

	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	//ここに	シェーダー関連の変数を追加



public:
	void Init();
	void Uninit();
	void Update();
	void Draw();


};