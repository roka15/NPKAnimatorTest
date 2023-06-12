#pragma once
#include "Skill.h"
namespace yeram_client
{
	class Arsmagna :
		public Skill
	{
	public:
		virtual void SetAnimations()override;
		virtual void Update()override;
		virtual void Render(HDC _hdc)override;
		virtual void Play()override;
		
	private:

	};
}

