#include <PreCompiled.h>

#include "RotateTo.h"

#include "Volt3D/Node/TransformNode.h"

#include "Volt3D/Error/ErrorReport.h"

v3d::RotateTo::RotateTo()
	: v3d::Action()
	, angle(0.0f)
	, angleDelta(0.0f)
	, target(nullptr)
{}

v3d::RotateTo::~RotateTo() {}

bool v3d::RotateTo::init(const float duration, const glm::vec3 angle)
{
	// Set duration
	if (setDuration(duration))
	{
		// Success.

		// set angle
		this->angle = angle;

		// Done.
		return true;
	}

	// Failed
	return false;
}

v3d::RotateTo * v3d::RotateTo::create(const float duration, const glm::vec3 angle)
{
	auto newRotateTo = new (std::nothrow) v3d::RotateTo();

	if (newRotateTo)
	{
		if (newRotateTo->init(duration, angle))
		{
			return newRotateTo;
		}
		else
		{
			delete newRotateTo;
			return nullptr;
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

bool v3d::RotateTo::setTarget(v3d::TransformNode * target)
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

void v3d::RotateTo::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// add time
	elapsedTime += delta;

	// Check if action is starting
	if (elapsedTime == 0.0f)
	{
		// starting. update distance
		angleDelta = angle - target->getRotation();
	}

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

void v3d::RotateTo::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// Check if action is starting
	if (elapsedTime == 0.0f)
	{
		// starting. update distance
		angleDelta = angle - target->getRotation();
	}

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