#include <PreCompiled.h>

#include "Sequence.h"

#include "Action.h"

#include "Volt3D/Config/BuildConfig.h"

#include "Volt3D/Error/ErrorReport.h"

v3d::Sequence::Sequence()
	: Action()
	, first(nullptr)
	, second(nullptr)
{}

v3d::Sequence::~Sequence()
{
	if (first)
	{
		delete first;
	}

	if (second)
	{
		delete second;
	}
}

bool v3d::Sequence::init(v3d::Action * action)
{
	if (action)
	{
		first = action;

		this->duration = first->getDuration();

		return true;
	}
	else
	{
		return false;
	}
}

bool v3d::Sequence::init(v3d::Action * first, v3d::Action * second)
{
	if (first && second)
	{
		this->first = first;
		this->second = second;

		this->duration = this->first->getDuration() + this->second->getDuration();

		return true;
	}
	else
	{
		return false;
	}
}

bool v3d::Sequence::init(const std::initializer_list<v3d::Action*>& actions)
{
	auto it = actions.begin();
	auto prev = *it;
	auto size = actions.size();
	
	for (unsigned int i = 1; i < (size - 1); ++i)
	{
		it = std::next(it);
		prev = create(prev, *it);
	}

	first = prev;
	second = *std::next(it);

	for (auto action : actions)
	{
		this->duration += action->getDuration();
	}

	return true;
}

v3d::Sequence * v3d::Sequence::create(v3d::Action * action)
{
	// Create sequence
	auto newSeq = new (std::nothrow) v3d::Sequence();

	// check
	if (newSeq)
	{
		// Success. Init with one action.
		if (newSeq->init(action))
		{
			// Success.
			return newSeq;
		}
		else
		{
			// Delete instance
			delete newSeq;
		}
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::SEQUENCE);
	}

	// Failed
	return nullptr;
}

v3d::Sequence * v3d::Sequence::create(v3d::Action * first, v3d::Action * second)
{
	// Create instance
	auto newSeq = new (std::nothrow) v3d::Sequence();

	// Check
	if (newSeq)
	{
		// Success. Init with two actions
		if (newSeq->init(first, second))
		{
			// Success.
			return newSeq;
		}
		else
		{
			// Delete instance
			delete newSeq;
		}
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::SEQUENCE);
	}

	// Failed
	return nullptr;
}

v3d::Sequence * v3d::Sequence::create(const std::initializer_list<v3d::Action*>& actions)
{
	// Get size
	const auto size = actions.size();

	// Check size
	if (size == 0)
	{
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
		// report
		v3d::Error::Report::report(v3d::Error::Code::generate(false, v3d::Error::Code::Source::SEQUENCE, v3d::Error::Code::SequenceErrorCode::LIST_OF_ACTION_SEQ_IS_EMPTY));
#endif

		// Failed.
		return nullptr;
	}
	else
	{
		// Has at least 1 action

		// Check size again
		if (size == 1)
		{
			// Create with 1 action
			return create(*actions.begin());
		}
		else if (size == 2)
		{
			// Create with 2 actions
			return create(*actions.begin(), *(std::prev(actions.end())));
		}
		else
		{
			// Create with more than 2 actions

			// Create instance
			auto newSeq = new (std::nothrow) v3d::Sequence();

			// Check
			if (newSeq)
			{
				// Success. Init with actions
				if (newSeq->init(actions))
				{
					// Success.
					return newSeq;
				}
				else
				{
					// Delete instance
					delete newSeq;
				}
			}
			else
			{
				// Report
				v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::SEQUENCE);
			}

			// Failed
			return nullptr;
		}
	}
}

bool v3d::Sequence::isDone() const
{
	return (first ? first->isDone() : true) && (second ? second->isDone() : true);
}

float v3d::Sequence::getExceededTime()
{
	// Check nullptr
	if (first)
	{
		// has First.

		// check if first is done
		if (first->isDone())
		{
			// first is doen

			// check nullptr
			if (second)
			{
				// has second

				// Check if second is done
				if (second->isDone())
				{
					// second is done
					return second->getExceededTime();
				}
			}
			else
			{
				// Doesn't have second
				return first->getExceededTime();
			}
		}
		// First action is still running
	}

	return 0.0f;
}

void v3d::Sequence::reset()
{
	if (first)
	{
		first->reset();
	}

	if (second)
	{
		second->reset();
	}

	elapsedTime = 0.0f;
}

bool v3d::Sequence::setTarget(v3d::TransformNode * target)
{
	if (first)
	{
		if (!first->setTarget(target))
		{
			return false;
		}
	}

	if (second)
	{
		return second->setTarget(target);
	}

	return false;
}

void v3d::Sequence::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// Check nullptr
	if (first == nullptr) return;

	// add time
	elapsedTime += delta;

	// check nullptr
	if (first)
	{
		// check if first is done
		if (first->isDone())
		{
			// First is done. check second
			if (second)
			{
				// Second exists.

				// Check if second is done
				if (second->isDone())
				{
					// Second is finished. Sequnce is done.
					return;
				}
				else
				{
					// Second is still running. 

					// Update second
					second->updateEase(delta);
				}
			}
		}
		else
		{
			// First is still running.

			// Update first.
			first->updateEase(delta);

			// Check if it's done
			if (first->isDone())
			{
				// check second
				if (second)
				{
					// second exists

					// Check if second is done
					if (second->isDone())
					{
						// second is done. Sequence finished
						return;
					}
					else
					{
						// second is still running.

						// get exceeded time of first action and update seoncd
						second->updateEase(first->getExceededTime());
					}
				}
			}
		}
	}
	// Else, do nothing
}

void v3d::Sequence::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;
	
	// add time
	elapsedTime += delta;

	// check nullptr
	if (first)
	{
		// check if first is done
		if (first->isDone())
		{
			// First is done. check second
			if (second)
			{
				// Second exists.

				// Check if second is done
				if (second->isDone())
				{
					// Second is finished. Sequnce is done.
					return;
				}
				else
				{
					// Second is still running. 

					// Update second
					second->update(delta);
				}
			}
		}
		else
		{
			// First is still running.

			// Update first.
			first->update(delta);
			
			// Check if it's done
			if (first->isDone())
			{
				// check second
				if (second)
				{
					// second exists

					// Check if second is done
					if (second->isDone())
					{
						// second is done. Sequence finished
						return;
					}
					else
					{
						// second is still running.

						// get exceeded time of first action and update seoncd
						second->update(first->getExceededTime());
					}
				}
			}
		}
	}
	// Else, do nothing
}
