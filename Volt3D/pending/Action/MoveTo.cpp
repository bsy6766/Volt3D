#include <PreCompiled.h>

#include "MoveTo.h"

#include "Volt3D/Node/TransformNode.h"

#include "Volt3D/Error/ErrorReport.h"

v3d::MoveTo::MoveTo()
	: v3d::Action()
	, destination(0.0f)
	, distance(0.0f)
	, target(nullptr)
{}

v3d::MoveTo::~MoveTo() {}

bool v3d::MoveTo::init(const float duration, const glm::vec3 & destination)
{
	// Set duration
	if (setDuration(duration))
	{
		// Success.

		// set destination
		this->destination = destination;

		// Done.
		return true;
	}

	// Failed
	return false;
}

v3d::MoveTo * v3d::MoveTo::create(const float duration, const glm::vec3 & destination)
{
	// Create instance
	auto newMoveTo = new (std::nothrow) v3d::MoveTo();

	// Check
	if (newMoveTo)
	{
		// success. Init.
		if (newMoveTo->init(duration, destination))
		{
			// Success.
			return newMoveTo;
		}
		else
		{
			// Delete instance
			delete newMoveTo;
		}
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::MOVE_TO);
	}

	// Failed
	return nullptr;
}

bool v3d::MoveTo::setTarget(v3d::TransformNode * target)
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

void v3d::MoveTo::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;
	if (target == nullptr) return;

	// Check if action is starting
	if (elapsedTime == 0.0f)
	{
		// starting. update distance
		distance = destination - target->getPosition();
	}

	elapsedTime += delta;

	if (duration == 0.0f)
	{
		// instant
		target->addPosition(distance);
	}
	else
	{
		// interval
		target->addPosition(distance * delta);
	}
}

void v3d::MoveTo::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// Check if action is starting
	if (elapsedTime == 0.0f)
	{
		// starting. update distance
		distance = destination - target->getPosition();
	}

	if (duration == 0.0f)
	{
		// instant
		target->addPosition(distance);

		elapsedTime += delta;
	}
	else
	{
		// interval
		float prevT = getLinearEaseTime();

		elapsedTime += delta;

		float curT = getLinearEaseTime();

		target->addPosition(distance * (curT - prevT));
	}
}
