#pragma once
#include "My_Resource.h"
namespace yeram_client
{
	
	class Image:public My_Resource
	{
	public:
		static Image* Create(const std::wstring& _key,UINT _width,UINT _height, COLORREF rgb = RGB(255, 0, 255));
		static Image* Create(const std::wstring& _key, UINT _width, UINT _height, BYTE* _bin_data);
		Image();
		~Image();
		
		COLORREF GetPixel(int x, int y)const;
		void SetPixel(int x, int y, COLORREF _color)const;
		virtual HRESULT Load(const std::wstring& _path) override;
		HDC GetHDC()const { return mHdc; }
		HBITMAP GetBitmap()const { return mBitmap; }
		UINT GetWidth()const { return mWidth; }
		UINT GetHeight()const { return mHeight; }
		
	private:
		HBITMAP mBitmap;
		HDC mHdc;
		int mWidth;
		int mHeight;
	};

}


