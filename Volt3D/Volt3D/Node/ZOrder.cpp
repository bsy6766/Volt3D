#include <PreCompiled.h>

#include "ZOrder.h"

#include "Utils/Macros.h"

v3d::ZOrder::ZOrder()
	: global(0)
	, local(0)
{}

v3d::ZOrder::ZOrder(const int localZOrder)
	: global(0)
	, local(localZOrder)
{}

v3d::ZOrder::ZOrder(const int globalZOrder, const int localZOrder)
	: global(globalZOrder)
	, local(localZOrder)
{}

v3d::ZOrder::~ZOrder() {}

int v3d::ZOrder::getGlobalZOrder() const
{
	return global;
}

int v3d::ZOrder::getLocalZOrder() const
{
	return local;
}

void v3d::ZOrder::setGlobalZOrder(const int globalZOrder)
{
	global = globalZOrder;
}

void v3d::ZOrder::setLocalZOrder(const int localZOrder)
{
	local = localZOrder;
}

bool v3d::ZOrder::operator==(const v3d::ZOrder& other)
{
	return (this->global == other.global) && (this->local == other.local);
}

bool v3d::ZOrder::isGlobalZOrderMaxInt() const
{
	return this->global == v3d::ZOrder::getMaxZOrder();
}

bool v3d::ZOrder::isLocalZOrderMaxInt() const
{
	return this->local == v3d::ZOrder::getMaxZOrder();
}

int v3d::ZOrder::getMaxZOrder()
{
	return std::numeric_limits<int>::max();
}

int v3d::ZOrder::getMinZOrder()
{
	return std::numeric_limits<int>::min();
}

void v3d::ZOrder::print() const
{
	v3d::Logger::getInstance().info("ZOrder (Global: " + std::to_string(global) + ", Local: " + std::to_string(local) + ")");
}
