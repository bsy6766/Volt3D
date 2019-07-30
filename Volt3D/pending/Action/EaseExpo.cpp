#include <PreCompiled.h>

#include "EaseExpo.h"

//
// =========================== EXPO IN ================================
//

v3d::EaseExpoIn::EaseExpoIn()
	: v3d::Ease()
{}

v3d::EaseExpoIn::~EaseExpoIn() {}

v3d::EaseExpoIn * v3d::EaseExpoIn::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseExpoIn();

	if (newEase)
	{
		if (newEase->init(action))
		{
			return newEase;
		}
		else
		{
			delete newEase;
			return nullptr;
		}
	}
	else
	{
		return false;
	}
}

float v3d::EaseExpoIn::getT() const
{
	//return duration * powf(2.0f, 10.0f * ((elapsedTime / duration) - 1.0f));
	return powf(2.0f, 10.0f * ((elapsedTime / duration) - 1.0f));
}




//
// =========================== EXPO OUT ===============================
//

v3d::EaseExpoOut::EaseExpoOut()
	: v3d::Ease()
{}

v3d::EaseExpoOut::~EaseExpoOut() {}

v3d::EaseExpoOut * v3d::EaseExpoOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseExpoOut();

	if (newEase)
	{
		if (newEase->init(action))
		{
			return newEase;
		}
		else
		{
			delete newEase;
			return nullptr;
		}
	}
	else
	{
		return false;
	}
}

float v3d::EaseExpoOut::getT() const 
{
	//return duration * (-powf(2.0f, -10.0f * (elapsedTime / duration)) + 1.0f);
	return (-powf(2.0f, -10.0f * (elapsedTime / duration)) + 1.0f);
}




//
// ========================= EXPO IN OUT ===============================
//

v3d::EaseExpoInOut::EaseExpoInOut()
	: v3d::Ease()
{}

v3d::EaseExpoInOut::~EaseExpoInOut() {}

v3d::EaseExpoInOut * v3d::EaseExpoInOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseExpoInOut();

	if (newEase)
	{
		if (newEase->init(action))
		{
			return newEase;
		}
		else
		{
			delete newEase;
			return nullptr;
		}
	}
	else
	{
		return false;
	}
}

float v3d::EaseExpoInOut::getT() const
{
	float t = (elapsedTime / (duration * 0.5f));

	if (t < 1.0f)
	{
		//return (duration * 0.5f) * powf(2.0f, 10.0f * (t - 1.0f));
		return (0.5f) * powf(2.0f, 10.0f * (t - 1.0f));
	}
	else
	{
		t -= 1.0f;

		//return (duration * 0.5f) * (-powf(2.0f, -10.0f * t) + 2.0f);
		return (0.5f) * (-powf(2.0f, -10.0f * t) + 2.0f);
	}
}