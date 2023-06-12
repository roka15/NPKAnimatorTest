#pragma once
#include "Component.h"
namespace yeram_client
{
	class Image;
	class SpriteRenderer :public Component
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();
		virtual void Initialize()override;
		virtual void Update()override;
		virtual void Render(HDC hdc)override;
		virtual void Release()override;
		void SetImage(const std::wstring& _filename, const std::wstring& _path);
		void CreateImage(std::wstring& _key, std::wstring& _image_name, Vector2 _size,Vector2 _pos,Vector2 _canvas_size,BYTE* _bindata);
		void SetImageSize(Vector2& _size) { mImageSize = _size; }
		void SetRenderType(ERenderType _type)
		{
			mRenderType = _type;
		}
		const float& GetWidth();
		const float& GetHeight();
		std::wstring GetImageKey() { return mImageKey; }
		std::wstring GetPath() { return mPath; }
	private:
		ERenderType mRenderType;
		std::wstring mImageKey;
		std::wstring mPath;
		Vector2 mImageSize;
		Vector2 mImagePos;
		Vector2 mCanvasSize;
	};
}


