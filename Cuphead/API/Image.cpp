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
		for (int i = 0; i < bmpInfo.bmiHeader.biSizeImage - 2; i += 3)
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

		//memcpy(bits, point, length24);

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
	bool Image::CheckPNG(BYTE* _bin_data)
	{
		const BYTE* point = _bin_data;
		bool flag = false;

		//png header 504E47 은 메모리에서 보면 png 로 표기됨(ascii).
		if (point[0] == 0x89 && point[1] == 0x50 && point[2] == 0x4E && point[3] == 0x47
			&& point[4] == 0x0D && point[5] == 0x0A && point[6] == 0x1A && point[7] == 0x0A)
			return true;

		return false;
	}
	BMPInfo* Image::PNG2BMP(BYTE* _bin_data)
	{
		//BMPInfo* info = new BMPInfo();
		//const BYTE* point = _bin_data;
		//point += 16;
		//int test = 1;
		//info->width = B2LEndianInt((void*)point);
		//point += sizeof(UINT);
		//info->height = B2LEndianInt((void*)point);
		//point += sizeof(UINT);
		//BYTE bitdepth = 0;
		//memcpy(&bitdepth, point, 1);
		//point += 1;
		//BYTE colorType = 0;
		//memcpy(&colorType, point, 1);
		//point += 2;
		//BYTE filter = 0;
		//memcpy(&filter, point, 1);

		//if (colorType == 2)
		//	info->channel = 3;
		//else if (colorType == 6)
		//	info->channel = 4;

		//char ChunkType[5] = {};
		////idat pos find
		//point = _bin_data + 8/*header size*/ + 21/*ihdr size*/ + 4/*crc size*/;
		//int chunklength = 0;
		//while (1)
		//{
		//	chunklength = B2LEndianInt((void*)point);
		//	point += sizeof(int);
		//	memcpy(ChunkType, point, 4);
		//	point += 4;

		//	if (strcmp(ChunkType, "IDAT") == 0)
		//	{
		//		break;
		//	}
		//	point += chunklength + 4/*crc size*/;
		//}
		//
		//
		//z_stream stream;
		//stream.zalloc = Z_NULL;
		//stream.zfree = Z_NULL;
		//stream.opaque = Z_NULL;

		//stream.avail_in = static_cast<uInt>(chunklength);
		//stream.next_in = const_cast<Bytef*>(point);


		//size_t bytesPerPixel = 0;
		//if (colorType == 0) {
		//	bytesPerPixel = bitdepth / 8;  // Grayscale
		//}
		//else if (colorType == 2) {
		//	bytesPerPixel = 3 * bitdepth / 8;  // RGB
		//}
		//else if (colorType == 3) {
		//	bytesPerPixel = bitdepth / 8;  // Palette (indexed color)
		//}
		//else if (colorType == 4) {
		//	bytesPerPixel = 2 * bitdepth / 8;  // Grayscale with alpha
		//}
		//else if (colorType == 6) {
		//	bytesPerPixel = 4 * bitdepth / 8;  // RGBA
		//}

		//int outsize = (info->width * bytesPerPixel+1) * info->height;
		//
		//info->data = new BYTE[outsize];
		//memset(info->data, 0, outsize);
		//stream.avail_out = static_cast<uInt>(outsize);
		//stream.next_out = info->data;

		//int ret = inflateInit(&stream);
		//if (ret != Z_OK)
		//{
		//	//err;
		//	return nullptr;
		//}

		////ret = uncompress(info->data, (uLongf*)&outsize, point, chunklength);
		//ret = inflate(&stream, Z_FINISH);
		//if (ret != Z_STREAM_END) {
		//	std::cerr << "Failed to decompress IDAT chunk: " << ret << std::endl;
		//	return nullptr;
		//}

		//inflateEnd(&stream);
		//return info;
		return nullptr;
	}

	void Image::BMPAlphaSet(BYTE* _bin_data, COLORREF alpha)
	{
	}
}
