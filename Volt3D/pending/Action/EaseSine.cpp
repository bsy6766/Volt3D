#include <PreCompiled.h>

#include "EaseSine.h"

#include "Volt3D/Utility/Utility.h"

//
// =========================== SINE IN ================================
//

v3d::EaseSineIn::EaseSineIn()
	: v3d::Ease()
{}

v3d::EaseSineIn::~EaseSineIn() {}

v3d::EaseSineIn * v3d::EaseSineIn::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseSineIn();

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

float v3d::EaseSineIn::getT() const
{
	//return -duration * cosf(elapsedTime / duration * (v3d::Utility::M_PI_2f)) + duration;
	return (-1.0f) * cosf((elapsedTime / duration) * (v3d::Utility::M_PI_2f)) + 1.0f;
}




//
// =========================== SINE OUT ===============================
//

v3d::EaseSineOut::EaseSineOut()
	: v3d::Ease()
{}

v3d::EaseSineOut::~EaseSineOut() {}

v3d::EaseSineOut * v3d::EaseSineOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseSineOut();

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

float v3d::EaseSineOut::getT() const 
{
	//return duration * sinf(elapsedTime / duration * (v3d::Utility::M_PI_2f));
	return sinf(elapsedTime / duration * (v3d::Utility::M_PI_2f));
}




//
// ========================= SINE IN OUT ===============================
//

v3d::EaseSineInOut::EaseSineInOut()
	: v3d::Ease()
{}

v3d::EaseSineInOut::~EaseSineInOut() {}

v3d::EaseSineInOut * v3d::EaseSineInOut::create(v3d::Action * action)
{
	if (action == nullptr) return nullptr;

	auto newEase = new (std::nothrow) v3d::EaseSineInOut();

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

float v3d::EaseSineInOut::getT() const
{
	//return duration * -0.5f * (cosf(v3d::Utility::M_PIf * elapsedTime / duration) - 1.0f);
	return (-0.5f) * (cosf(v3d::Utility::M_PIf * elapsedTime / duration) - 1.0f);
}