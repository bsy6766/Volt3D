#include <PreCompiled.h>

#include "EaseQuart.h"

//
// =========================== CUBIC IN ================================
//

v3d::EaseQuartIn::EaseQuartIn()
	: v3d::Ease()
{}

v3d::EaseQuartIn::~EaseQuartIn() {}

v3d::EaseQuartIn * v3d::EaseQuartIn::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseQuartIn();

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

float v3d::EaseQuartIn::getT() const
{
	float t = elapsedTime / duration;
	//return duration * t * t * t * t;
	return t * t * t * t;
}




//
// =========================== CUBIC OUT ===============================
//

v3d::EaseQuartOut::EaseQuartOut()
	: v3d::Ease()
{}

v3d::EaseQuartOut::~EaseQuartOut() {}

v3d::EaseQuartOut * v3d::EaseQuartOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseQuartOut();

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

float v3d::EaseQuartOut::getT() const 
{
	float t = elapsedTime / duration;
	t--;
	//return -duration * ((t * t * t * t - 1.0f));
	return (-1.0f) * ((t * t * t * t - 1.0f));
}




//
// ========================= CUBIC IN OUT ===============================
//

v3d::EaseQuartInOut::EaseQuartInOut()
	: v3d::Ease()
{}

v3d::EaseQuartInOut::~EaseQuartInOut() {}

v3d::EaseQuartInOut * v3d::EaseQuartInOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseQuartInOut();

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

float v3d::EaseQuartInOut::getT() const
{
	float t = elapsedTime / (duration * 0.5f);

	if (t < 1.0f)
	{
		//return (duration * 0.5f) * t * t * t * t;
		return (0.5f) * t * t * t * t;
	}
	else
	{
		t -= 2.0f;

		//return (duration * -0.5f) * (t * t * t * t - 2.0f);
		return (-0.5f) * (t * t * t * t - 2.0f);
	}
}