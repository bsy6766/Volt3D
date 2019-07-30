#include <PreCompiled.h>

#include "EaseCubic.h"

//
// =========================== CUBIC IN ================================
//

v3d::EaseCubicIn::EaseCubicIn()
	: v3d::Ease()
{}

v3d::EaseCubicIn::~EaseCubicIn() {}

v3d::EaseCubicIn * v3d::EaseCubicIn::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseCubicIn();

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

float v3d::EaseCubicIn::getT() const
{
	float t = elapsedTime / duration;
	//return duration * t * t * t;
	return t * t * t;
}




//
// =========================== CUBIC OUT ===============================
//

v3d::EaseCubicOut::EaseCubicOut()
	: v3d::Ease()
{}

v3d::EaseCubicOut::~EaseCubicOut() {}

v3d::EaseCubicOut * v3d::EaseCubicOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseCubicOut();

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

float v3d::EaseCubicOut::getT() const 
{
	float t = elapsedTime / duration;
	t--;
	//return duration * ((t*t*t + 1.0f));
	return ((t*t*t + 1.0f));
}




//
// ========================= CUBIC IN OUT ===============================
//

v3d::EaseCubicInOut::EaseCubicInOut()
	: v3d::Ease()
{}

v3d::EaseCubicInOut::~EaseCubicInOut() {}

v3d::EaseCubicInOut * v3d::EaseCubicInOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseCubicInOut();

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

float v3d::EaseCubicInOut::getT() const
{
	float t = elapsedTime;

	if ((t /= duration * 0.5f) < 1.0f)
	{
		//return (duration * 0.5f) * (t * t * t);
		return (0.5f) * (t * t * t);
	}
	else
	{
		t -= 2.0f;
		//return (duration * 0.5f) * ((t * t * t) + 2.0f);
		return (0.5f) * ((t * t * t) + 2.0f);
	}
}