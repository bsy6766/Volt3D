#include <PreCompiled.h>

#include "ProgressTo.h"

#include "Volt3D/2D/ProgressTimer.h"

#include "Volt3D/Error/ErrorReport.h"

v3d::ProgressTo::ProgressTo()
	: v3d::Action()
	, percentage(0.0f)
	, percentageDelta(0.0f)
	, target(nullptr)
{}

v3d::ProgressTo::~ProgressTo() {}

bool v3d::ProgressTo::init(const float duration, const float percentage)
{
	if (duration < 0.0f || percentage < 0.0f || percentage > 100.0f)
	{
		return false;
	}

	this->duration = duration;
	this->percentage = static_cast<float>(percentage);

	return true;
}

v3d::ProgressTo * v3d::ProgressTo::create(const float duration, const float percentage)
{
	// Create instance
	auto newProgressTo = new (std::nothrow) v3d::ProgressTo();

	// Check
	if (newProgressTo)
	{
		// Success. Init.
		if (newProgressTo->init(duration, percentage))
		{
			// Success.
			return newProgressTo;
		}
		else
		{
			// Delete instance.
			delete newProgressTo;
		}
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::PROGRESS_TO);
	}

	// Failed
	return nullptr;
}

bool v3d::ProgressTo::setTarget(v3d::TransformNode * progressTimerTarget)
{
	// Check nullptr
	if (progressTimerTarget)
	{
		// attempt down cast
		target = dynamic_cast<v3d::ProgressTimer*>(progressTimerTarget);

		// Check nullptr
		if (target)
		{
			// Done.
			return true;
		}
		// Else, failed downcast
	}
	// Else, nullptr

	// Failed
	return false;
}

void v3d::ProgressTo::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// Check if action is starting
	if (elapsedTime == 0.0f)
	{
		// starting. update
		percentageDelta	 = percentage - target->getPercentage();
	}

	// add time
	elapsedTime += delta;

	if (duration == 0.0f)
	{
		// instant
		target->addPercentage(percentageDelta);
	}
	else
	{
		// interval
		target->addPercentage(percentageDelta * delta);
	}
}

void v3d::ProgressTo::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// Check if action is starting
	if (elapsedTime == 0.0f)
	{
		// starting. update
		percentageDelta = percentage - target->getPercentage();
	}

	if (duration == 0.0f)
	{
		// instant
		target->addPercentage(percentageDelta);

		elapsedTime += delta;
	}
	else
	{
		// interval
		float prevT = getLinearEaseTime();

		elapsedTime += delta;

		float curT = getLinearEaseTime();

		target->addPercentage(percentageDelta * (curT - prevT));
	}
}