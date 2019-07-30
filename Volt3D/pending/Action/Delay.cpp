#include <PreCompiled.h>

#include "Delay.h"

#include "Volt3D/Error/ErrorReport.h"

#include "Volt3D/Core/Logger.h"

#include "Volt3D/Config/LogConfig.h"

v3d::Delay::Delay()
	: v3d::Action()
{}

v3d::Delay::~Delay() {}

bool v3d::Delay::init(const float duration)
{
	// Set duration
	return setDuration(duration);
}

v3d::Delay * v3d::Delay::create(const float duration)
{
	// Create instance
	auto newDelay = new (std::nothrow) v3d::Delay();

	// Check
	if (newDelay)
	{
		// success. Init.
		if (newDelay->init(duration))
		{
			// Success.
			return newDelay;
		}
		else
		{
			// Delete instance
			delete newDelay;
		}
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::DELAY);
	}

	// Failed.
	return nullptr;
}

bool v3d::Delay::setTarget(v3d::TransformNode * target)
{
	// Delay doesn't have target. But consider this operation as true so it doesn't break action system.
	return true;
}

void v3d::Delay::update(const float delta)
{
	// There is nothing to do with Delay. Just add time.
	elapsedTime += delta;
}
