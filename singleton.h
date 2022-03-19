//=================================================================================================
//
//	シングルトンテンプレートクラス[singleton.h]
//																			author : Ma
//																			date : 2021/07/24
//--------------------------------------------------------------------------------------------------
//	Design Patttern : Singleton
//==================================================================================================
#pragma once

template<class T>
class Singleton
{
private:
	Singleton(const Singleton& obj) {}
	Singleton& operator=(const Singleton& obj) {}
	static T* m_Instance;
	
protected:
	Singleton() {}
	virtual ~Singleton() {}

public:
	
	static void CreateInstance()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new T;
		}

	}

	static T& GetInstance()
	{
		return *m_Instance;
	}

	static void DestroyInstance()
	{
		delete m_Instance;
		m_Instance = nullptr;
	}

};

template <typename T> T* Singleton<T>::m_Instance = nullptr;
