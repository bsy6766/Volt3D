#include <PreCompiled.h>

#include "FadeBy.h"

#include "Volt3D/Node/TransformNode.h"
#include "Volt3D/Node/RenderNode.h"

#include "Volt3D/Error/ErrorReport.h"

v3d::FadeBy::FadeBy()
	: v3d::Action()
	, opacityDelta(0.0f)
	, target(nullptr)
{}

v3d::FadeBy::~FadeBy() {}

bool v3d::FadeBy::init(const float duration, const float opacity)
{
	// Set duration
	if (setDuration(duration))
	{
		// Success.

		// set distance
		this->opacityDelta = opacity;

		// Done.
		return true;
	}

	// Failed
	return false;
}

v3d::FadeBy * v3d::FadeBy::create(const float duration, const float opacity)
{
	// Create instance
	auto newFadeBy = new (std::nothrow) v3d::FadeBy();

	// Check
	if (newFadeBy)
	{
		// Success. Init.
		if (newFadeBy->init(duration, opacity))
		{
			// Success.
			return newFadeBy;
		}
		else
		{
			// delete instance.
			delete newFadeBy;
		}
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::FADE_BY);
	}

	// Failed.
	return nullptr;
}

bool v3d::FadeBy::setTarget(v3d::TransformNode * target)
{
	// Check nullptr
	if (target)
	{
		// RenderNode has opacity. Downcast.
		if (v3d::RenderNode* renderNodeTarget = dynamic_cast<v3d::RenderNode*>(target))
		{
			// Success.

			// Check if it has material
			if (renderNodeTarget->hasMaterial())
			{
				// Has material.
				this->target = renderNodeTarget;

				// Done.
				return true;
			}
		}
		// Else, failed to cast to RenderNode. Invalid target.
	}
	// Else, nullptr

	// Failed
	return false;
}

void v3d::FadeBy::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;
	if (target == nullptr) return;

	// add time
	elapsedTime += delta;

	if (duration == 0.0f)
	{
		// instant
		target->addOpacity(opacityDelta);
	}
	else
	{
		// interval
		target->addOpacity(opacityDelta * delta);
	}
}

void v3d::FadeBy::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;
	if (target == nullptr) return;

	if (duration == 0.0f)
	{
		// instant
		target->addOpacity(opacityDelta);

		elapsedTime += delta;
	}
	else
	{
		// interval
		float prevT = getLinearEaseTime();

		elapsedTime += delta;

		float curT = getLinearEaseTime();

		target->addOpacity(opacityDelta * (curT - prevT));
	}
}