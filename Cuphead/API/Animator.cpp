#include "Animator.h"
#include "Resources.h"
#include "Input.h"
#include "Application.h"
extern yeram_client::Application application;
namespace yeram_client
{
	Animator::Animator()
		: Component(EComponentType::Animator)
		, mActiveAnimation(nullptr)
		, mSpriteSheet(nullptr)
		, mbLoop(false)
		, mbStop(false)
	{

	}
	Animator::~Animator()
	{
		for (auto animation : mAnimations)
		{
			delete animation.second;
			animation.second = nullptr;
		}
		for (auto events : mEvents)
		{
			delete events.second;
			events.second = nullptr;
		}
	}
	void Animator::Initialize()
	{
	}
	void Animator::Update()
	{
		
		if (GetActive() == false)
			return;
		if (mActiveAnimation == nullptr)
			return;
		if (core::Input::GetKeyDown(core::EKeyCode::SPACE))
			mbStop = !mbStop;
		if (mbStop == true)
			return;

		mActiveAnimation->Update();
	
		if (mActiveAnimation->IsComplete())
		{
			Animator::Events* events
				= FindEvents(mActiveAnimation->GetName());

			if (events != nullptr)
				events->mCompleteEvent();
			if (mbLoop == true)
				mActiveAnimation->Reset();
		}

	}
	void Animator::Render(HDC hdc)
	{
		if (GetActive() == false)
			return;
		if (mActiveAnimation == nullptr)
			return;
		
		mActiveAnimation->Render(hdc);
	}
	void Animator::Release()
	{
	}
	void Animator::CreateAnimation(const std::wstring& name
		, Image* sheet, Vector2 leftTop
		, UINT coulmn, UINT row, UINT spriteLength
		, Vector2 offset, float duration)
	{
		Animation* animation = FindAnimation(name);

		if (animation != nullptr)
			return;

		animation = new Animation();
		animation->Create(sheet, leftTop, coulmn, row, spriteLength, offset, duration);
		animation->SetName(name);
		animation->SetAnimator(this);

		mAnimations.insert(std::make_pair(name, animation));
		Events* event = new Events();
		mEvents.insert(std::make_pair(name, event));
	}

	void Animator::CreateAnimations(const std::wstring& path, Vector2 offset, float duration)
	{
		UINT width = 0;
		UINT height = 0;
		UINT fileCount = 0;


		std::filesystem::path fs(path);
		std::vector<Image*> images = {};
		for (const auto& p : std::filesystem::recursive_directory_iterator(path))
		{
			std::wstring fileName = p.path().filename();
			std::wstring fullName = path + L"\\" + fileName;

			const std::wstring ext = p.path().extension();
			if (ext == L".png")
				continue;

			Image* image = Resources::Load<Image>(fileName, fullName);
			images.push_back(image);

			if (width < image->GetWidth())
			{
				width = image->GetWidth();
			}
			if (height < image->GetHeight())
			{
				height = image->GetHeight();
			}
			fileCount++;
		}

		std::wstring key = fs.parent_path().filename();
		key += fs.filename();
		mSpriteSheet = Image::Create(key, width * fileCount, height);

		//
		int index = 0;
		for (Image* image : images)
		{
			int centerX = (width - image->GetWidth()) / 2;
			int centerY = (height - image->GetHeight());

			BitBlt(mSpriteSheet->GetHDC()
				, width * index + centerX
				, 0 + centerY
				, image->GetWidth(), image->GetHeight()
				, image->GetHDC(), 0, 0, SRCCOPY);

			index++;
		}

		CreateAnimation(key, mSpriteSheet, Vector2::Zero, index, 1, index, offset, duration);
	}

