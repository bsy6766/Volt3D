#include <PreCompiled.h>

#include "EaseBack.h"

//
// =========================== BACK IN ================================
//

v3d::EaseBackIn::EaseBackIn()
	: v3d::Ease()
{}

v3d::EaseBackIn::~EaseBackIn() {}

v3d::EaseBackIn * v3d::EaseBackIn::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseBackIn();

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

float v3d::EaseBackIn::getT() const
{
	const float s = 1.70158f;
	const float t = elapsedTime / duration;
	const float postFix = t;

	//return duration * (postFix) * t * ((s + 1.0f) * t - s);
	return (postFix) * t * ((s + 1.0f) * t - s);
}




//
// =========================== BACK OUT ===============================
//

v3d::EaseBackOut::EaseBackOut()
	: v3d::Ease()
{}

v3d::EaseBackOut::~EaseBackOut() {}

v3d::EaseBackOut * v3d::EaseBackOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseBackOut();

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

float v3d::EaseBackOut::getT() const 
{
	float s = 1.70158f;
	float t = elapsedTime;
	//return duration * ((t = t / duration - 1.0f) * t * ((s + 1.0f) * t + s) + 1.0f);
	return ((t = t / duration - 1.0f) * t * ((s + 1.0f) * t + s) + 1.0f);
}




//
// ========================= BACK IN OUT ===============================
//

v3d::EaseBackInOut::EaseBackInOut()
	: v3d::Ease()
{}

v3d::EaseBackInOut::~EaseBackInOut() {}

v3d::EaseBackInOut * v3d::EaseBackInOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseBackInOut();

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

float v3d::EaseBackInOut::getT() const
{
	float t = elapsedTime;
	float s = 1.70158f;

	if ((t /= (duration * 0.5f)) < 1.0f)
	{
		//return (duration * 0.5f) * (t * t *(((s *= (1.525f)) + 1.0f) * t - s));
		return (0.5f) * (t * t *(((s *= (1.525f)) + 1.0f) * t - s));
	}
	else
	{
		float postFix = t -= 2.0f;
		//return (duration * 0.5f) * ((postFix)* t * (((s *= (1.525f)) + 1.0f) * t + s) + 2.0f);
		return (0.5f) * ((postFix) * t * (((s *= (1.525f)) + 1.0f) * t + s) + 2.0f);
	}
}