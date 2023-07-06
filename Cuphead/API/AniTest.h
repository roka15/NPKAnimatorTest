#pragma once
#include "Skill.h"
namespace yeram_client
{
	class AniTest :public Skill
	{
	public:
		virtual void SetAnimations()override;
		virtual void Update()override;
		virtual void Render(HDC _hdc)override;
		virtual void Play()override;
	private:

	};
}

