#include <PreCompiled.h>

#include "EaseQuad.h"

//
// =========================== QUAD IN ================================
//

v3d::EaseQuadIn::EaseQuadIn()
	: v3d::Ease()
{}

v3d::EaseQuadIn::~EaseQuadIn() {}

v3d::EaseQuadIn * v3d::EaseQuadIn::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseQuadIn();

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

float v3d::EaseQuadIn::getT() const
{
	float t = elapsedTime / duration;
	//return duration * t * t;
	return t * t;
}




//
// =========================== QUAD OUT ===============================
//

v3d::EaseQuadOut::EaseQuadOut()
	: v3d::Ease()
{}

v3d::EaseQuadOut::~EaseQuadOut() {}

v3d::EaseQuadOut * v3d::EaseQuadOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseQuadOut();

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

float v3d::EaseQuadOut::getT() const 
{
	float t = elapsedTime / duration;
	//return -duration * t * (t - 2.0f);
	return (-1.0f) * t * (t - 2.0f);
}




//
// ========================= QUAD IN OUT ===============================
//

v3d::EaseQuadInOut::EaseQuadInOut()
	: v3d::Ease()
{}

v3d::EaseQuadInOut::~EaseQuadInOut() {}

v3d::EaseQuadInOut * v3d::EaseQuadInOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseQuadInOut();

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

float v3d::EaseQuadInOut::getT() const
{
	float t = elapsedTime;

	if ((t /= duration * 0.5f) < 1.0f)
	{
		//return (duration * 0.5f) * (t * t);
		return (0.5f) * (t * t);
	}
	else
	{
		//return (duration * -0.5f) * (((t - 2.0f) * (--t)) - 1.0f);
		return (-0.5f) * (((t - 2.0f) * (--t)) - 1.0f);
	}
}