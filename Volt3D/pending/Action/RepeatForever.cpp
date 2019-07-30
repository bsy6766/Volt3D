#include <PreCompiled.h>

#include "RepeatForever.h"

#include "Action.h"

v3d::RepeatForever::RepeatForever()
	: v3d::Action()
	, action(nullptr)
{}

v3d::RepeatForever::~RepeatForever()
{
	if (action)
	{
		delete action;
	}
}

bool v3d::RepeatForever::init(v3d::Action * action)
{
	if (action)
	{
		this->action = action;
		return true;
	}

	return false;
}

v3d::RepeatForever * v3d::RepeatForever::create(v3d::Action * action)
{
	// Reject instant actions.
	if (action->getDuration() <= 0.0f) return nullptr;

	auto newRepeat = new (std::nothrow) v3d::RepeatForever();

	if (newRepeat)
	{
		if (newRepeat->init(action))
		{
			return newRepeat;
		}
		else
		{
			delete newRepeat;
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
}

bool v3d::RepeatForever::isDone() const
{
	// repeat forever never ends
	return false;
}

void v3d::RepeatForever::reset()
{
	if (action)
	{
		action->reset();
	}
}

float v3d::RepeatForever::getExceededTime()
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

bool v3d::RepeatForever::setTarget(v3d::TransformNode * target)
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

void v3d::RepeatForever::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// Check nullptr
	if (action == nullptr) return;

	// Update action
	action->updateEase(delta);

	// Check if it's done
	if (action->isDone())
	{
		// action is done

		// get exceeded time
		float t = action->getExceededTime();

		// reset
		action->reset();

		// consume all exceeded time 
		while (t > 0.0f)
		{
			// update with exceeded time
			action->updateEase(t);

			if (action->isDone())
			{
				// Action is done

				// update exceeded time.
				t = action->getExceededTime();

				// rest
				action->reset();
			}
			else
			{
				// consumed all
				t = 0.0f;
				break;
			}
		}
	}
}

void v3d::RepeatForever::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// Check nullptr
	if (action == nullptr) return;

	// Update action
	action->update(delta);

	// Check if it's done
	if (action->isDone())
	{
		// action is done

		// get exceeded time
		float t = action->getExceededTime();

		// reset
		action->reset();

		// consume all exceeded time 
		while (t > 0.0f)
		{
			// update with exceeded time
			action->update(t);

			if (action->isDone())
			{
				// Action is done

				// update exceeded time.
				t = action->getExceededTime();

				// rest
				action->reset();
			}
			else
			{
				// consumed all
				t = 0.0f;
				break;
			}
		}
	}
}