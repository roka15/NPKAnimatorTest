#include "Input.h"
#include "Time.h"
namespace core
{

	std::vector<Input::Key> Input::mKeys;
	Input::PushInfo Input::mBeforPush;
	float Input::mTime;
	std::queue<Input::PushInfo> Input::mKeyMessageQueue;
	int ASCII[(UINT)EKeyCode::MAX] =
	{
		'A','S','D','W','K','Z','C','N', VK_LEFT,VK_RIGHT,VK_UP,VK_DOWN,VK_LBUTTON,VK_RBUTTON,VK_SPACE,VK_SHIFT,
	    VK_ESCAPE,VK_RETURN,
	};
	
	void Input::Initialize()
	{
		for (UINT i = 0; i < (UINT)EKeyCode::MAX; i++)
		{
			Key keyInfo;
			keyInfo.keycode = (EKeyCode)i;
			keyInfo.state = EKeyState::NONE;
			keyInfo.bPressed = false;

			mKeys.push_back(keyInfo);
		}
	}
	void Input::Update()
	{
		mTime += yeram_client::Time::DeltaTime();

		for (UINT i = 0; i < (UINT)EKeyCode::MAX; i++)
		{
			
			if (GetAsyncKeyState(ASCII[i]) & 0x8000)
			{
				
				//이전 플게임에 눌려있었다.
				if (mKeys[i].bPressed)
				{
					mKeys[i].state = EKeyState::Pressed;
				}
				else
				{
					PushInfo push;
					push.keycode = (EKeyCode)i;
					push.Time = mTime;
					push.state = EKeyState::Down;
					mKeys[i].state = EKeyState::Down;
					mKeyMessageQueue.push(push);
					mKeys[i].Time = mTime;
				}
				mKeys[i].bPressed = true;
			}
			else if (GetAsyncKeyState(ASCII[i]) & 0x8001)//hold
			{

			}
			else //현재 안눌려있다.
			{
				if (mKeys[i].bPressed)
				{
					PushInfo push;
					push.keycode = (EKeyCode)i;
					push.Time = mTime;
					push.state = EKeyState::Up;
					mKeys[i].state = EKeyState::Up;
					mKeyMessageQueue.push(push);
				}
				else
				{
					mKeys[i].state = EKeyState::NONE;
				}

				mKeys[i].bPressed = false;
			}
		}
	}
	void Input::Render(HDC _hdc)
	{
	}
	void Input::Release()
	{
	}
}