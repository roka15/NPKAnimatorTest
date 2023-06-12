#pragma once
#include "Component.h"
namespace yeram_client
{
	class Audio :
		public Component
	{
	public:
		Audio();
		~Audio();
		virtual void Render(HDC hdc)override;
		void Play(std::wstring _key, std::wstring _path,bool _flag, int _volume=100);
	private:
		std::wstring mKey;
		std::wstring mPath;
	};
}


