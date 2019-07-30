#include <PreCompiled.h>

#include "EaseElastic.h"

#include "Volt3D/Utility/Utility.h"

//
// =========================== ELASTIC IN ================================
//

v3d::EaseElasticIn::EaseElasticIn()
	: v3d::Ease()
{}

v3d::EaseElasticIn::~EaseElasticIn() {}

v3d::EaseElasticIn * v3d::EaseElasticIn::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseElasticIn();

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

float v3d::EaseElasticIn::getT() const
{
	float t = elapsedTime;

	if (t == 0.0f)
	{
		return 0.0f;
	} 
	
	if ((t /= duration) == 1.0f)
	{
		return 1.0f;
	}
	
	float p = duration * 0.3f;
	float a = 1.0f;
	float s = p / 4.0f;
	float postFix = a * pow(2.0f, 10.0f * (t -= 1.0f)); // this is a fix, again, with post-increment operators

	return -(postFix * sinf((t * duration - s) * (2.0f * v3d::Utility::M_PIf) / p));
}




//
// =========================== ELASTIC OUT ===============================
//

v3d::EaseElasticOut::EaseElasticOut()
	: v3d::Ease()
{}

v3d::EaseElasticOut::~EaseElasticOut() {}

v3d::EaseElasticOut * v3d::EaseElasticOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseElasticOut();

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

float v3d::EaseElasticOut::getT() const 
{
	float t = elapsedTime;

	if (t == 0.0f)
	{
		return 0.0f;
	}
	
	if ((t /= duration) == 1.0f)
	{
		return 1.0f;
	}

	float p = duration * 0.3f;
	float a = 1.0f;
	float s = p / 4.0f;
	return (a * powf(2.0f, -10.0f * t) * sinf((t * duration - s) * (2.0f * v3d::Utility::M_PIf) / p) + 1.0f);
}




//
// ========================= ELASTIC IN OUT ===============================
//

v3d::EaseElasticInOut::EaseElasticInOut()
	: v3d::Ease()
{}

v3d::EaseElasticInOut::~EaseElasticInOut() {}

v3d::EaseElasticInOut * v3d::EaseElasticInOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseElasticInOut();

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

float v3d::EaseElasticInOut::getT() const
{
	float t = elapsedTime;

	if (t == 0.0f)
	{
		return 0.0f;
	}

	if ((t /= duration * 0.5f) == 2.0f)
	{
		return 1.0f;
	}

	float p = duration * (0.3f * 1.5f);
	float a = 1.0f;
	float s = p / 4.0f;

	if (t < 1.0f) 
	{
		float postFix = a * powf(2.0f, 10.0f * (t -= 1)); // postIncrement is evil
		return -0.5f * (postFix * sinf((t * duration - s) * (2.0f * v3d::Utility::M_PIf) / p));
	}
	else
	{
		float postFix = a * powf(2.0f, -10.0f * (t -= 1.0f)); // postIncrement is evil
		return postFix * sinf((t * duration - s) * (2.0f * v3d::Utility::M_PIf) / p) * 0.5f + 1.0f;
	}
}