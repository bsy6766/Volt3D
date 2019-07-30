#include <PreCompiled.h>

#include "EaseCirc.h"

//
// =========================== CIRC IN ================================
//

v3d::EaseCircIn::EaseCircIn()
	: v3d::Ease()
{}

v3d::EaseCircIn::~EaseCircIn() {}

v3d::EaseCircIn * v3d::EaseCircIn::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseCircIn();

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

float v3d::EaseCircIn::getT() const
{
	float t = elapsedTime / duration;

	//return -duration * (sqrtf(1.0f - (t * t)) - 1.0f);
	return (-1.0f) * (sqrtf(1.0f - (t * t)) - 1.0f);
}




//
// =========================== CIRC OUT ===============================
//

v3d::EaseCircOut::EaseCircOut()
	: v3d::Ease()
{}

v3d::EaseCircOut::~EaseCircOut() {}

v3d::EaseCircOut * v3d::EaseCircOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseCircOut();

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

float v3d::EaseCircOut::getT() const 
{
	float t = elapsedTime / duration;

	t -= 1.0f;

	//return duration * sqrtf(1.0f - (t * t));
	return sqrtf(1.0f - (t * t));
}




//
// ========================= CIRC IN OUT ===============================
//

v3d::EaseCircInOut::EaseCircInOut()
	: v3d::Ease()
{}

v3d::EaseCircInOut::~EaseCircInOut() {}

v3d::EaseCircInOut * v3d::EaseCircInOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseCircInOut();

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

float v3d::EaseCircInOut::getT() const
{
	float t = (elapsedTime / (duration * 0.5f));

	if (t < 1.0f)
	{
		//return (duration * -0.5f) * (sqrtf(1.0f - (t * t)) - 1.0f);
		return (-0.5f) * (sqrtf(1.0f - (t * t)) - 1.0f);
	}
	else
	{
		t -= 2.0f;

		//return (duration * 0.5f) * (sqrtf(1.0f - (t * t)) + 1.0f);
		return (0.5f) * (sqrtf(1.0f - (t * t)) + 1.0f);
	}
}