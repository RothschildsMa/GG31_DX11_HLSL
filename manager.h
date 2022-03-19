#pragma once
#include "singleton.h"

class Manager : public Singleton<Manager>
{
private:
	class Scene* m_Scene = nullptr;

public:

	void Create();
	void Destory();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	
	class Scene* GetScene() { return m_Scene; }

	template<typename T>
	void SetScene()
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}

		m_Scene = new T();
		m_Scene->Init();
	}

};

