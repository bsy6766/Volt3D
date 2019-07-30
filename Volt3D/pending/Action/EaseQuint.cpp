#include <PreCompiled.h>

#include "EaseQuint.h"

//
// =========================== QUINT IN ================================
//

v3d::EaseQuintIn::EaseQuintIn()
	: v3d::Ease()
{}

v3d::EaseQuintIn::~EaseQuintIn() {}

v3d::EaseQuintIn * v3d::EaseQuintIn::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseQuintIn();

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

float v3d::EaseQuintIn::getT() const
{
	float t = elapsedTime / duration;
	//return duration * t * t * t * t * t;
	return t * t * t * t * t;
}




//
// =========================== QUINT OUT ===============================
//

v3d::EaseQuintOut::EaseQuintOut()
	: v3d::Ease()
{}

v3d::EaseQuintOut::~EaseQuintOut() {}

v3d::EaseQuintOut * v3d::EaseQuintOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseQuintOut();

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

float v3d::EaseQuintOut::getT() const 
{
	float t = elapsedTime / duration;
	t--;
	//return duration * ((t * t * t * t * t + 1.0f));
	return ((t * t * t * t * t + 1.0f));
}




//
// ========================= QUINT IN OUT ===============================
//

v3d::EaseQuintInOut::EaseQuintInOut()
	: v3d::Ease()
{}

v3d::EaseQuintInOut::~EaseQuintInOut() {}

v3d::EaseQuintInOut * v3d::EaseQuintInOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseQuintInOut();

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

float v3d::EaseQuintInOut::getT() const
{
	float t = elapsedTime / (duration * 0.5f);

	if (t < 1.0f)
	{
		//return (duration * 0.5f) * t * t * t * t * t;
		return (0.5f) * t * t * t * t * t;
	}
	else
	{
		t -= 2.0f;

		//return (duration * 0.5f) * (t * t * t * t * t + 2.0f);
		return (0.5f) * (t * t * t * t * t + 2.0f);
	}
}