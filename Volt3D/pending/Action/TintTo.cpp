#include <PreCompiled.h>

#include "TintTo.h"

#include "Volt3D/Node/TransformNode.h"
#include "Volt3D/Node/RenderNode.h"

#include "Volt3D/Error/ErrorReport.h"

v3d::TintTo::TintTo()
	: v3d::Action()
	, color()
	, colorDelta()
{}

v3d::TintTo::~TintTo() {}

bool v3d::TintTo::init(const float duration, const v3d::Color3F& color)
{
	if (duration < 0.0f)
	{
		return false;
	}

	this->duration = duration;
	this->color = color;

	return true;
}

v3d::TintTo * v3d::TintTo::create(const float duration, const v3d::Color3F& color)
{
	auto newTintTo = new (std::nothrow) v3d::TintTo();

	if (newTintTo)
	{
		if (newTintTo->init(duration, color))
		{
			return newTintTo;
		}
		else
		{
			delete newTintTo;
			return nullptr;
		}
	}
	else
	{
		// report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::TINT_TO);

		return nullptr;
	}
}

bool v3d::TintTo::setTarget(v3d::TransformNode * renderNodeTarget)
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

void v3d::TintTo::updateEase(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// Check if action is starting
	if (elapsedTime == 0.0f)
	{
		// starting. update 
		colorDelta = color - target->getColor();
	}

	elapsedTime += delta;

	if (duration == 0.0f)
	{
		// instant
		target->addColor(colorDelta);
	}
	else
	{
		// interval
		target->addColor(colorDelta * delta);
	}
}

void v3d::TintTo::update(const float delta)
{
	// Reject 0 second update
	if (delta == 0.0f) return;

	// Check if action is starting
	if (elapsedTime == 0.0f)
	{
		// starting. update 
		colorDelta = color - target->getColor();
	}

	if (duration == 0.0f)
	{
		// instant
		target->addColor(colorDelta);

		elapsedTime += delta;
	}
	else
	{
		// interval
		float prevT = getLinearEaseTime();

		elapsedTime += delta;

		float curT = getLinearEaseTime();

		target->addColor(colorDelta * (curT - prevT));
	}
}
