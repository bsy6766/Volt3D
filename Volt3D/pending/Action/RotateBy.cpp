#include <PreCompiled.h>

#include "RotateBy.h"

#include "Volt3D/Node/TransformNode.h"

#include "Volt3D/Error/ErrorReport.h"

v3d::RotateBy::RotateBy()
	: v3d::Action()
	, angleDelta(0.0f)
	, target(nullptr)
{}

v3d::RotateBy::~RotateBy() {}

bool v3d::RotateBy::init(const float duration, const glm::vec3 angle)
{
	// Set duration
	if (setDuration(duration))
	{
		// Success.

		// set angle
		angleDelta = angle;

		// Done.
		return true;
	}

	// Failed
	return false;
}

v3d::RotateBy * v3d::RotateBy::create(const float duration, const glm::vec3 angle)
{
	// Create instance
	auto newRotateBy = new (std::nothrow) v3d::RotateBy();

	// Check
	if (newRotateBy)
	{
		// Success. Init.
		if (newRotateBy->init(duration, angle))
		{
			// Success.
			return newRotateBy;
		}
		else
		{
			// Delete instance.
			delete newRotateBy;
		}
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::ROTATE_BY);
	}

	// Failed
	return nullptr;
}

bool v3d::RotateBy::setTarget(v3d::TransformNode * target)
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

void v3d::RotateBy::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// add time
	elapsedTime += delta;

	if (duration == 0.0f)
	{
		// instant
		target->addRotation(angleDelta);
	}
	else
	{
		// interval
		target->addRotation(angleDelta * delta);
	}
}

void v3d::RotateBy::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	if (duration == 0.0f)
	{
		// instant
		target->addRotation(angleDelta);

		elapsedTime += delta;
	}
	else
	{
		// interval
		float prevT = getLinearEaseTime();

		elapsedTime += delta;

		float curT = getLinearEaseTime();

		target->addRotation(angleDelta * (curT - prevT));
	}
}
