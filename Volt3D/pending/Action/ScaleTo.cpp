#include <PreCompiled.h>

#include "ScaleTo.h"

#include "Volt3D/Node/TransformNode.h"

#include "Volt3D/Error/ErrorReport.h"

v3d::ScaleTo::ScaleTo()
	: v3d::Action()
	, scale(0.0f)
	, scaleDelta(0.0f)
	, target(nullptr)
{}

v3d::ScaleTo::~ScaleTo() {}

bool v3d::ScaleTo::init(const float duration, const glm::vec3 & scale)
{
	// Set duration
	if (setDuration(duration))
	{
		// Success.

		// set scale
		this->scale = scale;

		// Done.
		return true;
	}

	// Failed
	return false;
}

v3d::ScaleTo * v3d::ScaleTo::create(const float duration, const glm::vec3 & scale)
{
	// Create instance
	auto newScaleTo = new (std::nothrow) v3d::ScaleTo();

	// Check
	if (newScaleTo)
	{
		// Success. Init.
		if (newScaleTo->init(duration, scale))
		{
			// Success.
			return newScaleTo;
		}
		else
		{
			// Delete instance.
			delete newScaleTo;
		}
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::ROTATE_TO);
	}

	// Failed
	return nullptr;
}

bool v3d::ScaleTo::setTarget(v3d::TransformNode * target)
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

void v3d::ScaleTo::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// Check if action is starting
	if (elapsedTime == 0.0f)
	{
		// starting. update distance
		scaleDelta = scale - target->getScale();
	}

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

void v3d::ScaleTo::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// Check if action is starting
	if (elapsedTime == 0.0f)
	{
		// starting. update distance
		scaleDelta = scale - target->getScale();
	}
	
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
