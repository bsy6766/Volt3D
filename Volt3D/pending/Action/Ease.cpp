#include <PreCompiled.h>

#include "Ease.h"

v3d::Ease::Ease() 
	: v3d::Action()
	, prevT(0.0f)
	, action(nullptr)
{}

bool v3d::Ease::init(v3d::Action * action)
{
	// check nullptr
	if (action)
	{
		// Set action
		this->action = action;

		// Copy duration
		if (setDuration(action->getDuration()))
		{
			// Success.
			return true;
		}
	}

	// Failed.
	return false;
}

v3d::Ease::~Ease()
{
	if (action)
	{
		delete action;
		action = nullptr;
	}
}

bool v3d::Ease::isDone() const
{
	// Ease is done time is over.
	return elapsedTime >= duration;
}

void v3d::Ease::reset()
{
	if (action)
	{
		action->reset();
		elapsedTime = 0.0f;
		duration = action->getDuration();
		prevT = 0.0f;
	}
}

float v3d::Ease::getExceededTime()
{
	if (action)
	{
		return action->getExceededTime();
	}
	else
	{
		return 0.0f;
	}
}

bool v3d::Ease::setTarget(v3d::TransformNode * target)
{
	// Check nullptr
	if (action)
	{
		// set target
		return action->setTarget(target);
	}

	// Failed
	return false;
}

void v3d::Ease::update(const float delta)
{
	if (action)
	{
		if (duration == 0.0f)
		{
			action->update(delta);
		}
		else
		{
			// temp t
			float t = elapsedTime;
			// add time
			t += delta;
			// add time
			elapsedTime += delta;

			// clamp elapsed time to duration
			if (elapsedTime > duration)
			{
				elapsedTime = duration;
			}

			// get t by elapsed time
			float newT = getT();

			//action->update(newT - prevT);
			action->updateEase(newT - prevT);

			prevT = newT;

			// back up
			elapsedTime = t;
		}
	}
}