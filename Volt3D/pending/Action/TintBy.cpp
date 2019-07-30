#include <PreCompiled.h>

#include "TintBy.h"

#include "Volt3D/Node/TransformNode.h"
#include "Volt3D/Node/RenderNode.h"

#include "Volt3D/Error/ErrorReport.h"

v3d::TintBy::TintBy()
	: v3d::Action()
	, color()
{}

v3d::TintBy::~TintBy() {}

bool v3d::TintBy::init(const float duration, const v3d::Color3F& color)
{
	if (duration < 0.0f)
	{
		return false;
	}

	this->duration = duration;
	this->color = color;

	return true;
}

v3d::TintBy * v3d::TintBy::create(const float duration, const v3d::Color3F& color)
{
	auto newTintBy = new (std::nothrow) v3d::TintBy();

	if (newTintBy)
	{
		if (newTintBy->init(duration, color))
		{
			return newTintBy;
		}
		else
		{
			delete newTintBy;
			return nullptr;
		}
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::TINT_BY);
	}

	// Failed
	return nullptr;
}

bool v3d::TintBy::setTarget(v3d::TransformNode * renderNodeTarget)
{
	// Check nullptr
	if (renderNodeTarget)
	{
		// attempt down cast
		target = dynamic_cast<v3d::RenderNode*>(renderNodeTarget);

		// Check nullptr
		if (target)
		{
			// Done.
			return true;
		}
		// Else, failed downcast
	}
	// Else, nullptr

	// Failed
	return false;
}

void v3d::TintBy::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	elapsedTime += delta;

	if (duration == 0.0f)
	{
		// instant
		target->addColor(color);
	}
	else
	{
		// interval
		target->addColor(color * delta);
	}
}

void v3d::TintBy::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	if (duration == 0.0f)
	{
		// instant
		target->addColor(color);

		elapsedTime += delta;
	}
	else
	{
		// interval
		float prevT = getLinearEaseTime();

		elapsedTime += delta;

		float curT = getLinearEaseTime();

		target->addColor(color * (curT - prevT));
	}
}
