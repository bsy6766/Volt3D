#include <PreCompiled.h>

#include "MoveBy.h"

#include "Volt3D/Node/TransformNode.h"

#include "Volt3D/Error/ErrorReport.h"

v3d::MoveBy::MoveBy()
	: v3d::Action()
	, distance(0.0f)
	, target(nullptr)
{}

v3d::MoveBy::~MoveBy() {}

v3d::MoveBy * v3d::MoveBy::create(const float duration, const glm::vec3 & distance)
{
	// Create instance
	auto newMoveBy = new (std::nothrow) v3d::MoveBy();

	// Check
	if (newMoveBy)
	{
		// Success. Init.
		if (newMoveBy->init(duration, distance))
		{
			// Success.
			return newMoveBy;
		}
		else
		{
			// Delete instance.
			delete newMoveBy;
		}
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::MOVE_BY);
	}

	// Failed.
	return nullptr;
}

bool v3d::MoveBy::init(const float duration, const glm::vec3 & distance)
{
	// Set duration
	if (setDuration(duration))
	{
		// Success.

		// set distance
		this->distance = distance;

		// Done.
		return true;
	}

	// Failed
	return false;
}


bool v3d::MoveBy::setTarget(v3d::TransformNode * target)
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

void v3d::MoveBy::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;
	if (target == nullptr) return;

	// add time
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

void v3d::MoveBy::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;
	if (target == nullptr) return;

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