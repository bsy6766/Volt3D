#include <PreCompiled.h>

#include "Repeat.h"

#include "Action.h"

#include "Volt3D/Error/ErrorReport.h"

v3d::Repeat::Repeat()
	: v3d::Action()
	, action(nullptr)
	, repeat(0)
	, currentRepeat(0)
{}

v3d::Repeat::~Repeat()
{
	if (action)
	{
		delete action;
	}
}

bool v3d::Repeat::init(v3d::Action * action, const unsigned int repeat)
{
	if (action)
	{
		this->action = action;
		this->repeat = repeat;
		return true;
	}
	
	return false;
}

v3d::Repeat * v3d::Repeat::create(v3d::Action * action, const unsigned int repeat)
{
	// Create instance
	auto newRepeat = new (std::nothrow) v3d::Repeat();

	// Check
	if (newRepeat)
	{
		// Success. Init.
		if (newRepeat->init(action, repeat))
		{
			// Success.
			return newRepeat;
		}
		else
		{
			// Delete instance.
			delete newRepeat;
		}
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::REPEAT);
	}

	// Failed
	return nullptr;
}

unsigned int v3d::Repeat::getRepeat() const
{
	return repeat;
}

bool v3d::Repeat::isDone() const
{
	return (currentRepeat >= repeat);
}

void v3d::Repeat::reset()
{
	if (action)
	{
		action->reset();
	}
}

float v3d::Repeat::getExceededTime()
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

bool v3d::Repeat::setTarget(TransformNode * target)
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

void v3d::Repeat::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// Check nullptr
	if (action == nullptr) return;

	// Check if repeat is done
	if (!isDone())
	{
		// Repeat is not done

		// updat eaction
		action->updateEase(delta);

		// Check if action is finished
		if (action->isDone())
		{
			// Action is done

			// inc repeat counter
			currentRepeat++;

			// Check again if repeat is done
			if (!isDone())
			{
				// not done.

				// get exceeded time
				float t = action->getExceededTime();

				// reset action
				action->reset();

				// consume all exceeded time while repeat is not done.
				while (t > 0.0f && (!isDone()))
				{
					// update with exceeded time
					action->updateEase(t);

					if (action->isDone())
					{
						// Action is done

						// inc repeat counter
						currentRepeat++;

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
	}
}

void v3d::Repeat::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// Check nullptr
	if (action == nullptr) return;

	// Check if repeat is done
	if (!isDone())
	{
		// Repeat is not done

		// updat eaction
		action->update(delta);

		// Check if action is finished
		if (action->isDone())
		{
			// Action is done

			// inc repeat counter
			currentRepeat++;

			// Check again if repeat is done
			if (!isDone())
			{
				// not done.

				// get exceeded time
				float t = action->getExceededTime();

				// reset action
				action->reset();

				// consume all exceeded time while repeat is not done.
				while (t > 0.0f && (!isDone()))
				{
					// update with exceeded time
					action->update(t);

					if (action->isDone())
					{
						// Action is done

						// inc repeat counter
						currentRepeat++;

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
	}
}