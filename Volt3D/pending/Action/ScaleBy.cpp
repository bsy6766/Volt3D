#include <PreCompiled.h>

#include "ScaleBy.h"

#include "Volt3D/Node/TransformNode.h"

#include "Volt3D/Error/ErrorReport.h"

v3d::ScaleBy::ScaleBy()
	: v3d::Action()
	, scaleDelta(0.0f)
	, target(nullptr)
{}

v3d::ScaleBy::~ScaleBy() {}

bool v3d::ScaleBy::init(const float duration, const glm::vec3 & scale)
{
	// Set duration
	if (setDuration(duration))
	{
		// Success.

		// set scale
		scaleDelta = scale;

		// Done.
		return true;
	}

	// Failed
	return false;
}

v3d::ScaleBy * v3d::ScaleBy::create(const float duration, const glm::vec3 & scale)
{
	// Create instnace
	auto newScaleBy = new (std::nothrow) v3d::ScaleBy();

	// Check
	if (newScaleBy)
	{
		// Success. init.
		if (newScaleBy->init(duration, scale))
		{
			// Success.
			return newScaleBy;
		}
		else
		{
			// Delete instance.
			delete newScaleBy;
		}
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::SCALE_BY);
	}

	// Failed
	return nullptr;
}

bool v3d::ScaleBy::setTarget(v3d::TransformNode * target)
{
	// Check nullptr
	if (target)
	{
		// set
		this->target = target;

		// Done
		return true;
	}

	// Failed
	return false;
}

void v3d::ScaleBy::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// add time
	elapsedTime += delta;

	if (duration == 0.0f)
	{
		// instant
		target->addScale(scaleDelta);
	}
	else
	{
		// interval
		target->addScale(scaleDelta * delta);
	}
}

void v3d::ScaleBy::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	if (duration == 0.0f)
	{
		// instant
		target->addScale(scaleDelta);

		elapsedTime += delta;
	}
	else
	{
		// interval
		float prevT = getLinearEaseTime();

		elapsedTime += delta;

		float curT = getLinearEaseTime();

		target->addScale(scaleDelta * (curT - prevT));
	}
}
