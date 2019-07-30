#include <PreCompiled.h>

#include "Activate.h"

#include "Volt3D/Node/TransformNode.h"

#include "Volt3D/Error/ErrorReport.h"

#include "Volt3D/Core/Logger.h"

#include "Volt3D/Config/LogConfig.h"

v3d::Activate::Activate()
	: v3d::Action()
	, target(nullptr)
	, active(false)
{}

v3d::Activate::~Activate() {}

v3d::Activate * v3d::Activate::create(const bool active)
{
	// Create new instance
	auto newActivate = new (std::nothrow) v3d::Activate();

	// check
	if (newActivate)
	{
		// success.
		newActivate->active = active;

		// Done.
		return newActivate;
	}
	else
	{
		// Report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::ACTIVATE);

		// Failed
		return nullptr;
	}
}

bool v3d::Activate::setTarget(v3d::TransformNode * target)
{
	// Check nullptr
	if (target)
	{
		// set
		this->target = target;

		// Done.
		return true;
	}

	// Failed
	return false;
}

void v3d::Activate::update(const float delta)
{
	// Check nullptr
	if (target == nullptr) return;

	// Set target node active.
	target->setActive(active);
}
