#include <PreCompiled.h>

#include "ProgressBy.h"

#include "Volt3D/2D/ProgressTimer.h"

#include "Volt3D/Error/ErrorReport.h"

v3d::ProgressBy::ProgressBy()
	: v3d::Action()
	, percentage(0)
	, target(nullptr)
{}

v3d::ProgressBy::~ProgressBy() {}

bool v3d::ProgressBy::init(const float duration, const float percentage)
{
	// Set duration
	if (setDuration(duration))
	{
		// Success.

		// set percentage
		this->percentage = percentage;

		// Done.
		return true;
	}

	// Failed
	return false;
}

v3d::ProgressBy * v3d::ProgressBy::create(const float duration, const float percentage)
{
	// Create instance
	auto newProgressBy = new (std::nothrow) v3d::ProgressBy();

	// Check
	if (newProgressBy)
	{
		// Success. init.
		if (newProgressBy->init(duration, percentage))
		{
			// Success.
			return newProgressBy;
		}
		else
		{
			// Delete instance.
			delete newProgressBy;
		}
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::PROGRESS_BY);
	}

	// Failed
	return nullptr;
}

bool v3d::ProgressBy::setTarget(v3d::TransformNode * progressTimerTarget)
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

void v3d::ProgressBy::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// add time
	elapsedTime += delta;

	if (duration == 0.0f)
	{
		// instant
		target->addPercentage(percentage);
	}
	else
	{
		// interval
		target->addPercentage(percentage * delta);
	}
}

void v3d::ProgressBy::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	if (duration == 0.0f)
	{
		// instant
		target->addPercentage(percentage);

		elapsedTime += delta;
	}
	else
	{
		// interval
		float prevT = getLinearEaseTime();

		elapsedTime += delta;

		float curT = getLinearEaseTime();

		target->addPercentage(percentage * (curT - prevT));
	}
}