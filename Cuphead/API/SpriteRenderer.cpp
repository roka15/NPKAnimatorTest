#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Application.h"
#include "Resources.h"
namespace yeram_client
{
	extern Application application;

	SpriteRenderer::SpriteRenderer()
		:Component(EComponentType::SpriteRenderer)
		, mRenderType(ERenderType::StretchBlt)
	{
		SetName(L"SpriteRenderer");
	}

	SpriteRenderer::~SpriteRenderer()
	{

	}

	void SpriteRenderer::Initialize()
	{
	}

	void SpriteRenderer::Update()
	{
	}

	void SpriteRenderer::Render(HDC hdc)
	{
		if (mbActive == false)
			return;
		Transform* transform = mOwner->GetComponent<Transform>();
		Vector2 pos = transform->GetPos();
		Vector2 obj_size = transform->GetSize();
		Vector2 scale = transform->GetScale();
		Vector2 size;

		size.x = obj_size.x == 0 ?mImageSize.x : obj_size.x;
		size.y = obj_size.y == 0 ? mImageSize.y : obj_size.y;

		pos.x -= (size.x*scale.x / 2);
		pos.y -= (size.y * scale.x);
		if (mImageKey.size() != 0)
		{
			Image* findImage = Resources::Find<Image>(mImageKey);
			if (findImage == nullptr)
			{
				findImage = Resources::Load<Image>(mImageKey, mPath);
			}	
			switch (mRenderType)
			{
			case ERenderType::BitBlt:
				BitBlt(hdc, pos.x, pos.y, size.x * scale.x, size.y * scale.y, findImage->GetHDC(), 0, 0, SRCCOPY);
				break;
			case ERenderType::TransParentBlt:
				TransparentBlt(hdc, pos.x, pos.y, size.x*scale.x, size.y*scale.y, findImage->GetHDC(), 0, 0, mImageSize.x, mImageSize.y, (RGB(255, 0, 255)));
				break;
			case ERenderType::StretchBlt:
				StretchBlt(hdc, pos.x, pos.y, size.x * scale.x, size.y * scale.y, findImage->GetHDC(), 0, 0, mImageSize.x, mImageSize.y, SRCCOPY);
				break;
			}
		}
	}

	void SpriteRenderer::Release()
	{
		/*const std::wstring name = mImage->GetKey();
		Resources::Remove(name);*/
	}

	void SpriteRenderer::SetImage(const std::wstring& _filename, const std::wstring& _path)
	{
		Image* image = Resources::Find<Image>(_filename);
		if (image == nullptr)
		{
			image=Resources::Load<Image>(_filename, _path);
			mImageKey = image->GetKey();
			mPath = _path;
		
			Resources::Insert<Image>(mImageKey, image);
			core::ResourceDeleterThread::RegisterResourceInfo(image);
		}
		Transform* tf = mOwner->GetComponent<Transform>();
		Vector2 pos = tf->GetPos();
		mImageKey = image->GetKey();
		mPath = _path;
		mImageSize = Vector2{(int)image->GetWidth(),(int)image->GetHeight()};
		pos.x -= mImageSize.x;
		pos.y -= mImageSize.y;
	}

	void SpriteRenderer::CreateImage(std::wstring& _key,std::wstring&_image_name, Vector2 _size, Vector2 _pos, Vector2 _canvas_size, BYTE* _bindata)
	{
		std::wstring imageName = _key;
		Image* canvase = Image::Create(imageName, _canvas_size.x, _canvas_size.y);
		std::wstring dummyName = _key + L"dummy";
		Image* image = Image::Create(dummyName, _size.x, _size.y, (BYTE*)_bindata);
		mImageKey = imageName;
		
		mImageSize.x = image->GetWidth();
		mImageSize.y = image->GetHeight();
		Vector2 imagesize = Vector2{ (float)image->GetWidth(),(float)image->GetHeight() };
		BitBlt(canvase->GetHDC()
			, _pos.x
			, _pos.y
			, image->GetWidth(), image->GetHeight()
			, image->GetHDC(), 0, 0, SRCCOPY);
	}

	const float& SpriteRenderer::GetWidth()
	{
		return mImageSize.x;
	}

	const float& SpriteRenderer::GetHeight()
	{
		return mImageSize.y;
	}

}