	void Animator::CreateAnimations(const std::wstring& aniName,
		const roka::file::CSVInfo* const csvInfo,
		const roka::file::PackInfo* const packInfo,
		Vector2 offset, float duration,
		UINT startIndex, UINT endIndex)
	{
		UINT width = 0;
		UINT height = 0;
		UINT fileCount = 0;

		std::wstring fileName = s2ws(csvInfo->name);
		std::wstring ResultKey = fileName + aniName;
		std::pair canvas_size = csvInfo->base_size;
		int imageCount = endIndex == startIndex? packInfo->binbuf.size():endIndex-startIndex;
		mSpriteSheet = Image::Create(ResultKey, canvas_size.first * imageCount, canvas_size.second);

		std::vector<Image*> images = {};
		int start = 0;
		int end = 0;
		if (startIndex == endIndex)
		{
			end = imageCount;
		}
		else
		{
			start = startIndex;
			end = endIndex;
		}

		for (int i=start;i< end;i++)
		{
			char* buf = packInfo->binbuf[i]->buffer;
			std::wstring imageName = fileName + (s2ws(packInfo->binbuf[i]->name));
			Image* image 
				= Image::Create(imageName, csvInfo->size[i].first, csvInfo->size[i].second, (BYTE*)packInfo->binbuf[i]->buffer);
			
			images.push_back(image);
		}
		//
		int index = 0;
		for (int i=0;i<imageCount;i++)
		{
			int centerX = csvInfo->pos[i].first;
			int centerY = csvInfo->pos[i].second;

			Vector2 imagesize = Vector2{ (float)images[i]->GetWidth(),(float)images[i]->GetHeight() };
			BitBlt(mSpriteSheet->GetHDC()
				, canvas_size.first * i + centerX
				, 0 + centerY
				, images[i]->GetWidth(), images[i]->GetHeight()
				, images[i]->GetHDC(), 0, 0, SRCCOPY);
		}

		CreateAnimation(ResultKey, mSpriteSheet, Vector2::Zero, imageCount, 1, imageCount, offset, duration);
	}

	Animation* Animator::FindAnimation(const std::wstring& name)
	{
		std::map<std::wstring, Animation*>::iterator iter
			= mAnimations.find(name);

		if (iter == mAnimations.end())
			return nullptr;

		return iter->second;
	}
	void Animator::Play(const std::wstring& name, bool loop)
	{
		if (mActiveAnimation != nullptr)
		{
			Animator::Events* prevEvents
				= FindEvents(mActiveAnimation->GetName());

			if (prevEvents != nullptr)
				prevEvents->mEndEvent();
		}

		mActiveAnimation = FindAnimation(name);
		mActiveAnimation->Reset();
		mbLoop = loop;

		Animator::Events* events
			= FindEvents(mActiveAnimation->GetName());

		if (events != nullptr)
			events->mStartEvent();

	}
	void Animator::Play(const std::wstring& name, bool loop, UINT startIndex, UINT endIndex)
	{
		if (mActiveAnimation != nullptr)
		{
			Animator::Events* prevEvents
				= FindEvents(mActiveAnimation->GetName());

			if (prevEvents != nullptr)
				prevEvents->mEndEvent();
		}

		mActiveAnimation = FindAnimation(name);
		mActiveAnimation->Reset();
		mActiveAnimation->SetRangeIndex(startIndex, endIndex);
		mbLoop = loop;

		Animator::Events* events
			= FindEvents(mActiveAnimation->GetName());

		if (events != nullptr)
			events->mStartEvent();

	}
	Animator::Events* Animator::FindEvents(const std::wstring& name)
	{
		std::map<std::wstring, Events*>::iterator iter
			= mEvents.find(name);

		if (iter == mEvents.end())
			return nullptr;

		return iter->second;
	}
	std::function<void()>& Animator::GetStartEvent(const std::wstring& name)
	{
		Animation* animation = FindAnimation(name);

		Animator::Events* events
			= FindEvents(animation->GetName());

		return events->mStartEvent.mEvent;
	}
	std::function<void()>& Animator::GetCompleteEvent(const std::wstring& name)
	{
		Animation* animation = FindAnimation(name);

		Animator::Events* events
			= FindEvents(animation->GetName());

		return events->mCompleteEvent.mEvent;
	}
	std::function<void()>& Animator::GetEndEvent(const std::wstring& name)
	{
		Animation* animation = FindAnimation(name);

		Animator::Events* events
			= FindEvents(animation->GetName());

		return events->mEndEvent.mEvent;
	}
}