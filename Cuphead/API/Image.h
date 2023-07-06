#pragma once
#include "My_Resource.h"
#ifdef _DEBUG
#pragma comment(lib,"..\\PngLib\\lib\\Debug\\Pnglib.lib")
#pragma comment(lib,"..\\PngLib\\lib\\Debug\\Zlib.lib")

#else
#pragma comment(lib,"..\\External\\PngLib\\lib\\Release\\Zlib.lib")
#pragma comment(lib,"..\\External\\PngLib\\lib\\Release\\Pnglib.lib")
#endif

#include "../PngLib/include/zlib.h"
#include "../PngLib/include/png.h"



namespace yeram_client
{
	struct BMPInfo
	{
		~BMPInfo() { delete data; }
		UINT width;
		UINT height;
		UINT channel;
		BYTE* data;
	};
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
		
		static bool CheckPNG(BYTE* _bin_data);
		static BMPInfo* PNG2BMP(BYTE* _bin_data);
		static void BMPAlphaSet(BYTE* _bin_data, COLORREF alpha = RGB(255, 0, 255));
	private:
		HBITMAP mBitmap;
		HDC mHdc;
		int mWidth;
		int mHeight;
	};

}


