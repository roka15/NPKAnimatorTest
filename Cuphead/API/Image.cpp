#include "Image.h"
#include "Application.h"
#include "Resources.h"
extern yeram_client::Application application;
namespace yeram_client
{
	Image* Image::Create(const std::wstring& _key, UINT _width, UINT _height, COLORREF rgb)
	{
		if (_width == 0 || _height == 0)
			return nullptr;

		Image* image = Resources::Find<Image>(_key);
		if (image != nullptr)
			return image;

		image = new Image();
		HDC MainHDC = application.GetHDC();

		image->mBitmap = CreateCompatibleBitmap(MainHDC, _width, _height);
		image->mHdc = CreateCompatibleDC(MainHDC);

		HBITMAP oldBitmap = (HBITMAP)SelectObject(image->mHdc, image->mBitmap);
		DeleteObject(oldBitmap);
		image->mWidth = _width;
		image->mHeight = _height;

		image->SetKey(_key);
		image->SetTime(Resources::GetTime());
		Resources::Insert(_key, image);

		HBRUSH myBrush = (HBRUSH)CreateSolidBrush(rgb);
		HBRUSH oldBrush = (HBRUSH)SelectObject(image->mHdc, myBrush);

		Rectangle(image->mHdc, -1, -1, _width + 1, _height + 1);

		DeleteObject(SelectObject(image->mHdc, oldBrush));


		return image;
	}

	Image* Image::Create(const std::wstring& _key, UINT _width, UINT _height, BYTE* _bin_data)
	{
		if (_width == 0 || _height == 0)
			return nullptr;

		Image* image = Resources::Find<Image>(_key);
		if (image != nullptr)
			return image;

		image = new Image();
		HDC MainHDC = application.GetHDC();

		/*
		roka::file::FileSystem file;
		roka::file::FileInfo* info=new roka::file::FileInfo();
		info->name = "test";
		info->buffer = (char*)_bin_data;
		info->length = 54 + _width * _height * 3 + _height;
		file.SaveFile("..\\Resources",info);

		BYTE* tempBuffer = new BYTE[_width * _height * 3];
		int index_ = 0;
		int temp_point = 54;
		for (int h = 0; h < _height; ++h)
		{
			for (int w = 0; w < _width; ++w)
			{
				tempBuffer[index_++] = _bin_data[temp_point];
			}
			temp_point++;
		}
		*/

		BITMAPINFO bmpInfo;

		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfo.bmiHeader.biWidth = _width;
		bmpInfo.bmiHeader.biHeight = _height;
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = 24;
		bmpInfo.bmiHeader.biCompression = BI_RGB;
		bmpInfo.bmiHeader.biSizeImage = _height * ((_width * 24 + 31) / 32) * 4;
		//bmpInfo.bmiHeader.biXPelsPerMeter = 0;
		//bmpInfo.bmiHeader.biYPelsPerMeter = 0;
		//bmpInfo.bmiHeader.biClrUsed = 0;
		//bmpInfo.bmiHeader.biClrImportant = 0;



		BYTE* point = _bin_data;
		point += 10;
		int byteOffset = 0;
		memcpy(&byteOffset, point, sizeof(int));
		point = _bin_data;
		point += byteOffset;
		//point = tempBuffer;


		BYTE* bits;
		
		int length24 = _width * _height * 3;
		int length32 = _width * _height * 4;
		
		image->mHdc = CreateCompatibleDC(MainHDC);
		image->mBitmap = CreateDIBSection(image->mHdc, (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS, (void**)&bits, NULL, 0);
		BITMAPINFO test = {};

		memset(bits, 255, bmpInfo.bmiHeader.biSizeImage);

		int index = 0;
		for (int i = 0; i < bmpInfo.bmiHeader.biSizeImage-2; i += 3)
		{
			if (point[i] == 0 && point[i + 1] == 0 && point[i + 2] == 0)
			{
				bits[i + 0] = 255;
				bits[i + 1] = 0;
				bits[i + 2] = 255;
			}
			else
			{
				bits[i + 0] = point[i + 0];
				bits[i + 1] = point[i + 1];
				bits[i + 2] = point[i + 2];
			}
		}

		//memcpy(bits, point, length);

		HBITMAP oldBitmap = (HBITMAP)SelectObject(image->mHdc, image->mBitmap);
		DeleteObject(oldBitmap);
		image->mWidth = _width;
		image->mHeight = _height;

		image->SetKey(_key);
		image->SetTime(Resources::GetTime());
		Resources::Insert(_key, image);

		//delete tempBuffer;
		return image;
	}

	Image::Image()
		:mBitmap(NULL)
		, mHdc(NULL)
		, mWidth(0)
		, mHeight(0)
	{

	}
	Image::~Image()
	{

	}
	COLORREF Image::GetPixel(int x, int y)const
	{
		return ::GetPixel(mHdc, x, y);
	}
	void Image::SetPixel(int x, int y, COLORREF _color)const
	{
		::SetPixel(mHdc, x, y, _color);
	}
	HRESULT Image::Load(const std::wstring& _path)
	{
		//null,이미지경로,어떻게 이미지를 불러올것인가,얼마나 가져올것인가(다 가져올거라 0 0 ) , 로드 방식 
		mBitmap = (HBITMAP)LoadImageW(nullptr, _path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if (mBitmap == nullptr)
		{
			return E_FAIL;
		}
		//mBitmap 에 불러와진 image 정보 불러와서 bitinfo에 넣음.
		BITMAP bitInfo = {};
		GetObject(mBitmap, sizeof(BITMAP), &bitInfo);

		mWidth = bitInfo.bmWidth;
		mHeight = bitInfo.bmHeight;

		HDC mainDC = application.GetHDC();
		mHdc = CreateCompatibleDC(mainDC);

		HBITMAP oldBitmap = (HBITMAP)SelectObject(mHdc, mBitmap);
		DeleteObject(oldBitmap);
		return S_OK;
	}
}
