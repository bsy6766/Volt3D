#include <PreCompiled.h>

#include "FadeTo.h"

#include "Volt3D/Node/TransformNode.h"
#include "Volt3D/Node/RenderNode.h"

#include "Volt3D/Config/BuildConfig.h"

#include "Volt3D/Core/Logger.h"

#include "Volt3D/Error/ErrorReport.h"

v3d::FadeTo::FadeTo()
	: v3d::Action()
	, opacity(0.0f)
	, opacityDelta(0.0f)
	, target(nullptr)
{}

v3d::FadeTo::~FadeTo() {}

bool v3d::FadeTo::init(const float duration, const float opacity)
{
	if (setDuration(duration))
	{
		// Check opacity
		if (opacity < 0.0f || 1.0f < opacity)
		{
			// Opacity is less than 0 or greater than 1. Clamp.
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
			v3d::Logger::getInstance().warn("[FadeTo] Clamped opacity " + std::to_string(opacity) + " to [0.0f, 1.0f].");
#endif
		}

		// Done.
		return true;
	}

	// Failed
	return false;
}

v3d::FadeTo * v3d::FadeTo::create(const float duration, const float opacity)
{
	// Create instance
	auto newFadeTo = new (std::nothrow) v3d::FadeTo();

	// Check
	if (newFadeTo)
	{
		// Success. Init.
		if (newFadeTo->init(duration, opacity))
		{
			// Success.
			return newFadeTo;
		}
		else
		{
			// Delete instance
			delete newFadeTo;
		}
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::FADE_TO);
	}

	// Failed.
	return nullptr;
}

bool v3d::FadeTo::setTarget(v3d::TransformNode * target)
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

void v3d::FadeTo::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;
	if (target == nullptr) return;

	// Check if action is starting
	if (elapsedTime == 0.0f)
	{
		// starting. update distance
		opacityDelta = opacity - target->getOpacity();
	}

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

void v3d::FadeTo::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;
	if (target == nullptr) return;

	// Check if action is starting
	if (elapsedTime == 0.0f)
	{
		// starting. update distance
		opacityDelta = opacity - target->getOpacity();
	}

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
