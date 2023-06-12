#pragma once
#include "CommonInclude.h"

namespace core
{
	enum class EKeyCode
	{
		A, S, D, W, K,Z,C,N,
		Left,
		Right,
		Up,
		Down,
		MouseLeft,
		MouseRight,
		SPACE,
		SHIFT,
		ESC,
		ENTER,
		MAX
	};

	enum class EKeyState
	{
		NONE,
		Down,
		Pressed,
		Up,
		Max,
	};
	class Input
	{
	public:
		struct Key
		{
			//키
			EKeyCode keycode;
			EKeyState state;
			//키 상태
			bool bPressed;
			float Time;
		};
		struct PushInfo
		{
			EKeyCode keycode;
			EKeyState state;
			float Time;
		};
		static void Initialize();
		static void Update();
		static void Render(HDC _hdc);
		static void Release();
		static EKeyCode GetBeforKeyCode() { return mBeforPush.keycode; }
		
		inline static EKeyState GetKeyState(EKeyCode _keycode)
		{
			return mKeys[(UINT)_keycode].state;
		}
		static __forceinline bool GetKey(EKeyCode _keycode)
		{
			if (mKeys[(UINT)_keycode].state == EKeyState::Pressed)
			{
				return true;
			}
			else return false;
		}
		static __forceinline bool GetKeyDown(EKeyCode _keycode)
		{
			if (mKeys[(UINT)_keycode].state == EKeyState::Down)
			{
				return true;
			}
			else return false;
		}
		static __forceinline bool GetKeyUp(EKeyCode _keycode)
		{
			if (mKeys[(UINT)_keycode].state == EKeyState::Up)
			{
				return true;
			}
			else return false;
		}
		static __forceinline bool GetKeyDoubleDown(EKeyCode _keycode, float _second)
		{
			float befor_time = mBeforPush.Time;
		
			if (mBeforPush.keycode == _keycode)
			{
				float diff = mTime - befor_time;
				mBeforPush.Time = 0.0f;
				if (mTime == befor_time)
					return false;
				else if (befor_time == 0.0f)
					return false;
				if (diff <= _second)
				{
					return true;
				}
			}
			return false;
		}
		static __forceinline bool KeyMessageQueueEmpty()
		{
			if (mKeyMessageQueue.empty() == true)
				return true;
			else return false;
		}
		static __forceinline PushInfo& GetQueueFront()
		{
			PushInfo& info = mKeyMessageQueue.front();
			
			return info;
		}
		static __forceinline void KeyMessageQueuePop()
		{
			mKeyMessageQueue.pop();
		}
		static __forceinline EKeyCode GetFirstPriorityKey(EKeyCode _code1, EKeyCode _code2)
		{
			EKeyCode key = mKeys[(UINT)_code1].Time > mKeys[(UINT)_code2].Time ? _code2 : _code1;
			return key;
		}
	private:
		static std::vector<Key> mKeys;
		static PushInfo mBeforPush;
		static float mTime;
		static std::queue<PushInfo> mKeyMessageQueue;
	};
}

