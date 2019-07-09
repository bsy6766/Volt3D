#include <PreCompiled.h>

#include "BaseNode.h"

#include "Utils/Logger.h"

unsigned int v3d::BaseNode::idCounter = 0;

v3d::BaseNode::BaseNode(std::string_view name)
	: name(name)
	, id(++BaseNode::idCounter)
	, active(true)
{}

v3d::BaseNode::~BaseNode() {}

unsigned int v3d::BaseNode::getID() const
{
	return id;
}

std::string v3d::BaseNode::getName() const
{
	return name;
}

void v3d::BaseNode::setActive(const bool active)
{
	this->active = active;
}

bool v3d::BaseNode::isActive() const
{
	return active;
}

void v3d::BaseNode::print(const bool printBase) const
{
	auto& logger = v3d::Logger::getInstance();
	
	logger.info("[BaseNode] name: " + name);
	logger.info("[BaseNode] id: " + std::to_string(id));
}
