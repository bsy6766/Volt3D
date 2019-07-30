#include <PreCompiled.h>

#include "EaseBounce.h"

//
// ========================== BOUNCE BASE ===============================
//

v3d::EaseBounceBase::EaseBounceBase()
	: v3d::Ease()
{}

v3d::EaseBounceBase::~EaseBounceBase() {}

float v3d::EaseBounceBase::getBounce(float t, const float d) const
{
	if ((t /= d) < (1 / 2.75f))
	{
		return (7.5625f * t * t);
	}
	else if (t < (2 / 2.75f))
	{
		float postFix = t -= (1.5f / 2.75f);
		return (7.5625f * (postFix) * t + 0.75f);
	}
	else if (t < (2.5 / 2.75))
	{
		float postFix = t -= (2.25f / 2.75f);
		return (7.5625f * (postFix) * t + 0.9375f);
	}
	else
	{
		float postFix = t -= (2.625f / 2.75f);
		return (7.5625f * (postFix) * t + 0.984375f);
	}
}




//
// =========================== BOUNCE IN ================================
//

v3d::EaseBounceIn::EaseBounceIn()
	: v3d::EaseBounceBase()
{}

v3d::EaseBounceIn::~EaseBounceIn() {}

v3d::EaseBounceIn * v3d::EaseBounceIn::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseBounceIn();

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

float v3d::EaseBounceIn::getT() const
{
	return 1.0f - getBounce(duration - elapsedTime, duration);
}




//
// =========================== BOUNCE OUT ===============================
//

v3d::EaseBounceOut::EaseBounceOut()
	: v3d::EaseBounceBase()
{}

v3d::EaseBounceOut::~EaseBounceOut() {}

v3d::EaseBounceOut * v3d::EaseBounceOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseBounceOut();

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

float v3d::EaseBounceOut::getT() const 
{
	return getBounce(elapsedTime, duration);
}




//
// ========================= BOUNCE IN OUT ===============================
//

v3d::EaseBounceInOut::EaseBounceInOut()
	: v3d::EaseBounceBase()
{}

v3d::EaseBounceInOut::~EaseBounceInOut() {}

v3d::EaseBounceInOut * v3d::EaseBounceInOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseBounceInOut();

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

float v3d::EaseBounceInOut::getT() const
{
	if (elapsedTime < (duration * 0.5f))
	{
		return (1.0f - getBounce(duration - (elapsedTime * 2.0f), duration)) * 0.5f;
	}
	else
	{
		return (getBounce((elapsedTime * 2.0f) - duration, duration) * 0.5f + 0.5f);
	}
}